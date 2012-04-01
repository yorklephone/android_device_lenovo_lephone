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

#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <poll.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/select.h>

#include <linux/capella_cm3602.h>

#include <cutils/log.h>

#include "ProximitySensor.h"
/*****************************************************************************/
ProximitySensor::ProximitySensor()
    : SensorBase(PROXIMITY_DEVICE_NAME, "cm3602-proximity"), mEnabled(0), mInputReader(4), mHasPendingEvent(false){
    mPendingEvent.version = sizeof(sensors_event_t);
    mPendingEvent.sensor = ID_P;
    mPendingEvent.type = SENSOR_TYPE_PROXIMITY;
    memset(mPendingEvent.data, 0, sizeof(mPendingEvent.data));

    mEnabled = isEnabled();
    if(mEnabled){
    	setInitialState();
    }
}

ProximitySensor::~ProximitySensor() {
}

int ProximitySensor::setInitialState() {
    struct input_absinfo absinfo;
    if (!ioctl(data_fd, EVIOCGABS(EVENT_TYPE_PROXIMITY), &absinfo)) {
        // make sure to report an event immediately
        mHasPendingEvent = true;
        mPendingEvent.distance = absinfo.value;
    }
    return 0;
}

int ProximitySensor::enable(int32_t, int en) {
    int newState = en ? 1 : 0;
    int err = 0;

    // don't set enable state if it's already valid
    if(mEnabled == newState) {
        return err;
    }

    // ok we need to set our enabled state
    int fd = open(PROXIMITY_ACTIVE_FILE, O_WRONLY);
    if(fd >= 0) {
        char buffer[20];
        int bytes = sprintf(buffer, "%d\n", newState);
        err = write(fd, buffer, bytes);
        err = err < 0 ? -errno : 0;
    } else {
        err = -errno;
    }

    LOGE_IF(err < 0, "Error setting enable of ProximitySensor (%s)", strerror(-err));

    if (!err) {
        mEnabled = newState;
    }

    return err;
}

bool ProximitySensor::hasPendingEvents() const {
    return mHasPendingEvent;
}

int ProximitySensor::readEvents(sensors_event_t* data, int count){
    if (count < 1)
        return -EINVAL;

    if (mHasPendingEvent) {
        mHasPendingEvent = false;
        mPendingEvent.timestamp = getTimestamp();
        *data = mPendingEvent;
        return mEnabled ? 1 : 0;
    }

    ssize_t n = mInputReader.fill(data_fd);
    if (n < 0)
        return n;

    int numEventReceived = 0;
    input_event const* event;

    while (count && mInputReader.readEvent(&event)) {
        int type = event->type;
        if (type == EV_REL) {
            if (event->code == REL_P_INT) {
                mPendingEvent.distance = event->value * 3.5;
            }
        } else if (type == EV_SYN) {
            mPendingEvent.timestamp = timevalToNano(event->time);
            if (mEnabled) {
                *data++ = mPendingEvent;
                count--;
                numEventReceived++;
            }
        } else {
            LOGE("ProximitySensor: unknown event (type=%d, code=%d)", type, event->code);
        }
        mInputReader.next();
    }

    return numEventReceived;
}

int ProximitySensor::isEnabled(){
    int fd = open(PROXIMITY_WAKE_FILE, O_RDONLY);
    if (fd >= 0) {
        char buffer[20];
        int amt = read(fd, buffer, 20);
        close(fd);
        if(amt > 0) {
            return (buffer[0] == '1');
        } else {
            LOGE("Proximity Sensor: isEnable failed to read (%s)", strerror(errno));
            return 0;
        }
    } else {
        LOGE("Proximity Sensor: isEnabled failed to open %s", PROXIMITY_WAKE_FILE);
        return 0;
    }
}
