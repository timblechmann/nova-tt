//  cross-platform wrapper for nanosleep
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
//  the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
//  Boston, MA 02111-1307, USA.

/** \file thread_priority.hpp */

#ifndef NOVA_TT_THREAD_PRITORITY_HPP
#define NOVA_TT_THREAD_PRITORITY_HPP

#include <cassert>
#include <vector>

#include <pthread.h>

#if (_POSIX_PRIORITY_SCHEDULING - 0) >=  200112L || (_POSIX_MEMLOCK - 0) >=  200112L
#define USE_PTHREAD
#include <sched.h>
#endif


namespace nova
{

inline int thread_priority(void)
{
#ifdef USE_PTHREAD
    pthread_t this_thread = pthread_self();
    int policy;
    struct sched_param param;

    int status = pthread_getschedparam(this_thread, &policy, &param);
    assert(status == 0);

    return param.sched_priority;
#else
    return 0;
#endif
}

namespace detail
{
inline std::pair<int, int> thread_priority_interval(int policy)
{
#ifdef USE_PTHREAD
    int minimum = sched_get_priority_min(policy);
    int maximum = sched_get_priority_max(policy);
    return std::make_pair(minimum, maximum);
#else
    return std::make_pair(0, 0);
#endif
}

inline bool thread_set_priority(int policy, int priority)
{
#ifdef USE_PTHREAD
    pthread_t this_thread = pthread_self();

    struct sched_param parm;
    parm.sched_priority = priority;

    int status = pthread_setschedparam(this_thread, policy, &parm);

    return status == 0;
#else
    return false;
#endif
}

} /* namespace detail */

inline std::pair<int, int> thread_priority_interval(void)
{
#ifdef USE_PTHREAD
    return detail::thread_priority_interval(SCHED_OTHER);
#else
    return std::make_pair(0, 0);
#endif
}

inline std::pair<int, int> thread_priority_interval_rt(void)
{
#ifdef USE_PTHREAD
    return detail::thread_priority_interval(SCHED_FIFO);
#else
    return std::make_pair(0, 0);
#endif
}

inline bool thread_set_priority(int priority)
{
#ifdef USE_PTHREAD
    return detail::thread_set_priority(SCHED_OTHER, priority);
#else
    return false;
#endif
}

inline bool thread_set_priority_rt(int priority)
{
#ifdef USE_PTHREAD
    return detail::thread_set_priority(SCHED_FIFO, priority);
#else
    return false;
#endif
}

}  /* namespace nova */

#endif /* NOVA_TT_THREAD_PRITORITY_HPP */
