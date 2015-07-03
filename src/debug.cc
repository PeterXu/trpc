/* 
 * Copyright (c) 1996-2010 Barton P. Miller
 * 
 * We provide the Paradyn Parallel Performance Tools (below
 * described as "Paradyn") on an AS IS basis, and do not warrant its
 * validity or performance.  We reserve the right to update, modify,
 * or discontinue this software at any time.  We shall have no
 * obligation to supply such updates or modifications or any other
 * form of support to you.
 * 
 * By your use of Paradyn, you understand and agree that we (or any
 * other person or entity with proprietary rights in Paradyn) are
 * under no obligation to provide either maintenance services,
 * update services, notices of latent defects, or correction of
 * defects for Paradyn.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "debug.h"

int papisrv_debug_papisrv = 0;
int papisrv_debug_trpc = 0;
int papisrv_debug_initialized = 0;

static void init_debug()
{
    if(getenv("PAPISRV_DEBUG_PAPISRV"))
        papisrv_debug_papisrv = 1;
    if(getenv("PAPISRV_DEBUG_TRPC"))
        papisrv_debug_trpc = 1;
}

int papisrv_printf_int(const char * format, ...)
{
    if(!papisrv_debug_initialized)
        init_debug();

    if(!papisrv_debug_papisrv) return 0;
    if(NULL == format) return -1;

    va_list va;
    va_start(va,format);
    int ret = vfprintf(stderr, format, va);
    va_end(va);

    return ret;
}

int trpc_printf_int(const char * format, ...)
{
    if(!papisrv_debug_initialized)
        init_debug();

    if(!papisrv_debug_trpc) return 0;
    if(NULL == format) return -1;

    va_list va;
    va_start(va,format);
    int ret = vfprintf(stderr, format, va);
    va_end(va);

    return ret;
}
