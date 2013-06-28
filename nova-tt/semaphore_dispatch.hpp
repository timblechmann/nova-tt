//  semaphore class, libdispatch wrapper
//  based on API proposed in N2043
//
//  Copyright (C) 2008-2013 Tim Blechmann
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

#ifndef NOVA_TT_SEMAPHORE_DISPATCH_HPP
#define NOVA_TT_SEMAPHORE_DISPATCH_HPP


#include <cassert>
#include <dispatch/dispatch.h>

#include <boost/noncopyable.hpp>
#include <boost/static_assert.hpp>


namespace nova {
namespace nova_tt {

/** semaphore class */
template <bool has_timed_wait = false>
class semaphore:
    boost::noncopyable
{
public:
    semaphore(unsigned int i=0)
    {
        sem = dispatch_semaphore_create(i);
    }

    ~semaphore(void)
    {}

    /** signal semaphore */
    void post(void)
    {
        long status = dispatch_semaphore_signal(sem);
    }

    /** wait until this semaphore is signaled */
    void wait(void)
    {
        long status = dispatch_semaphore_wait(sem, DISPATCH_TIME_FOREVER);
    }

    /** try to wait for the semaphore
     *
     * \return true, if the value can be decremented
     *         false, otherweise
     */
    bool try_wait(void)
    {
        long status = dispatch_semaphore_wait(sem, DISPATCH_TIME_NOW);
        return status == 0;
    }

    /** try to wait for the semaphore until timeout
     *
     * \return true, if the value can be decremented
     *         false, otherweise
     */
    bool timed_wait(struct timespec const & absolute_timeout)
    {
        dispatch_time_t timeout = dispatch_walltime(&absolute_timeout, 0);
        long status = dispatch_semaphore_wait(sem, timeout);

        return status == 0;
    }

    int value(void)
    {
        assert(false);
    }

private:
    dispatch_semaphore_t sem;
};

} // namespace nova_tt
} // namespace nova

#endif /* NOVA_TT_SEMAPHORE_POSIX_HPP */
