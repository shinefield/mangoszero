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

#ifndef MANGOS_H_GAMEOBJECTMODEL
#define MANGOS_H_GAMEOBJECTMODEL

#include <G3D/Matrix3.h>
#include <G3D/Vector3.h>
#include <G3D/AABox.h>
#include <G3D/Ray.h>

#include "platform/Define.h"
#include "DBCStructure.h"
#include "GameObject.h"

namespace VMAP
{
    class WorldModel;
}


class GameObjectModel
{
        bool collision_enabled;
        G3D::AABox iBound;
        G3D::Matrix3 iInvRot;
        G3D::Vector3 iPos;
        //G3D::Vector3 iRot;
        float iInvScale;
        float iScale;
        VMAP::WorldModel* iModel;

        GameObjectModel() : collision_enabled(false), iModel(NULL) {}
        bool initialize(const GameObject* const pGo, const GameObjectDisplayInfoEntry* info);

    public:
        std::string name;

        const G3D::AABox& getBounds() const
        {
            return iBound;
        }

        ~GameObjectModel();

        const G3D::Vector3& getPosition() const
        {
            return iPos;
        }

        /** Enables\disables collision. */
        void disable()
        {
            collision_enabled = false;
        }
        void enable(bool enabled)
        {
            collision_enabled = enabled;
        }

        bool intersectRay(const G3D::Ray& Ray, float& MaxDist, bool StopAtFirstHit) const;

        static GameObjectModel* construct(const GameObject* const pGo);
};
#endif
