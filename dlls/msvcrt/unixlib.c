/*
 * MSVCRT Unix interface
 *
 * Copyright 2020 Alexandre Julliard
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#if 0
#pragma makedep unix
#endif

#include "config.h"
#include "wine/port.h"

#include <stdio.h>
#include <stdarg.h>
#define __USE_ISOC9X 1
#define __USE_ISOC99 1
#include <math.h>
#ifdef HAVE_IEEEFP_H
#include <ieeefp.h>
#endif

#include "ntstatus.h"
#define WIN32_NO_STATUS
#include "winternl.h"
#include "unixlib.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(msvcrt);

/*********************************************************************
 *      exp
 */
static double CDECL unix_exp( double x )
{
    return exp( x );
}

/*********************************************************************
 *      exp2
 */
static double CDECL unix_exp2( double x )
{
#ifdef HAVE_EXP2
    return exp2(x);
#else
    return pow(2, x);
#endif
}

/*********************************************************************
 *      pow
 */
static double CDECL unix_pow( double x, double y )
{
    return pow( x, y );
}

static const struct unix_funcs funcs =
{
    unix_exp,
    unix_exp2,
    unix_pow,
};

NTSTATUS CDECL __wine_init_unix_lib( HMODULE module, DWORD reason, const void *ptr_in, void *ptr_out )
{
    if (reason != DLL_PROCESS_ATTACH) return STATUS_SUCCESS;
    TRACE( "\n" );
    *(const struct unix_funcs **)ptr_out = &funcs;
    return STATUS_SUCCESS;
}
