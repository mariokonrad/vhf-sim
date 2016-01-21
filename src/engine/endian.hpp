#ifndef __ENDIAN__HPP__
#define __ENDIAN__HPP__

/* LICENSE: BSD
 *
 * Copyright (c) 2010, Mario Konrad. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * - The names of its contributors must not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>

namespace endian
{
template <typename T> inline T swap_bytes(T v)
{
	union {
		T v;
		uint8_t c[sizeof(T)];
	} x;
	x.v = v;
	uint8_t t;
	for (unsigned int i = 0; i < sizeof(T) / 2; ++i) {
		t = x.c[sizeof(T) - 1 - i];
		x.c[sizeof(T) - 1 - i] = x.c[i];
		x.c[i] = t;
	}
	return x.v;
}

template <> inline uint8_t swap_bytes(uint8_t v) { return v; }

template <typename T> inline bool is_little()
{
	const static union U {
		T x;
		uint8_t c[sizeof(T)];
	} u = {1};
	static const bool LITTLE = u.c[0] == 0x01;
	// static const bool LITTLE = ((union { T x; uint8_t c[sizeof(T)]; }) { 0x01}).c[0] == 0x01;
	// // disabled: not ANSI C++
	return LITTLE;
}

template <typename T> inline bool is_big() { return !is_little<T>(); }

template <typename T> inline T ntoh(T v) { return is_little<T>() ? swap_bytes(v) : v; }

template <typename T> inline T hton(T v) { return is_little<T>() ? swap_bytes(v) : v; }

template <typename T> inline T to_big(T v) { return is_little<T>() ? swap_bytes(v) : v; }

template <typename T> inline T to_little(T v) { return is_little<T>() ? v : swap_bytes(v); }

inline bool is_little() { return is_little<uint32_t>(); }

inline bool is_big() { return is_big<uint32_t>(); }
}

#endif
