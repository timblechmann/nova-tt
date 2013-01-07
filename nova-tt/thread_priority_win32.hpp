//
//  Copyright (C) 2012 Tim Blechmann
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

#ifndef NOVA_TT_THREAD_PRITORITY_WIN32_HPP
#define NOVA_TT_THREAD_PRITORITY_WIN32_HPP

#include <cassert>
#include <vector>

#include "Windows.h"

#define NOVA_TT_PRIORITY_RT

namespace nova {

inline std::pair<int, int> thread_priority_interval()
{
    return std::make_pair(THREAD_PRIORITY_IDLE, THREAD_PRIORITY_NORMAL);
}

inline bool thread_set_priority(int priority)
{
    HANDLE this_thread = GetCurrentThread();
    return SetThreadPriority(this_thread, priority);
}

inline std::pair<int, int> thread_priority_interval_rt()
{
    return std::make_pair(THREAD_PRIORITY_ABOVE_NORMAL, THREAD_PRIORITY_TIME_CRITICAL);
}

inline bool thread_set_priority_rt(int priority)
{
    return thread_set_priority(priority);
}

}  /* namespace nova */

#endif /* NOVA_TT_THREAD_PRITORITY_WIN32_HPP */
