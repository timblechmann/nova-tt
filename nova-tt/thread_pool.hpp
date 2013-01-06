//  c++11-style thread pool
//  Copyright (C) 2013 Tim Blechmann
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

#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <condition_variable>
#include <deque>
#include <future>
#include <mutex>
#include <thread>
#include <vector>

namespace nova   {
namespace detail {

struct task_base
{
    virtual void run() = 0;
};

template <typename T>
struct task:
    task_base
{
    typedef std::packaged_task<T()> work;

    explicit task (work && rhs):
        packaged_task(std::move(rhs))
    {}

    void run()
    {
        packaged_task();
    }

    std::packaged_task<T()> packaged_task;
};

// LATER: implement some work-stealing
class thread_pool
{
public:
    thread_pool(size_t thread_count):
        request_stop(false)
    {
        for (size_t i = 0; i != thread_count; ++i)
            worker_threads.emplace_back(std::bind(&thread_pool::worker_thread, this));
    }

    ~thread_pool()
    {
        request_stop = true;
        condition.notify_all();

        for (;;) {
            std::shared_ptr<task_base> work;
            {
                std::unique_lock<std::mutex> lock(queue_guard);
                if (tasks.empty())
                    break;

                work = std::move(tasks.front());
                tasks.pop_front();
            }
            work->run();
        }

        for (auto & thread : worker_threads)
            thread.join();
    }

    template<class Functor>
    auto schedule(Functor f) -> std::future<decltype( f() )>
    {
        typedef decltype(f()) T;

        if (worker_threads.empty())
            return std::async(std::launch::async, f);

        std::packaged_task<T()> job(f);
        std::future<T> ret = job.get_future();

        {
            std::unique_lock<std::mutex> lock(queue_guard);
            std::shared_ptr<task_base> task_ptr (new task<T>(std::move(job)));
            tasks.emplace_back(task_ptr);
        }

        condition.notify_one();
        return ret;
    }

private:
    void worker_thread()
    {
        for (;;) {
            std::shared_ptr<task_base> work;
            {
                std::unique_lock<std::mutex> lock(queue_guard);
                while(!request_stop && tasks.empty())
                    condition.wait(lock);

                if (request_stop && tasks.empty())
                    return;

                work = std::move(tasks.front());
                tasks.pop_front();
            }
            work->run();
        }
    }

    std::vector< std::thread > worker_threads;
    std::deque< std::shared_ptr<task_base> > tasks;

    std::mutex queue_guard;
    std::condition_variable condition;
    bool request_stop;
};

}

using detail::thread_pool;

}

#endif // THREAD_POOL_HPP
