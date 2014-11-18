/**
 * mangos-zero is a full featured server for World of Warcraft in its vanilla
 * version, supporting clients for patch 1.12.x.
 *
 * Copyright (C) 2005-2014  MaNGOS project  <http://getmangos.com>
 * Parts Copyright (C) 2013-2014  CMaNGOS project <http://cmangos.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * World of Warcraft, and all World of Warcraft or Warcraft art, images,
 * and lore are copyrighted by Blizzard Entertainment, Inc.
 */

#include <openssl/opensslv.h>
#include <openssl/crypto.h>
#include <ace/Get_Opt.h>
#include <ace/Dev_Poll_Reactor.h>
#include <ace/TP_Reactor.h>
#include <ace/ACE.h>
#include <ace/Acceptor.h>
#include <ace/SOCK_Acceptor.h>

#include "Common.h"
#include "configuration/Config.h"
#include "database/DatabaseEnv.h"
#include "log/Log.h"
#include "utilities/Util.h"
#include "revision.h"
#include "revision_nr.h"
#include "revision_sql.h"
#include "system/SystemConfig.h"

#include "RealmList.h"
#include "AuthSocket.h"

#ifdef WIN32
#include "system/ServiceWin32.h"
char serviceName[] = "realmd";
char serviceLongName[] = "MaNGOS realmd service";
char serviceDescription[] = "Massive Network Game Object Server";
/*
 * -1 - not in service mode
 *  0 - stopped
 *  1 - running
 *  2 - paused
 */
int m_ServiceStatus = -1;
#else
#include "system/PosixDaemon.h"
#endif

bool StartDB();
void UnhookSignals();
void HookSignals();

bool stopEvent = false;                                     ///< Setting it to true stops the server

DatabaseType LoginDatabase;                                 ///< Accessor to the realm server database

/// Print out the usage string for this program on the console.
void usage(const char* prog)
{
    sLog.outString("Usage: \n %s [<options>]\n"
                   "    -v, --version            print version and exit\r\n"
                   "    -c config_file           use config_file as configuration file\r\n"
#ifdef WIN32
                   "    Running as service functions:\r\n"
                   "    -s run                   run as service\r\n"
                   "    -s install               install service\r\n"
                   "    -s remove                remove service\r\n"
#else
                   "    Running as daemon functions:\r\n"
                   "    -s run                   run as daemon\r\n"
                   "    -s stop                  stop daemon\r\n"
#endif
                   , prog);
}

