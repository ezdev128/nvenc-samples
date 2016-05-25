/*
 * Copyright 1993-2014 NVIDIA Corporation.  All rights reserved.
 *
 * NOTICE TO LICENSEE:
 *
 * This source code and/or documentation ("Licensed Deliverables") are
 * subject to NVIDIA intellectual property rights under U.S. and
 * international Copyright laws.
 *
 * These Licensed Deliverables contained herein is PROPRIETARY and
 * CONFIDENTIAL to NVIDIA and is being provided under the terms and
 * conditions of a form of NVIDIA software license agreement by and
 * between NVIDIA and Licensee ("License Agreement") or electronically
 * accepted by Licensee.  Notwithstanding any terms or conditions to
 * the contrary in the License Agreement, reproduction or disclosure
 * of the Licensed Deliverables to any third party without the express
 * written consent of NVIDIA is prohibited.
 *
 * ALL NVIDIA DESIGN SPECIFICATIONS, REFERENCE BOARDS, FILES, DRAWINGS,
 * DIAGNOSTICS, LISTS, AND OTHER DOCUMENTS (TOGETHER AND SEPARATELY,
 * �MATERIALS�) ARE BEING PROVIDED �AS IS.� WITHOUT EXPRESS OR IMPLIED
 * WARRANTY OF ANY KIND.  NVIDIA DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THESE LICENSED DELIVERABLES, INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE.
 * NOTWITHSTANDING ANY TERMS OR CONDITIONS TO THE CONTRARY IN THE LICENSE
 * AGREEMENT, IN NO EVENT SHALL NVIDIA BE LIABLE FOR ANY SPECIAL, INDIRECT,
 * INCIDENTAL, OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING
 * FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
 * WITH THE USE OR PERFORMANCE OF THESE LICENSED DELIVERABLES.
 *
 * Information furnished is believed to be accurate and reliable. However,
 * NVIDIA assumes no responsibility for the consequences of use of such
 * information nor for any infringement of patents or other rights of
 * third parties, which may result from its use.  No License is granted
 * by implication or otherwise under any patent or patent rights of NVIDIA
 * Corporation.  Specifications mentioned in the software are subject to
 * change without notice. This publication supersedes and replaces all
 * other information previously supplied.
 *
 * NVIDIA Corporation products are not authorized for use as critical
 * components in life support devices or systems without express written
 * approval of NVIDIA Corporation.
 *
 * U.S. Government End Users.  These Licensed Deliverables are a
 * "commercial item" as that term is defined at 48 C.F.R. 2.101 (OCT
 * 1995), consisting of "commercial computer software" and "commercial
 * computer software documentation" as such terms are used in 48
 * C.F.R. 12.212 (SEPT 1995) and is provided to the U.S. Government
 * only as a commercial end item.  Consistent with 48 C.F.R.12.212 and
 * 48 C.F.R. 227.7202-1 through 227.7202-4 (JUNE 1995), all
 * U.S. Government End Users acquire the Licensed Deliverables with
 * only those rights set forth herein.
 *
 * Any use of the Licensed Deliverables in individual and commercial
 * software must include, in the user documentation and internal
 * comments to the code, the above Disclaimer and U.S. Government End
 * Users Notice.
 */

//---------------------------------------------------------------------------
// NvThreadingLinux.h
//
// Linux implementation of the NvThreading::INvThreading interface.
//
// Copyright(c) 2003 NVIDIA Corporation.
//---------------------------------------------------------------------------

#ifndef _NV_THREADING_LINUX_H
#define _NV_THREADING_LINUX_H

#if (NV_PROFILE==1)
#include <sys/time.h>
#endif
#include <sys/resource.h>
#include <sys/types.h>
#include <pthread.h>

#include <threads/NvThreading.h>

class CNvThreadingLinux : public INvThreading
{
    public:
        CNvThreadingLinux();
        virtual ~CNvThreadingLinux();

        //////////////////////////////////////////////////////////////////////
        // Mutex.
        //////////////////////////////////////////////////////////////////////

        virtual NvResult MutexCreate(Handle *puMutexHandle);
        virtual NvResult MutexAcquire(Handle uMutexHandle);
        virtual NvResult MutexTryAcquire(Handle uMutexHandle);
        virtual NvResult MutexRelease(Handle uMutexHandle);
        virtual NvResult MutexDestroy(Handle *puMutexHandle);

