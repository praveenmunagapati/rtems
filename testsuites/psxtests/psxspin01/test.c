/*
 *  This test exercises the POSIX Spinlock manager.
 *
 *  COPYRIGHT (c) 1989-2006.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include <pthread.h>

#include <rtems.h>  /* for task creation */

#define NUMBER_THREADS 2
pthread_t ThreadIds[NUMBER_THREADS];
pthread_spinlock_t Spinlock;

volatile int mainThreadSpinning;

rtems_task SpinlockThread(rtems_task_argument arg)
{
  int  status;

  if ( mainThreadSpinning ) {
    puts( "main thread is not supposed to be spinning yet" );
    exit(0);
  }
  puts( "pthread_spin_lock( &Spinlock ) from Thread -- OK" );
  status = pthread_spin_lock( &Spinlock );
  assert( status == 0 );

  puts( "sleep to allow main thread to run" );
  sleep( 1 );

  if ( !mainThreadSpinning ) {
    puts( "main thread is not spinning on lock" );
    exit(0);
  }

  puts( "pthread_spin_unlock( &Spinlock ) from Thread -- OK" );
  status = pthread_spin_unlock( &Spinlock );
  assert( status == 0 );

  rtems_task_delete( RTEMS_SELF );
}

/*
 *  main entry point to the test
 */

