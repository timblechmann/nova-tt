//  cross-platform wrapper for setting thread affinity
//  Copyright (C) 2009 Tim Blechmann
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; see the file COPYING.  If not, write to
//  the Free Software Foundation, Inc., 51 Franklin Street - Fifth Floor,
//  Boston, MA 02110-1301, USA.

/** \file thread_affinity.hpp */

#ifndef NOVA_TT_THREAD_AFFINITY_HPP
#define NOVA_TT_THREAD_AFFINITY_HPP

#ifdef __linux__

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <pthread.h>

namespace nova {

/** set the affinity of a thread
 *
 *  \return true, if successful, false otherwise
 */
inline bool thread_set_affinity(int i)
{
    pthread_t thread = pthread_self();

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    int status = CPU_SET(i, &cpuset);
    int error = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);

    return error == 0;
}

}

#elif __APPLE__

#include <pthread.h>

#include <mach/thread_policy.h>
#include <mach/thread_act.h>

namespace nova {

inline bool thread_set_affinity(int i)
{
    thread_affinity_policy affinityPolicy { i + 1 };
    kern_return_t status = thread_policy_set( pthread_mach_thread_np(pthread_self()),
                                              THREAD_AFFINITY_POLICY,
                                              (thread_policy_t)&affinityPolicy,
                                              THREAD_AFFINITY_POLICY_COUNT
                                            );

    return status == KERN_SUCCESS;
}

}

#else

namespace nova {

inline bool thread_set_affinity(int i)
{
    return false;
}

}

#endif

#endif /* NOVA_TT_THREAD_AFFINITY_HPP */