        //////////////////////////////////////////////////////////////////////
        // Events.
        //////////////////////////////////////////////////////////////////////

        virtual NvResult EventCreate(Handle *puEventHandle, bool bManual, bool bSet);
        virtual NvResult EventWait(Handle uEventHandle, U32 uTimeoutMs);
        virtual NvResult EventSet(Handle uEventHandle);
        virtual NvResult EventReset(Handle uEventHandle);
        virtual NvResult EventDestroy(Handle *puEventHandle);

        //////////////////////////////////////////////////////////////////////
        // Semaphores.
        //////////////////////////////////////////////////////////////////////

        virtual NvResult SemaphoreCreate(Handle *puSemaphoreHandle, U32 uInitCount, U32 uMaxCount);
        virtual NvResult SemaphoreIncrement(Handle uSemaphoreHandle);
        virtual NvResult SemaphoreDecrement(Handle uSemaphoreHandle, U32 uTimeoutMs);
        virtual NvResult SemaphoreDestroy(Handle *puSemaphoreHandle);

        //////////////////////////////////////////////////////////////////////
        // Timers.
        //////////////////////////////////////////////////////////////////////

        virtual NvResult TimerCreate(Handle *puTimerHandle, bool (*pFunc)(void *pParam), void *pParam, U32 uTimeMs, U32 uPeriodMs);
        virtual NvResult TimerDestroy(Handle *puTimerHandle);

        //////////////////////////////////////////////////////////////////////
        // Threads.
        //////////////////////////////////////////////////////////////////////

        virtual NvResult ThreadCreate(Handle *puThreadHandle, U32(*pFunc)(void *pParam), void *pParam, S32 sPriority);
        virtual NvResult ThreadPriorityGet(Handle uThreadHandle, S32 &rsPriority);
        virtual NvResult ThreadPrioritySet(Handle uThreadHandle, S32 sPriority);
        virtual NvResult ThreadDestroy(Handle *puThreadHandle);
        virtual bool ThreadIsCurrent(Handle uThreadHandle);

        //////////////////////////////////////////////////////////////////////
        // Misc.
        //////////////////////////////////////////////////////////////////////

        virtual U32 GetTicksMs();
        virtual U32 GetThreadID(Handle hThreadHandle);

        //////////////////////////////////////////////////////////////////////
        // Linux specific.
        //////////////////////////////////////////////////////////////////////

        void ResetTimeBase(U32 uOffsetMs);

    private:
        typedef unsigned long long time_ms_t;

        struct CMutexData
        {
            pthread_mutexattr_t mutexattr;
            pthread_mutex_t     mutex;
        };

        struct CNvTimerData
        {
            pthread_cond_t     condition;
            pthread_mutex_t    mutex;
            pthread_t          thread;
            pthread_attr_t     thread_attr;
            time_ms_t          nextTime;
            U32                period;
            bool               exit;
            bool (*pFunc)(void *);
            void              *pParam;
        };

        struct CConditionData
        {
            pthread_cond_t     condition;
            pthread_mutex_t    mutex;
            bool               signaled;
            bool               manual;
        };

        struct CNvThreadData
        {
            U32(*pFunc)(void *);
            pthread_cond_t     condition;
            pthread_mutex_t    mutex;
            void              *pParam;
            pthread_t          thread;
            pthread_attr_t     thread_attr;
            pid_t              pid;
            S32                priority;

#if (NV_PROFILE==1)
            struct itimerval profTimer;
#endif
        };

        struct CNvSemaphoreData
        {
            pthread_cond_t  condition;
            pthread_mutex_t mutex;
            U32             maxCount;
            U32             count;
        };

        time_t initialTime;

        // Inherited from calling thread
        int m_iSchedPolicy;        // SCHED_OTHER = 0, SCHED_FIFO = 1, SCHED_RR = 2
        S32 m_iSchedPriorityMin;   // Minimum priority for scheduling policy
        S32 m_iSchedPriorityMax;   // Maximum priority for scheduling policy
        S32 m_iSchedPriorityBase;  // Base priority

        static void *TimerFunc(void *lpParameter);
        static void *ThreadFunc(void *lpParameter);

        static time_ms_t GetTime();

        virtual NvResult _MutexCreate(Handle *puMutexHandle, bool bIsRecursive);
};

#endif
