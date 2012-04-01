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

#include <linux/lightsensor.h>

#include <cutils/log.h>

#include "LightSensor.h"
/*****************************************************************************/
//构造函数，继承自SensorBase。设定传感器Kxsd9的各项默认参数。
//dev_name=LS_DEVICE_NAME；data_name="cm3602-light"；mEnabled=0；mInputReader=4；mHasPendingEvent=false
LightSensor::LightSensor()
    : SensorBase(LIGHT_DEVICE_NAME, "cm3602-light"), mEnabled(0), mInputReader(4), mHasPendingEvent(false){
    mPendingEvent.version = sizeof(sensors_event_t);
    //设定传感器类型ID
    mPendingEvent.sensor = ID_L;
    //设定传感器类型
    mPendingEvent.type = SENSOR_TYPE_LIGHT;
    //请求内存分配
    memset(mPendingEvent.data, 0, sizeof(mPendingEvent.data));

    mEnabled = isEnabled();
    if(mEnabled){
    	setInitialState();
    }
}

//析构函数
LightSensor::~LightSensor(){
}

int LightSensor::setInitialState(){
    struct input_absinfo absinfo;
    if (!ioctl(data_fd, EVIOCGABS(EVENT_TYPE_LIGHT), &absinfo)) {
        mPendingEvent.light = indexToValue(absinfo.value);
        mHasPendingEvent = true;
    }
    return 0;
}

//启用传感器
int LightSensor::enable(int32_t, int en){
	//
    int newState = en ? 1 : 0;
    int err = 0;
	//
    if (newState != mEnabled) {
    	//如果未启用，则打开设备
        if (!mEnabled) {
            open_device();
        }
        //启用设备，返回值为err
        int fd = open(LIGHT_ACTIVE_FILE, O_WRONLY);
        if(fd >= 0) {
            char buffer[20];
            int bytes = sprintf(buffer, "%d\n", newState);
            err = write(fd, buffer, bytes);
            err = err < 0 ? -errno : 0;
        } else {
            err = -errno;
        }
        LOGE_IF(err, "LIGHTSENSOR_ENABLE failed (%s)", strerror(-err));
        if (!err) {
            mEnabled = en ? 1 : 0;
            if (en) {
                setInitialState();
            }
        }
        if (!mEnabled) {
            close_device();
        }
    }
    return err;
}

bool LightSensor::hasPendingEvents() const {
    return mHasPendingEvent;
}

int LightSensor::readEvents(sensors_event_t* data, int count){
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
            if (event->code == REL_ADC) {
                if (event->value != -1) {
                    // FIXME: not sure why we're getting -1 sometimes
                    mPendingEvent.light = indexToValue(event->value);
                }
            }
        } else if (type == EV_SYN) {
            mPendingEvent.timestamp = timevalToNano(event->time);
            if (mEnabled) {
                *data++ = mPendingEvent;
                count--;
                numEventReceived++;
            }
        } else {
            LOGE("LightSensor: unknown event (type=%d, code=%d)",
                    type, event->code);
        }
        mInputReader.next();
    }

    return numEventReceived;
}

float LightSensor::indexToValue(size_t value) const{
    
    
    int index = (int)(value - 20) / (int)20;
    if (index < 0) 
    { index = 0; };
    static const float luxValues[10] = {
            10.0, 160.0, 225.0, 320.0, 640.0,
            1280.0, 2600.0, 5800.0, 8000.0, 10240.0
    };

    const int maxIndex = sizeof(luxValues)/sizeof(*luxValues) - 1;
    if (index > maxIndex)
        index = maxIndex;
    return luxValues[index];
}

int LightSensor::isEnabled(){
    int fd = open(LIGHT_WAKE_FILE, O_RDONLY);
    if (fd >= 0) {
        char buffer[20];
        int amt = read(fd, buffer, 20);
        close(fd);
        if(amt > 0) {
            return (buffer[0] == '1');
        } else {
            LOGE("Light Sensor: isEnable failed to read (%s)", strerror(errno));
            return 0;
        }
    } else {
        LOGE("Light Sensor: isEnabled failed to open %s", LIGHT_WAKE_FILE);
        return 0;
    }
}

