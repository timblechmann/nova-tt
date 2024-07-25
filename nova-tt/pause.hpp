//  spin_lock class
//  Copyright (C) 2010, 2011 Tim Blechmann
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

/** \file spin_lock.hpp */

#ifndef NOVA_TT_PAUSE_HPP
#define NOVA_TT_PAUSE_HPP

#if defined(_MSC_VER)
#include <Windows.h>
#elif defined(__x86_64__) || defined(__SSE2__)
/* amd64 resp. SSE2 */
#define NOVA_TT_USE_MM_PAUSE
#include <immintrin.h>
#elif defined(__i386__) && defined(__GNUC__)
#define NOVA_TT_USE_REP_NOP
#elif defined(__aarch64__) && defined(__GNUC__)
/* ARM64 */
#define NOVA_TT_USE_ARM_ISB
#elif defined(__arm__) && defined(__GNUC__) && \
    (defined(__ARM_ARCH_6K__) || \
    defined(__ARM_ARCH_6Z__) || \
    defined(__ARM_ARCH_6ZK__) || \
    defined(__ARM_ARCH_6T2__) || \
    defined(__ARM_ARCH_7__) || \
    defined(__ARM_ARCH_7A__) || \
    defined(__ARM_ARCH_7R__) || \
    defined(__ARM_ARCH_7M__) || \
    defined(__ARM_ARCH_7S__) || \
    defined(__ARM_ARCH_8A__))
/* mnemonic 'yield' is supported from ARMv6k onwards */
#define NOVA_TT_USE_ARM_YIELD
#else /* fall back to busy-waiting */
#warning "Cannot pause CPU, falling back to busy-waiting."
#endif

namespace nova   {
namespace detail {

static inline void pause()
{
#if defined(_MSC_VER)
	YieldProcessor(); /* expands to pause/yield instruction */
#elif defined(NOVA_TT_USE_MM_PAUSE)
	_mm_pause();
#elif defined(NOVA_TT_USE_REP_NOP)
	__asm__ __volatile__( "rep; nop" : : : "memory" );
#elif defined(NOVA_TT_USE_ARM_ISB)
	__asm__ __volatile__("isb");
#elif defined(NOVA_TT_USE_ARM_YIELD)
	__asm__ __volatile__("yield");
#endif
}

}}

#endif /* NOVA_TT_PAUSE_HPP */
