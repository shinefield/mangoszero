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

#ifndef MANGOS_H_QUERYRESULT
#define MANGOS_H_QUERYRESULT

#include "Common.h"
#include "debugging/Errors.h"
#include "Field.h"

class MANGOS_DLL_SPEC QueryResult
{
    public:
        QueryResult(uint64 rowCount, uint32 fieldCount)
            : mFieldCount(fieldCount), mRowCount(rowCount) {}

        virtual ~QueryResult() {}

        virtual bool NextRow() = 0;

        Field* Fetch() const
        {
            return mCurrentRow;
        }

        const Field& operator [](int index) const
        {
            return mCurrentRow[index];
        }

        uint32 GetFieldCount() const
        {
            return mFieldCount;
        }
        uint64 GetRowCount() const
        {
            return mRowCount;
        }

    protected:
        Field* mCurrentRow;
        uint32 mFieldCount;
        uint64 mRowCount;
};

typedef std::vector<std::string> QueryFieldNames;

class MANGOS_DLL_SPEC QueryNamedResult
{
    public:
        explicit QueryNamedResult(QueryResult* query, QueryFieldNames const& names) : mQuery(query), mFieldNames(names) {}
        ~QueryNamedResult()
        {
            delete mQuery;
        }

        // compatible interface with QueryResult
        bool NextRow()
        {
            return mQuery->NextRow();
        }
        Field* Fetch() const
        {
            return mQuery->Fetch();
        }
        uint32 GetFieldCount() const
        {
            return mQuery->GetFieldCount();
        }
        uint64 GetRowCount() const
        {
            return mQuery->GetRowCount();
        }
        Field const& operator[](int index) const
        {
            return (*mQuery)[index];
        }

        // named access
        Field const& operator[](const std::string& name) const
        {
            return mQuery->Fetch()[GetField_idx(name)];
        }
        QueryFieldNames const& GetFieldNames() const
        {
            return mFieldNames;
        }

        uint32 GetField_idx(const std::string& name) const
        {
            for (size_t idx = 0; idx < mFieldNames.size(); ++idx)
            {
                if (mFieldNames[idx] == name)
                    return idx;
            }
            MANGOS_ASSERT(false && "unknown field name");
            return uint32(-1);
        }

    protected:
        QueryResult* mQuery;
        QueryFieldNames mFieldNames;
};

#endif
