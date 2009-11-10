//  spin_lock class, wrapper for apple's spinlock
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

#ifndef NOVA_TT_SPIN_LOCK_APPLE_HPP
#define NOVA_TT_SPIN_LOCK_APPLE_HPP

#include <boost/noncopyable.hpp>
#include <boost/thread/locks.hpp>

#include <libkern/OSAtomic.h>

namespace nova
{

/** spinlock, implements the Lockable concept
 */
class spin_lock:
    public boost::noncopyable
{
public:
    spin_lock(void):
        lock_(0)
    {}

    ~spin_lock(void)
    {}

    void lock(void)
    {
        OSSpinLockLock(&lock_);
    }

    bool try_lock(void)
    {
        return OSSpinLockTry(&lock_);
    }

    void unlock(void)
    {
        OSSpinLockUnlock(&lock_);
    }

    typedef boost::lock_guard<spin_lock> scoped_lock;

private:
    OSSpinLock lock_;
};

} /* namespace nova */

#endif /* NOVA_TT_SPIN_LOCK_APPLE_HPP */
