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

#ifndef MANGOS_H_THREADING
#define MANGOS_H_THREADING

#include <assert.h>
#include <ace/Thread.h>
#include <ace/TSS_T.h>
#include <ace/Atomic_Op.h>

namespace ACE_Based
{
    class Runnable
    {
        public:
            virtual ~Runnable() {}
            virtual void run() = 0;

            void incReference()
            {
                ++m_refs;
            }
            void decReference()
            {
                if (!--m_refs)
                    delete this;
            }
        private:
            ACE_Atomic_Op<ACE_Thread_Mutex, long> m_refs;
    };

    enum Priority
    {
        Idle,
        Lowest,
        Low,
        Normal,
        High,
        Highest,
        Realtime,
    };

#define MAXPRIORITYNUM (Realtime + 1)

    class ThreadPriority
    {
        public:
            ThreadPriority();
            int getPriority(Priority p) const;

        private:
            int m_priority[MAXPRIORITYNUM];
    };

    class Thread
    {
        public:
            Thread();
            explicit Thread(Runnable* instance);
            ~Thread();

            bool start();
            bool wait();
            void destroy();

            void suspend();
            void resume();

            void setPriority(Priority type);

            static void Sleep(unsigned long msecs);
            static ACE_thread_t currentId();
            static ACE_hthread_t currentHandle();
            static Thread* current();

        private:
            Thread(const Thread&);
            Thread& operator=(const Thread&);

            static ACE_THR_FUNC_RETURN ThreadTask(void* param);

            ACE_thread_t m_iThreadId;
            ACE_hthread_t m_hThreadHandle;
            Runnable* m_task;

            typedef ACE_TSS<Thread> ThreadStorage;
            // global object - container for Thread class representation of every thread
            static ThreadStorage m_ThreadStorage;
            // use this object to determine current OS thread priority values mapped to enum Priority{}
            static ThreadPriority m_TpEnum;
    };
}
#endif