#if defined(__rtems__)
int test_main(void)
#else
int main(
  int    argc,
  char **argv
)
#endif
{
  pthread_spinlock_t    spinlock;
  pthread_spinlock_t    spinlockExtra;
  int                   status;
  int                   p;
  pthread_t             thread_id;
  int                   i;
  rtems_status_code     rstatus;
  rtems_id              taskid;

  puts( "\n\n*** POSIX SPINLOCK TEST 01 ***" );

  puts( "pthread_spin_init( NULL, PTHREAD_PROCESS_PRIVATE ) -- EINVAL" );
  status = pthread_spin_init( NULL, PTHREAD_PROCESS_PRIVATE );
  assert( status == EINVAL );

  puts( "pthread_spin_init( NULL, PTHREAD_PROCESS_SHARED ) -- EINVAL" );
  status = pthread_spin_init( NULL, PTHREAD_PROCESS_PRIVATE );
  assert( status == EINVAL );

  puts( "pthread_spin_init( &spinlock, 0x1234 ) -- EINVAL" );
  status = pthread_spin_init( &spinlock, 0x1234 );
  assert( status == EINVAL );

  puts( "pthread_spin_init( &spinlock, PTHREAD_PROCESS_SHARED ) -- EINVAL" );
  status = pthread_spin_init( &spinlock, PTHREAD_PROCESS_SHARED );
  assert( status == EINVAL );

  /* This successfully creates one */
  puts( "pthread_spin_init( &Spinlock, PTHREAD_PROCESS_PRIVATE ) -- OK" );
  status = pthread_spin_init( &Spinlock, PTHREAD_PROCESS_PRIVATE );
  assert( status == 0 );

  puts( "pthread_spin_init( &spinlock, PTHREAD_PROCESS_PRIVATE ) -- EAGAIN" );
  status = pthread_spin_init( &spinlock, PTHREAD_PROCESS_PRIVATE );
  assert( status == EAGAIN );

  puts( "pthread_spin_init( &spinlock, PTHREAD_PROCESS_PRIVATE ) -- EAGAIN" );
  status = pthread_spin_init( &spinlock, PTHREAD_PROCESS_PRIVATE );
  assert( status == EAGAIN );

  puts( "pthread_spin_lock( NULL ) -- EINVAL" );
  status = pthread_spin_lock( NULL );
  assert( status == EINVAL );

  puts( "pthread_spin_trylock( NULL ) -- EINVAL" );
  status = pthread_spin_trylock( NULL );
  assert( status == EINVAL );

  puts( "pthread_spin_unlock( NULL ) -- EINVAL" );
  status = pthread_spin_unlock( NULL );
  assert( status == EINVAL );

  puts( "pthread_spin_destroy( NULL ) -- EINVAL" );
  status = pthread_spin_destroy( NULL );
  assert( status == EINVAL );

  spinlock = 0;

  puts( "pthread_spin_lock( &spinlock ) -- EINVAL" );
  status = pthread_spin_lock( &spinlock );
  assert( status == EINVAL );

  puts( "pthread_spin_trylock( &spinlock ) -- EINVAL" );
  status = pthread_spin_trylock( &spinlock );
  assert( status == EINVAL );

  puts( "pthread_spin_unlock( &spinlock ) -- EINVAL" );
  status = pthread_spin_unlock( &spinlock );
  assert( status == EINVAL );

  puts( "pthread_spin_destroy( &spinlock ) -- EINVAL" );
  status = pthread_spin_destroy( &spinlock );
  assert( status == EINVAL );

  puts( "pthread_spin_unlock( &Spinlock ) -- already unlocked OK" );
  status = pthread_spin_unlock( &Spinlock );
  assert( status == 0 );

  /* Now some basic locking and unlocking with a deadlock verification */
  puts( "pthread_spin_lock( &Spinlock ) -- OK" );
  status = pthread_spin_lock( &Spinlock );
  assert( status == 0 );

  puts( "pthread_spin_lock( &Spinlock ) -- EDEADLK" );
  status = pthread_spin_lock( &Spinlock );
  assert( status == EDEADLK );

  puts( "pthread_spin_trylock( &Spinlock ) -- EDEADLK" );
  status = pthread_spin_trylock( &Spinlock );
  assert( status == EDEADLK );

  puts( "pthread_spin_unlock( &Spinlock ) -- OK" );
  status = pthread_spin_unlock( &Spinlock );
  assert( status == 0 );

  /* Try lock/unlock pair */
  puts( "pthread_spin_trylock( &Spinlock ) -- OK" );
  status = pthread_spin_trylock( &Spinlock );
  assert( status == 0 );

  puts( "pthread_spin_unlock( &Spinlock ) -- OK" );
  status = pthread_spin_unlock( &Spinlock );
  assert( status == 0 );

  /* Let another thread lock a spinlock and we contend with it */

  mainThreadSpinning = 0;

  /*  Create a helper task */
  rstatus = rtems_task_create(
     rtems_build_name( 'S', 'P', 'I', 'N' ),
     1,
     RTEMS_MINIMUM_STACK_SIZE,
     RTEMS_DEFAULT_MODES,
     RTEMS_DEFAULT_ATTRIBUTES,
     &taskid
  );
  assert( rstatus == RTEMS_SUCCESSFUL );

  rstatus = rtems_task_start( taskid, SpinlockThread, 0 );
  assert( rstatus == RTEMS_SUCCESSFUL );
  /* We should be preempted immediately.  The thread is expected to:
   *    + verify we haven't set the main thread spinning flag
   *    + lock the spinlock
   *    + delay
   */

  mainThreadSpinning = 1;
  puts( "pthread_spin_lock( &Spinlock ) -- OK" );
  status = pthread_spin_lock( &Spinlock );
  assert( status == 0 );

  /* The thread wakes up, unlocks spin lock, and deletes itself.
   * So when we get back here, about a second has passed and we now
   * have the spinlock locked.
   */

  /* spin lock should be locked when we return so destroying it gives busy */
  puts( "pthread_spin_destroy( &Spinlock ) -- EBUSY" );
  status = pthread_spin_destroy( &Spinlock );
  assert( status == EBUSY );

  /* Unlock it for a normal destroy */
  puts( "pthread_spin_unlock( &Spinlock ) -- OK" );
  status = pthread_spin_unlock( &Spinlock );
  assert( status == 0 );

  puts( "pthread_spin_destroy( &Spinlock ) -- OK" );
  status = pthread_spin_destroy( &Spinlock );
  assert( status == 0 );

  /*************** END OF TEST *****************/
  puts( "*** END OF POSIX SPINLOCK TEST 01 ***" );
  exit(0);
}
