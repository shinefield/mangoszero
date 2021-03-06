/*
* Copyright (C) 2010 - 2014 Eluna Lua Engine <http://emudevs.com/>
* This program is free software licensed under GPL version 3
* Please see the included DOCS/LICENSE.md for more information
*/

#ifndef WORLDPACKETMETHODS_H
#define WORLDPACKETMETHODS_H

namespace LuaPacket
{
    /**
     * Returns the opcode of the [WorldPacket].
     *
     * @return uint16 opcode
     */
    int GetOpcode(Eluna* E, WorldPacket* packet)
    {
        Eluna::Push(E->L, packet->GetOpcode());
        return 1;
    }

    /**
     * Returns the size of the [WorldPacket].
     *
     * @return uint32 size : size of [WorldPacket]
     */
    int GetSize(Eluna* E, WorldPacket* packet)
    {
        Eluna::Push(E->L, packet->size());
        return 1;
    }

    /**
     * Sets the opcode of the [WorldPacket] by specifying an opcode.
     *
     * @param uint32 opcode : the opcode specified to be set for the [WorldPacket]
     */
    int SetOpcode(Eluna* E, WorldPacket* packet)
    {
        uint32 opcode = Eluna::CHECKVAL<uint32>(E->L, 2);
        if (opcode >= NUM_MSG_TYPES)
            return luaL_argerror(E->L, 2, "valid opcode expected");
        packet->SetOpcode((OpcodesList)opcode);
        return 0;
    }

    /**
     * Reads and returns an int8 value from the [WorldPacket].
     *
     * @return int8 value
     */
    int ReadByte(Eluna* E, WorldPacket* packet)
    {
        int8 _byte;
        (*packet) >> _byte;
        Eluna::Push(E->L, _byte);
        return 1;
    }

    /**
     * Reads and returns a uint8 value from the [WorldPacket].
     *
     * @return uint8 value
     */
    int ReadUByte(Eluna* E, WorldPacket* packet)
    {
        uint8 _ubyte;
        (*packet) >> _ubyte;
        Eluna::Push(E->L, _ubyte);
        return 1;
    }

    /**
     * Reads and returns an int16 value from the [WorldPacket].
     *
     * @return int16 value
     */
    int ReadShort(Eluna* E, WorldPacket* packet)
    {
        int16 _short;
        (*packet) >> _short;
        Eluna::Push(E->L, _short);
        return 1;
    }

    /**
     * Reads and returns a uint16 value from the [WorldPacket].
     *
     * @return uint16 value
     */
    int ReadUShort(Eluna* E, WorldPacket* packet)
    {
        uint16 _ushort;
        (*packet) >> _ushort;
        Eluna::Push(E->L, _ushort);
        return 1;
    }

    /**
     * Reads and returns an int32 value from the [WorldPacket].
     *
     * @return int32 value
     */
    int ReadLong(Eluna* E, WorldPacket* packet)
    {
        int32 _long;
        (*packet) >> _long;
        Eluna::Push(E->L, _long);
        return 1;
    }

    /**
     * Reads and returns a uint32 value from the [WorldPacket].
     *
     * @return uint32 value
     */
    int ReadULong(Eluna* E, WorldPacket* packet)
    {
        uint32 _ulong;
        (*packet) >> _ulong;
        Eluna::Push(E->L, _ulong);
        return 1;
    }

    /**
     * Reads and returns a float value from the [WorldPacket].
     *
     * @return float value
     */
    int ReadFloat(Eluna* E, WorldPacket* packet)
    {
        float _val;
        (*packet) >> _val;
        Eluna::Push(E->L, _val);
        return 1;
    }

    /**
     * Reads and returns a double value from the [WorldPacket].
     *
     * @return double value
     */
    int ReadDouble(Eluna* E, WorldPacket* packet)
    {
        double _val;
        (*packet) >> _val;
        Eluna::Push(E->L, _val);
        return 1;
    }

