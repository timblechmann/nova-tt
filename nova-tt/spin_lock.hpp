//  spin_lock class
//  Copyright (C) 2010 - 2013 Tim Blechmann
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

/** \file spin_lock.hpp */

#ifndef NOVA_TT_SPIN_LOCK_HPP
#define NOVA_TT_SPIN_LOCK_HPP

#include <cassert>

#include <boost/atomic.hpp>

#include "pause.hpp"

namespace nova {

/** spinlock, implements the Lockable concept
 */
class spin_lock
{
    static const bool locked_state = 0;
    static const bool unlocked_state = 1;
    boost::atomic<bool> state;

#if __cplusplus >= 201103L
    spin_lock(spin_lock const & rhs) = delete;
    spin_lock & operator=(spin_lock const & rhs) = delete;
#else
    spin_lock(spin_lock const & rhs);
    spin_lock & operator=(spin_lock const & rhs);
#endif

public:
    struct scoped_lock
    {
        scoped_lock(spin_lock & sl):
            sl_(sl)
        {
            sl_.lock();
        }

        ~scoped_lock(void)
        {
            sl_.unlock();
        }

        spin_lock & sl_;
    };

    spin_lock(void):
        state((bool)unlocked_state)
    {}

    ~spin_lock(void)
    {
        assert (state == (bool)unlocked_state);
    }

    void lock(void)
    {
        for(;;) {
			while (state.load(boost::memory_order_relaxed) != (bool)unlocked_state)
                detail::pause();

            if (try_lock())
                break;
        }
    }

    bool try_lock(void)
    {
        return state.exchange((bool)locked_state, boost::memory_order_acquire) == (bool)unlocked_state;
    }

    void unlock(void)
    {
        assert(state.load(boost::memory_order_relaxed) == (bool)locked_state);
        state.store((bool)unlocked_state, boost::memory_order_release);
    }
};

struct padded_spin_lock:
    public spin_lock
{
    static const int padding_bytes = 64 - sizeof(spin_lock);
    boost::uint8_t padding[padding_bytes];
};

} /* namespace nova */

#endif /* NOVA_TT_SPIN_LOCK_HPP */
