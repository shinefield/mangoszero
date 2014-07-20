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

#ifndef MANGOS_RANDOMMOTIONGENERATOR_H
#define MANGOS_RANDOMMOTIONGENERATOR_H

#include "MovementGenerator.h"

// define chance for creature to not stop after reaching a waypoint
#define MOVEMENT_RANDOM_MMGEN_CHANCE_NO_BREAK 30

template<class T>
class MANGOS_DLL_SPEC RandomMovementGenerator
    : public MovementGeneratorMedium< T, RandomMovementGenerator<T> >
{
    public:
        explicit RandomMovementGenerator(const Creature&);
        explicit RandomMovementGenerator(float x, float y, float z, float radius, float verticalZ = 0.0f) :
            i_nextMoveTime(0), i_x(x), i_y(y), i_z(z), i_radius(radius), i_verticalZ(verticalZ) {}

        void _setRandomLocation(T&);
        void Initialize(T&);
        void Finalize(T&);
        void Interrupt(T&);
        void Reset(T&);
        bool Update(T&, const uint32&);
        MovementGeneratorType GetMovementGeneratorType() const override { return RANDOM_MOTION_TYPE; }
    private:
        ShortTimeTracker i_nextMoveTime;
        float i_x, i_y, i_z;
        float i_radius;
        float i_verticalZ;
};

#endif