/// Launch the realm server
extern int main(int argc, char** argv)
{
    ///- Command line parsing
    char const* cfg_file = _REALMD_CONFIG;

    char const* options = ":c:s:";

    ACE_Get_Opt cmd_opts(argc, argv, options);
    cmd_opts.long_option("version", 'v');

    char serviceDaemonMode = '\0';

    int option;
    while ((option = cmd_opts()) != EOF)
    {
        switch (option)
        {
            case 'c':
                cfg_file = cmd_opts.opt_arg();
                break;
            case 'v':
                printf("%s\n", _FULLVERSION(REVISION_DATE, REVISION_TIME, REVISION_NR, REVISION_ID));
                return 0;

            case 's':
            {
                const char* mode = cmd_opts.opt_arg();

                if (!strcmp(mode, "run"))
                    serviceDaemonMode = 'r';
#ifdef WIN32
                else if (!strcmp(mode, "install"))
                    serviceDaemonMode = 'i';
                else if (!strcmp(mode, "remove"))
                    serviceDaemonMode = 'u';
#else
                else if (!strcmp(mode, "stop"))
                    serviceDaemonMode = 's';
#endif
                else
                {
                    sLog.outError("Runtime-Error: -%c unsupported argument %s", cmd_opts.opt_opt(), mode);
                    usage(argv[0]);
                    Log::WaitBeforeContinueIfNeed();
                    return 1;
                }
                break;
            }
            case ':':
                sLog.outError("Runtime-Error: -%c option requires an input argument", cmd_opts.opt_opt());
                usage(argv[0]);
                Log::WaitBeforeContinueIfNeed();
                return 1;
            default:
                sLog.outError("Runtime-Error: bad format of command line arguments");
                usage(argv[0]);
                Log::WaitBeforeContinueIfNeed();
                return 1;
        }
    }

#ifdef WIN32                                                // windows service command need execute before configuration read
    switch (serviceDaemonMode)
    {
        case 'i':
            if (WinServiceInstall())
                sLog.outString("Installing service");
            return 1;
        case 'u':
            if (WinServiceUninstall())
                sLog.outString("Removing service");
            return 1;
        case 'r':
            WinServiceRun();
            break;
    }
#endif

    if (!sConfig.SetSource(cfg_file))
    {
        sLog.outError("Could not find configuration file %s.", cfg_file);
        Log::WaitBeforeContinueIfNeed();
        return 1;
    }

#ifndef WIN32                                               // POSIX daemon commands need apply after configuration read
    switch (serviceDaemonMode)
    {
        case 'r':
            startDaemon();
            break;
        case 's':
            stopDaemon();
            break;
    }
#endif

    sLog.Initialize();

    sLog.outString("%s [realm-daemon]", _FULLVERSION(REVISION_DATE, REVISION_TIME, REVISION_NR, REVISION_ID));
    sLog.outString("<Ctrl-C> to stop.\n");
    sLog.outString("Using configuration file %s.", cfg_file);

    ///- Check the version of the configuration file
    uint32 confVersion = sConfig.GetIntDefault("ConfVersion", 0);
    if (confVersion < _REALMDCONFVERSION)
    {
        sLog.outError("*****************************************************************************");
        sLog.outError(" WARNING: Your configuration is outdated.");
        sLog.outError("          Please check for updates, as your current default values may cause");
        sLog.outError("          strange behavior.");
        sLog.outError("*****************************************************************************");
        Log::WaitBeforeContinueIfNeed();
    }

    DETAIL_LOG("%s (Library: %s)", OPENSSL_VERSION_TEXT, SSLeay_version(SSLEAY_VERSION));
    if (SSLeay() < 0x009080bfL)
    {
        DETAIL_LOG("WARNING: Outdated version of OpenSSL lib. Logins to server may not work!");
        DETAIL_LOG("WARNING: Minimal required version [OpenSSL 0.9.8k]");
    }

    DETAIL_LOG("Using ACE: %s", ACE_VERSION);

    // Manually delete this pointers, because memory allocating done with overloaded mem manager
    // (function new) removed with 'delete' from internal (standard memman) ace library.
    // As result - crash on exit.
    ACE_Reactor_Impl* aceReactorImp;
    ACE_Reactor* aceReactor;

#if defined (ACE_HAS_EVENT_POLL) || defined (ACE_HAS_DEV_POLL)
    aceReactorImp = new ACE_Dev_Poll_Reactor(ACE::max_handles(), true);
#else
    aceReactorImp = new ACE_TP_Reactor();
#endif

    aceReactor = new ACE_Reactor(aceReactorImp, false);
    ACE_Reactor::instance(aceReactor, false);

    sLog.outBasic("Max allowed open files is %d", ACE::max_handles());

    /// realmd PID file creation
    std::string pidfile = sConfig.GetStringDefault("PidFile", "");
    if (!pidfile.empty())
    {
        uint32 pid = CreatePIDFile(pidfile);
        if (!pid)
        {
            sLog.outError("Can't create PID file %s.\n", pidfile.c_str());
            Log::WaitBeforeContinueIfNeed();
            return 1;
        }

        sLog.outString("Daemon PID: %u\n", pid);
    }

    ///- Initialize the database connection
    if (!StartDB())
    {
        Log::WaitBeforeContinueIfNeed();
        return 1;
    }

    ///- Get the list of realms for the server
    sRealmList.Initialize(sConfig.GetIntDefault("RealmsStateUpdateDelay", 20));
    if (sRealmList.size() == 0)
    {
        sLog.outError("No valid realms specified.");
        Log::WaitBeforeContinueIfNeed();
        return 1;
    }

    // clean up query
    // set expired bans to inactive
    LoginDatabase.BeginTransaction();
    LoginDatabase.Execute("UPDATE account_banned SET active = 0 WHERE unbandate<=UNIX_TIMESTAMP() AND unbandate<>bandate");
    LoginDatabase.Execute("DELETE FROM ip_banned WHERE unbandate<=UNIX_TIMESTAMP() AND unbandate<>bandate");
    LoginDatabase.CommitTransaction();

    ///- Launch the listening network socket
    ACE_Acceptor<AuthSocket, ACE_SOCK_Acceptor> acceptor;

    uint16 rmport = sConfig.GetIntDefault("RealmServerPort", DEFAULT_REALMSERVER_PORT);
    std::string bind_ip = sConfig.GetStringDefault("BindIP", "0.0.0.0");

    ACE_INET_Addr bind_addr(rmport, bind_ip.c_str());

    if (acceptor.open(bind_addr, ACE_Reactor::instance(), ACE_NONBLOCK) == -1)
    {
        sLog.outError("MaNGOS realmd can't bind to %s:%d", bind_ip.c_str(), rmport);
        Log::WaitBeforeContinueIfNeed();
        return 1;
    }

    ///- Catch termination signals
    HookSignals();

    ///- Handle affinity for multiple processors and process priority on Windows
#ifdef WIN32
    {
        HANDLE hProcess = GetCurrentProcess();

        uint32 Aff = sConfig.GetIntDefault("UseProcessors", 0);
        if (Aff > 0)
        {
            ULONG_PTR appAff;
            ULONG_PTR sysAff;

            if (GetProcessAffinityMask(hProcess, &appAff, &sysAff))
            {
                ULONG_PTR curAff = Aff & appAff;            // remove non accessible processors

                if (!curAff)
                {
                    sLog.outError("Processors marked in UseProcessors bitmask (hex) %x not accessible for realmd. Accessible processors bitmask (hex): %x", Aff, appAff);
                }
                else
                {
                    if (SetProcessAffinityMask(hProcess, curAff))
                        sLog.outString("Using processors (bitmask, hex): %x", curAff);
                    else
                        sLog.outError("Can't set used processors (hex): %x", curAff);
                }
            }
            sLog.outString();
        }

        bool Prio = sConfig.GetBoolDefault("ProcessPriority", false);

        if (Prio)
        {
            if (SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS))
                sLog.outString("realmd process priority class set to HIGH");
            else
                sLog.outError("Can't set realmd process priority class.");
            sLog.outString();
        }
    }
