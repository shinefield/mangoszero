/**
 * mangos-zero is a full featured server for World of Warcraft in its vanilla
 * version, supporting clients for patch 1.12.x.
 *
 * Copyright (C) 2005-2014  MaNGOS project  <http://getmangos.com>
 * Parts Copyright (C) 2006-2014  ScriptDev2 <http://scriptdev2.com/>
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

#ifndef SC_GRIDSEARCH_H
#define SC_GRIDSEARCH_H

#include "Object.h"
class GameObject;
class Creature;

struct ObjectDistanceOrder : public std::binary_function<const WorldObject, const WorldObject, bool>
{
    const Unit* m_pSource;

    ObjectDistanceOrder(const Unit* pSource) : m_pSource(pSource) {};

    bool operator()(const WorldObject* pLeft, const WorldObject* pRight) const
    {
        return m_pSource->GetDistanceOrder(pLeft, pRight);
    }
};

struct ObjectDistanceOrderReversed : public std::binary_function<const WorldObject, const WorldObject, bool>
{
    const Unit* m_pSource;

    ObjectDistanceOrderReversed(const Unit* pSource) : m_pSource(pSource) {};

    bool operator()(const WorldObject* pLeft, const WorldObject* pRight) const
    {
        return !m_pSource->GetDistanceOrder(pLeft, pRight);
    }
};

GameObject* GetClosestGameObjectWithEntry(WorldObject* pSource, uint32 uiEntry, float fMaxSearchRange);
Creature* GetClosestCreatureWithEntry(WorldObject* pSource, uint32 uiEntry, float fMaxSearchRange, bool bOnlyAlive = true, bool bOnlyDead = false, bool bExcludeSelf = false);

void GetGameObjectListWithEntryInGrid(std::list<GameObject*>& lList , WorldObject* pSource, uint32 uiEntry, float fMaxSearchRange);
void GetCreatureListWithEntryInGrid(std::list<Creature*>& lList, WorldObject* pSource, uint32 uiEntry, float fMaxSearchRange);

// Used in: hyjalAI.cpp
/*
class AllFriendlyCreaturesInGrid
{
    public:
        AllFriendlyCreaturesInGrid(Unit const* obj) : pUnit(obj) {}
        bool operator() (Unit* u)
        {
            if (u->isAlive() && u->GetVisibility() == VISIBILITY_ON && u->IsFriendlyTo(pUnit))
                return true;

            return false;
        }

    private:
        Unit const* pUnit;
};
*/

#endif
