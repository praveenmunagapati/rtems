/*  config.h
 *
 *  This include file defines the Configuration Table for this test.
 *
 *  COPYRIGHT (c) 1989-1997.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may in
 *  the file LICENSE in this distribution or at
 *  http://www.rtems.org/license/LICENSE.
 */


/* configuration information */

#define CONFIGURE_TMTEST

#define CONFIGURE_APPLICATION_NEEDS_SIMPLE_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_TIMER_DRIVER

#define CONFIGURE_MAXIMUM_TASKS              102
#define CONFIGURE_MAXIMUM_TIMERS             100
#define CONFIGURE_MAXIMUM_SEMAPHORES         100
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES     100
#define CONFIGURE_MAXIMUM_PARTITIONS         100
#define CONFIGURE_MAXIMUM_REGIONS            100
#define CONFIGURE_MAXIMUM_PORTS              100
#define CONFIGURE_MAXIMUM_PERIODS            100
#define CONFIGURE_TICKS_PER_TIMESLICE        0

#define CONFIGURE_POSIX_INIT_THREAD_TABLE

#define CONFIGURE_MAXIMUM_POSIX_THREADS              10
#define CONFIGURE_MAXIMUM_POSIX_KEYS                 10

#include <rtems/confdefs.h>

/* end of include file */