#endif

    // server has started up successfully => enable async DB requests
    LoginDatabase.AllowAsyncTransactions();

    // maximum counter for next ping
    uint32 numLoops = (sConfig.GetIntDefault("MaxPingTime", 30) * (MINUTE * 1000000 / 100000));
    uint32 loopCounter = 0;

#ifndef WIN32
    detachDaemon();
#endif
    ///- Wait for termination signal
    while (!stopEvent)
    {
        // don't move this outside the loop, the reactor will modify it
        ACE_Time_Value interval(0, 100000);

        if (ACE_Reactor::instance()->run_reactor_event_loop(interval) == -1)
            break;

        if ((++loopCounter) == numLoops)
        {
            loopCounter = 0;
            DETAIL_LOG("Ping MySQL to keep connection alive");
            LoginDatabase.Ping();
        }
#ifdef WIN32
        if (m_ServiceStatus == 0) stopEvent = true;
        while (m_ServiceStatus == 2) Sleep(1000);
#endif
    }

    delete aceReactor;
    delete aceReactorImp;

    ///- Wait for the delay thread to exit
    LoginDatabase.HaltDelayThread();

    ///- Remove signal handling before leaving
    UnhookSignals();

    sLog.outString("Halting process...");
    return 0;
}

/// Handle termination signals
/** Put the global variable stopEvent to 'true' if a termination signal is caught **/
void OnSignal(int s)
{
    switch (s)
    {
        case SIGINT:
        case SIGTERM:
            stopEvent = true;
            break;
#ifdef _WIN32
        case SIGBREAK:
            stopEvent = true;
            break;
#endif
    }

    signal(s, OnSignal);
}

/// Initialize connection to the database
bool StartDB()
{
    std::string dbstring = sConfig.GetStringDefault("LoginDatabaseInfo", "");
    if (dbstring.empty())
    {
        sLog.outError("Database not specified");
        return false;
    }

    sLog.outString("Login Database total connections: %i", 1 + 1);

    if (!LoginDatabase.Initialize(dbstring.c_str()))
    {
        sLog.outError("Can't connect to database");
        return false;
    }

    if (!LoginDatabase.CheckRequiredField("realmd_db_version", REVISION_DB_REALMD))
    {
        ///- Wait for already started DB delay threads to end
        LoginDatabase.HaltDelayThread();
        return false;
    }

    return true;
}

/// Define hook 'OnSignal' for all termination signals
void HookSignals()
{
    signal(SIGINT, OnSignal);
    signal(SIGTERM, OnSignal);
#ifdef _WIN32
    signal(SIGBREAK, OnSignal);
#endif
}

/// Unhook the signals before leaving
void UnhookSignals()
{
    signal(SIGINT, 0);
    signal(SIGTERM, 0);
#ifdef _WIN32
    signal(SIGBREAK, 0);
#endif
}
