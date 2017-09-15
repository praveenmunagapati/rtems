/**
 * @file
 *
 * @brief Call to Function Enables Reinitializing of the Barrier
 * @ingroup POSIXAPI
 */

/*
 *  POSIX Barrier Manager -- Initialize a Barrier Instance
 *
 *  COPYRIGHT (c) 1989-2006.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <pthread.h>
#include <errno.h>

#include <rtems/system.h>
#include <rtems/posix/barrierimpl.h>
#include <rtems/posix/posixapi.h>

/*
 *  pthread_barrier_init
 *
 *  This directive creates a barrier.  A barrier id is returned.
 *
 *  Input parameters:
 *    barrier          - pointer to barrier id
 *    attr             - barrier attributes
 *    count            - number of threads before automatic release
 *
 *  Output parameters:
 *    barrier     - barrier id
 *    0           - if successful
 *    error code  - if unsuccessful
 */

int pthread_barrier_init(
  pthread_barrier_t           *barrier,
  const pthread_barrierattr_t *attr,
  unsigned int                 count
)
{
  POSIX_Barrier_Control         *the_barrier;
  CORE_barrier_Attributes        the_attributes;
  pthread_barrierattr_t          my_attr;
  const pthread_barrierattr_t   *the_attr;

  /*
   *  Error check parameters
   */
  if ( !barrier )
    return EINVAL;

  if ( count == 0 )
    return EINVAL;

  /*
   * If the user passed in NULL, use the default attributes
   */
  if ( attr ) {
    the_attr = attr;
  } else {
    (void) pthread_barrierattr_init( &my_attr );
    the_attr = &my_attr;
  }

  /*
   * Now start error checking the attributes that we are going to use
   */
  if ( !the_attr->is_initialized )
    return EINVAL;

  if ( !_POSIX_Is_valid_pshared( the_attr->process_shared ) ) {
    return EINVAL;
  }

  /*
   * Convert from POSIX attributes to Core Barrier attributes
   */
  the_attributes.discipline    = CORE_BARRIER_AUTOMATIC_RELEASE;
  the_attributes.maximum_count = count;

  the_barrier = _POSIX_Barrier_Allocate();

  if ( !the_barrier ) {
    _Objects_Allocator_unlock();
    return EAGAIN;
  }

  _CORE_barrier_Initialize( &the_barrier->Barrier, &the_attributes );

  _Objects_Open_u32(
    &_POSIX_Barrier_Information,
    &the_barrier->Object,
    0
  );

  *barrier = the_barrier->Object.id;
  _Objects_Allocator_unlock();
  return 0;
}
