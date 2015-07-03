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
#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdlib.h>
#include <stdio.h>

extern int trpc_printf_int(const char *format, ...);
extern int papisrv_printf_int(const char *format, ...);

extern int papisrv_debug_papisrv;
extern int papisrv_debug_trpc;
extern int papisrv_debug_initialized;

#if defined(__GNUC__)
#define trpc_printf(format, args...) do { \
     if(!papisrv_debug_initialized || papisrv_debug_trpc)\
        trpc_printf_int(format, ## args); } while(0)
#define papisrv_printf(format, args...) do { \
     if(!papisrv_debug_initialized || papisrv_debug_papisrv)\
        papisrv_printf_int(format, ## args); } while(0)
#else
#define papisrv_printf papisrv_printf_int
#define trpc_printf trpc_printf_int
#endif

#endif
