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

#ifndef MANGOS_H_WORLDPACKET
#define MANGOS_H_WORLDPACKET

#include "Common.h"
#include "network/ByteBuffer.h"
#include "Opcodes.h"

// Note: m_opcode and size stored in platfom dependent format
// ignore endianess until send, and converted at receive
class WorldPacket : public ByteBuffer
{
    public:
        // just container for later use
        WorldPacket()                                       : ByteBuffer(0), m_opcode(MSG_NULL_ACTION)
        {
        }
        explicit WorldPacket(uint16 opcode, size_t res = 200) : ByteBuffer(res), m_opcode(opcode) { }
        // copy constructor
        WorldPacket(const WorldPacket& packet)              : ByteBuffer(packet), m_opcode(packet.m_opcode)
        {
        }

        void Initialize(uint16 opcode, size_t newres = 200)
        {
            clear();
            _storage.reserve(newres);
            m_opcode = opcode;
        }

        uint16 GetOpcode() const { return m_opcode; }
        void SetOpcode(uint16 opcode) { m_opcode = opcode; }
        inline const char* GetOpcodeName() const { return LookupOpcodeName(m_opcode); }

    protected:
        uint16 m_opcode;
};
#endif