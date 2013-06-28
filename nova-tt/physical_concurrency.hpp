//  detect if system supports hyperthreading
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

#ifndef NOVA_TT_PHYSICAL_CONCURRENCT_HPP
#define NOVA_TT_PHYSICAL_CONCURRENCT_HPP

#include <fstream>
#include <string>
#include <thread>
#include <vector>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#if defined(__APPLE__)
#include <sys/sysctl.h>
#endif

namespace nova   {

inline size_t physical_concurrency()
{
#if defined(__APPLE__)
	size_t ret;
	size_t sz = sizeof(ret);
	sysctlbyname("hw.physicalcpu", &ret, &sz, NULL, 0);
	return ret;
#elif(__linux__)
	// may fail if some CPUs
	try {
		std::ifstream online_cpus;
		online_cpus.open("/sys/devices/system/cpu/online");

		if (!online_cpus)
			return std::thread::hardware_concurrency();

		std::string line;
		std::getline(online_cpus, line);

		std::vector<std::string> min_max_ids;

		boost::split(min_max_ids, line, boost::is_any_of("-"));

		if ((min_max_ids.size() != 2) &&
			(boost::lexical_cast<size_t>(min_max_ids[0]) != 0) )
			return std::thread::hardware_concurrency();

		return boost::lexical_cast<size_t>(min_max_ids[1]) + 1;
	} catch(...) {
		return std::thread::hardware_concurrency();
	}

#else
	return std::thread::hardware_concurrency();
#endif
}

}

#endif // NOVA_TT_PHYSICAL_CONCURRENCT_HPP
