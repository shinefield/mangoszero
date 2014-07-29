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

#ifndef MANGOS_H_WAYPOINTMOVEMENTGENERATOR
#define MANGOS_H_WAYPOINTMOVEMENTGENERATOR

/** @page PathMovementGenerator is used to generate movements
 * of waypoints and flight paths.  Each serves the purpose
 * of generate activities so that it generates updated
 * packets for the players.
 */

#include <vector>
#include <set>

#include "MovementGenerator.h"
#include "WaypointManager.h"
#include "DBCStructure.h"

#define FLIGHT_TRAVEL_UPDATE  100
#define STOP_TIME_FOR_PLAYER  (3 * MINUTE * IN_MILLISECONDS)// 3 Minutes

template<class T, class P>
class MANGOS_DLL_SPEC PathMovementBase
{
    public:
        PathMovementBase() : i_currentNode(0) {}
        virtual ~PathMovementBase() {};

        // template pattern, not defined .. override required
        void LoadPath(T&);
        uint32 GetCurrentNode() const { return i_currentNode; }

    protected:
        P i_path;
        uint32 i_currentNode;
};

/** WaypointMovementGenerator loads a series of way points
 * from the DB and apply it to the creature's movement generator.
 * Hence, the creature will move according to its predefined way points.
 */

template<class T>
class MANGOS_DLL_SPEC WaypointMovementGenerator;

template<>
class MANGOS_DLL_SPEC WaypointMovementGenerator<Creature>
    : public MovementGeneratorMedium< Creature, WaypointMovementGenerator<Creature> >,
  public PathMovementBase<Creature, WaypointPath const*>
{
    public:
        WaypointMovementGenerator(Creature&) : i_nextMoveTime(0), m_isArrivalDone(false), m_lastReachedWaypoint(0) {}
        ~WaypointMovementGenerator() { i_path = NULL; }
        void Initialize(Creature& u);
        void Interrupt(Creature&);
        void Finalize(Creature&);
        void Reset(Creature& u);
        bool Update(Creature& u, const uint32& diff);

        void MovementInform(Creature&);

        MovementGeneratorType GetMovementGeneratorType() const { return WAYPOINT_MOTION_TYPE; }

        // now path movement implmementation
        void LoadPath(Creature& c);

        bool GetResetPosition(Creature&, float& x, float& y, float& z) const;

        void AddToWaypointPauseTime(int32 waitTimeDiff);

        uint32 getLastReachedWaypoint() const { return m_lastReachedWaypoint; }

    private:
        void Stop(int32 time) { i_nextMoveTime.Reset(time); }
        bool Stopped(Creature& u);
        bool CanMove(int32 diff, Creature& u);

        void OnArrived(Creature&);
        void StartMove(Creature&);

        void StartMoveNow(Creature& creature);

        ShortTimeTracker i_nextMoveTime;
        bool m_isArrivalDone;
        uint32 m_lastReachedWaypoint;
};

/** FlightPathMovementGenerator generates movement of the player for the paths
 * and hence generates ground and activities for the player.
 */
class MANGOS_DLL_SPEC FlightPathMovementGenerator
    : public MovementGeneratorMedium< Player, FlightPathMovementGenerator >,
  public PathMovementBase<Player, TaxiPathNodeList const*>
{
    public:
        explicit FlightPathMovementGenerator(TaxiPathNodeList const& pathnodes, uint32 startNode = 0)
        {
            i_path = &pathnodes;
            i_currentNode = startNode;
        }
        void Initialize(Player&);
        void Finalize(Player&);
        void Interrupt(Player&);
        void Reset(Player&);
        bool Update(Player&, const uint32&);
        MovementGeneratorType GetMovementGeneratorType() const override { return FLIGHT_MOTION_TYPE; }

        TaxiPathNodeList const& GetPath() { return *i_path; }
        uint32 GetPathAtMapEnd() const;
        bool HasArrived() const { return (i_currentNode >= i_path->size()); }
        void SetCurrentNodeAfterTeleport();
        void SkipCurrentNode() { ++i_currentNode; }
        bool GetResetPosition(Player&, float& x, float& y, float& z) const;
};

#endif
