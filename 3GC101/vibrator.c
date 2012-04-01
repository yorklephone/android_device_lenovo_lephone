/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <hardware_legacy/vibrator.h>
#include "qemu.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

/***RK_ID:RK_VIBRATOR. DEP_RK_ID:NULL. AUT:xieyan@gmail.com DATE:2010-07-21. START***/
#include <pthread.h>

//#define THE_DEVICE "/sys/class/timed_output/vibrator/enable"
#define THE_DEVICE "/sys/module/vibrator_ctrl/parameters/active_vibrator"
/***RK_ID:RK_VIBRATOR. DEP_RK_ID:NULL. AUT:xieyan@gmail.com DATE:2010-07-21. END***/

static int sendit(int timeout_ms)
{
    int nwr, ret, fd;
    char value[20];
/***RK_ID:RK_VIBRATOR. DEP_RK_ID:NULL. AUT:xieyan@gmail.com DATE:2010-07-21. START***/
	int enable;
/***RK_ID:RK_VIBRATOR. DEP_RK_ID:NULL. AUT:xieyan@gmail.com DATE:2010-07-21. END***/

#ifdef QEMU_HARDWARE
    if (qemu_check()) {
        return qemu_control_command( "vibrator:%d", timeout_ms );
    }
#endif

/***RK_ID:RK_VIBRATOR. DEP_RK_ID:NULL. AUT:xieyan@gmail.com DATE:2010-07-21. START***/
    fd = open(THE_DEVICE, O_RDWR);
    if(fd < 0)
        return errno;

    if ( timeout_ms > 0)
        enable = 1;
    else
        enable = 0;

    nwr = sprintf(value, "%d\n", enable);
//    nwr = sprintf(value, "%d\n", timeout_ms);
/***RK_ID:RK_VIBRATOR. DEP_RK_ID:NULL. AUT:xieyan@gmail.com DATE:2010-07-21. END***/

    ret = write(fd, value, nwr);

    close(fd);

    return (ret == nwr) ? 0 : -1;
}


/***RK_ID:RK_VIBRATOR. DEP_RK_ID:NULL. AUT:xieyan@gmail.com DATE:2010-07-21. START***/
#if 1
void my_alarm_handler(int a)
{
    sendit(0);
}

static void *vibrator_pthread_function(void *arg)
{
   struct timeval scanTimer;
   int timeout_ms = (int)arg;
   
   scanTimer.tv_sec = timeout_ms/1000;
   scanTimer.tv_usec = (timeout_ms%1000)*1000;       
   select( 0, NULL, NULL, NULL, &scanTimer );

   //pthread_mutex_lock(&g_lock);
   my_alarm_handler(NULL);
   ///pthread_mutex_unlock(&g_lock);

   pthread_exit(NULL);
   return 0;   
}

int vibrator_on(int timeout_ms)
{
    int result = 0;
    pthread_t vibrator_thread=0;
 
    /* constant on, up to maximum allowed time */
    if (sendit(timeout_ms) < 0)
        return -1;

    result = pthread_create( &vibrator_thread, NULL, vibrator_pthread_function, (void *)timeout_ms);
    pthread_detach(vibrator_thread);

    return 0;
}

int vibrator_off(void)
{
    if (sendit(0) < 0)
        return -1;

    return 0;
}
#else
int vibrator_on(int timeout_ms)
{
    /* constant on, up to maximum allowed time */
    return sendit(timeout_ms);
}

int vibrator_off()
{
    return sendit(0);
}
#endif
/***RK_ID:RK_VIBRATOR. DEP_RK_ID:NULL. AUT:xieyan@gmail.com DATE:2010-07-21. END***/
