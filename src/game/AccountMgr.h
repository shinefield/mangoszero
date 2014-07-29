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

#ifndef MANGOS_H_ACCMGR
#define MANGOS_H_ACCMGR

#include <string>

#include "Policies/Singleton.h"
#include "Common.h"

enum AccountOpResult
{
    AOR_OK,
    AOR_NAME_TOO_LONG,
    AOR_PASS_TOO_LONG,
    AOR_NAME_ALREDY_EXIST,
    AOR_NAME_NOT_EXIST,
    AOR_DB_INTERNAL_ERROR
};

#define MAX_ACCOUNT_STR 16

class AccountMgr
{
    public:
        AccountMgr();
        ~AccountMgr();

        AccountOpResult CreateAccount(std::string username, std::string password);
        AccountOpResult DeleteAccount(uint32 accid);
        AccountOpResult ChangeUsername(uint32 accid, std::string new_uname, std::string new_passwd);
        AccountOpResult ChangePassword(uint32 accid, std::string new_passwd);
        bool CheckPassword(uint32 accid, std::string passwd);

        uint32 GetId(std::string username);
        AccountTypes GetSecurity(uint32 acc_id);
        bool GetName(uint32 acc_id, std::string& name);
        uint32 GetCharactersCount(uint32 acc_id);
        std::string CalculateShaPassHash(std::string& name, std::string& password);

        static bool normalizeString(std::string& utf8str);
};

#define sAccountMgr MaNGOS::Singleton<AccountMgr>::Instance()

#endif