    /**
     * Reads and returns a uint64 value from the [WorldPacket].
     *
     * @return uint64 value : value returned as string
     */
    int ReadGUID(Eluna* E, WorldPacket* packet)
    {
        uint64 guid;
        (*packet) >> guid;
        Eluna::Push(E->L, guid);
        return 1;
    }

    /**
     * Reads and returns a string value from the [WorldPacket].
     *
     * @return string value
     */
    int ReadString(Eluna* E, WorldPacket* packet)
    {
        std::string _val;
        (*packet) >> _val;
        Eluna::Push(E->L, _val);
        return 1;
    }

    /**
     * Writes an uint64 value to the [WorldPacket].
     *
     * @param uint64 value : the value to be written to the [WorldPacket]
     */
    int WriteGUID(Eluna* E, WorldPacket* packet)
    {
        uint64 guid = Eluna::CHECKVAL<uint64>(E->L, 2);
        (*packet) << guid;
        return 0;
    }

    /**
     * Writes a string to the [WorldPacket].
     *
     * @param string value : the string to be written to the [WorldPacket]
     */
    int WriteString(Eluna* E, WorldPacket* packet)
    {
        std::string _val = Eluna::CHECKVAL<std::string>(E->L, 2);
        (*packet) << _val;
        return 0;
    }

    /**
     * Writes an int8 value to the [WorldPacket].
     *
     * @param int8 value : the int8 value to be written to the [WorldPacket]
     */
    int WriteByte(Eluna* E, WorldPacket* packet)
    {
        int8 byte = Eluna::CHECKVAL<int8>(E->L, 2);
        (*packet) << byte;
        return 0;
    }

    /**
     * Writes an uint8 value to the [WorldPacket].
     *
     * @param uint8 value : the uint8 value to be written to the [WorldPacket]
     */
    int WriteUByte(Eluna* E, WorldPacket* packet)
    {
        uint8 byte = Eluna::CHECKVAL<uint8>(E->L, 2);
        (*packet) << byte;
        return 0;
    }

    /**
     * Writes an int16 value to the [WorldPacket].
     *
     * @param int16 value : the int16 value to be written to the [WorldPacket]
     */
    int WriteShort(Eluna* E, WorldPacket* packet)
    {
        int16 _short = Eluna::CHECKVAL<int16>(E->L, 2);
        (*packet) << _short;
        return 0;
    }

    /**
     * Writes an uint16 value to the [WorldPacket].
     *
     * @param uint16 value : the uint16 value to be written to the [WorldPacket]
     */
    int WriteUShort(Eluna* E, WorldPacket* packet)
    {
        uint16 _ushort = Eluna::CHECKVAL<uint16>(E->L, 2);
        (*packet) << _ushort;
        return 0;
    }

    /**
     * Writes an int32 value to the [WorldPacket].
     *
     * @param int32 value : the int32 value to be written to the [WorldPacket]
     */
    int WriteLong(Eluna* E, WorldPacket* packet)
    {
        int32 _long = Eluna::CHECKVAL<int32>(E->L, 2);
        (*packet) << _long;
        return 0;
    }

    /**
     * Writes an uint32 value to the [WorldPacket].
     *
     * @param uint32 value : the uint32 value to be written to the [WorldPacket]
     */
    int WriteULong(Eluna* E, WorldPacket* packet)
    {
        uint32 _ulong = Eluna::CHECKVAL<uint32>(E->L, 2);
        (*packet) << _ulong;
        return 0;
    }

    /**
     * Writes a float value to the [WorldPacket].
     *
     * @param float value : the float value to be written to the [WorldPacket]
     */
    int WriteFloat(Eluna* E, WorldPacket* packet)
    {
        float _val = Eluna::CHECKVAL<float>(E->L, 2);
        (*packet) << _val;
        return 0;
    }

    /**
     * Writes a double value to the [WorldPacket].
     *
     * @param double value : the double value to be written to the [WorldPacket]
     */
    int WriteDouble(Eluna* E, WorldPacket* packet)
    {
        double _val = Eluna::CHECKVAL<double>(E->L, 2);
        (*packet) << _val;
        return 0;
    }
};

#endif
