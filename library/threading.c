/*
 *  Threading abstraction layer
 *
 *  Copyright (C) 2006-2014, ARM Limited, All Rights Reserved
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_THREADING_C)

#include "mbedtls/threading.h"

#if defined(MBEDTLS_THREADING_PTHREAD)
static int threading_mutex_init_pthread( mbedtls_threading_mutex_t *mutex )
{
    if( mutex == NULL )
        return( MBEDTLS_ERR_THREADING_BAD_INPUT_DATA );

    if( pthread_mutex_init( mutex, NULL ) != 0 )
        return( MBEDTLS_ERR_THREADING_MUTEX_ERROR );

    return( 0 );
}

static int threading_mutex_free_pthread( mbedtls_threading_mutex_t *mutex )
{
    if( mutex == NULL )
        return( MBEDTLS_ERR_THREADING_BAD_INPUT_DATA );

    if( pthread_mutex_destroy( mutex ) != 0 )
        return( MBEDTLS_ERR_THREADING_MUTEX_ERROR );

    return( 0 );
}

static int threading_mutex_lock_pthread( mbedtls_threading_mutex_t *mutex )
{
    if( mutex == NULL )
        return( MBEDTLS_ERR_THREADING_BAD_INPUT_DATA );

    if( pthread_mutex_lock( mutex ) != 0 )
        return( MBEDTLS_ERR_THREADING_MUTEX_ERROR );

    return( 0 );
}

static int threading_mutex_unlock_pthread( mbedtls_threading_mutex_t *mutex )
{
    if( mutex == NULL )
        return( MBEDTLS_ERR_THREADING_BAD_INPUT_DATA );

    if( pthread_mutex_unlock( mutex ) != 0 )
        return( MBEDTLS_ERR_THREADING_MUTEX_ERROR );

    return( 0 );
}

int (*mbedtls_mutex_init)( mbedtls_threading_mutex_t * ) = threading_mutex_init_pthread;
int (*mbedtls_mutex_free)( mbedtls_threading_mutex_t * ) = threading_mutex_free_pthread;
int (*mbedtls_mutex_lock)( mbedtls_threading_mutex_t * ) = threading_mutex_lock_pthread;
int (*mbedtls_mutex_unlock)( mbedtls_threading_mutex_t * ) = threading_mutex_unlock_pthread;
#endif /* MBEDTLS_THREADING_PTHREAD */

#if defined(MBEDTLS_THREADING_ALT)
static int threading_mutex_fail( mbedtls_threading_mutex_t *mutex )
{
    ((void) mutex );
    return( MBEDTLS_ERR_THREADING_BAD_INPUT_DATA );
}

int (*mbedtls_mutex_init)( mbedtls_threading_mutex_t * ) = threading_mutex_fail;
int (*mbedtls_mutex_free)( mbedtls_threading_mutex_t * ) = threading_mutex_fail;
int (*mbedtls_mutex_lock)( mbedtls_threading_mutex_t * ) = threading_mutex_fail;
int (*mbedtls_mutex_unlock)( mbedtls_threading_mutex_t * ) = threading_mutex_fail;

int mbedtls_threading_set_alt( int (*mutex_init)( mbedtls_threading_mutex_t * ),
                       int (*mutex_free)( mbedtls_threading_mutex_t * ),
                       int (*mutex_lock)( mbedtls_threading_mutex_t * ),
                       int (*mutex_unlock)( mbedtls_threading_mutex_t * ) )
{
    mbedtls_mutex_init = mutex_init;
    mbedtls_mutex_free = mutex_free;
    mbedtls_mutex_lock = mutex_lock;
    mbedtls_mutex_unlock = mutex_unlock;

    return( 0 );
}
#endif /* MBEDTLS_THREADING_ALT */

#endif /* MBEDTLS_THREADING_C */