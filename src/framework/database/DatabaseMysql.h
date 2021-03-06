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

#ifndef MANGOS_H_DATABASEMYSQL
#define MANGOS_H_DATABASEMYSQL

#include "policies/Singleton.h"
#include <ace/Thread_Mutex.h>
#include <ace/Guard_T.h>
#ifdef WIN32
#include <winsock2.h>
#endif
#include <mysql.h>

#include "Database.h"

// MySQL prepared statement class
class MANGOS_DLL_SPEC MySqlPreparedStatement : public SqlPreparedStatement
{
    public:
        MySqlPreparedStatement(const std::string& fmt, SqlConnection& conn, MYSQL* mysql);
        ~MySqlPreparedStatement();

        // prepare statement
        virtual bool prepare() override;

        // bind input parameters
        virtual void bind(const SqlStmtParameters& holder) override;

        // execute DML statement
        virtual bool execute() override;

    protected:
        // bind parameters
        void addParam(unsigned int nIndex, const SqlStmtFieldData& data);

        static enum_field_types ToMySQLType(const SqlStmtFieldData& data, my_bool& bUnsigned);

    private:
        void RemoveBinds();

        MYSQL* m_pMySQLConn;
        MYSQL_STMT* m_stmt;
        MYSQL_BIND* m_pInputArgs;
        MYSQL_BIND* m_pResult;
        MYSQL_RES* m_pResultMetadata;
};

class MANGOS_DLL_SPEC MySQLConnection : public SqlConnection
{
    public:
        MySQLConnection(Database& db) : SqlConnection(db), mMysql(NULL) {}
        ~MySQLConnection();

        //! Initializes Mysql and connects to a server.
        /*! infoString should be formated like hostname;username;password;database. */
        bool Initialize(const char* infoString) override;

        QueryResult* Query(const char* sql) override;
        QueryNamedResult* QueryNamed(const char* sql) override;
        bool Execute(const char* sql) override;

        unsigned long escape_string(char* to, const char* from, unsigned long length);

        bool BeginTransaction() override;
        bool CommitTransaction() override;
        bool RollbackTransaction() override;

    protected:
        SqlPreparedStatement* CreateStatement(const std::string& fmt) override;

    private:
        bool _TransactionCmd(const char* sql);
        bool _Query(const char* sql, MYSQL_RES** pResult, MYSQL_FIELD** pFields, uint64* pRowCount, uint32* pFieldCount);

        MYSQL* mMysql;
};

class MANGOS_DLL_SPEC DatabaseMysql : public Database
{
        friend class MaNGOS::OperatorNew<DatabaseMysql>;

    public:
        DatabaseMysql();
        ~DatabaseMysql();

        // must be call before first query in thread
        void ThreadStart() override;
        // must be call before finish thread run
        void ThreadEnd() override;

    protected:
        virtual SqlConnection* CreateConnection() override;

    private:
        static size_t db_count;
};

#endif
