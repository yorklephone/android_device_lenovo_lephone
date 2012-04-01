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

#include <hardware/sensors.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <math.h>

#include <poll.h>
#include <pthread.h>

#include <linux/input.h>

#include <cutils/atomic.h>
#include <cutils/log.h>

#include "nusensors.h"
#include "LightSensor.h"
#include "ProximitySensor.h"
#include "Kxsd9Sensor.h"

/*****************************************************************************/
//结构体。定义各个传感器的属性
struct sensors_poll_context_t {
    struct sensors_poll_device_t device; // must be first

        sensors_poll_context_t();
        ~sensors_poll_context_t();
    int activate(int handle, int enabled);
    int setDelay(int handle, int64_t ns);
    int pollEvents(sensors_event_t* data, int count);

private:
    enum {
        light           = 0,
        proximity       = 1,
        kxsd9           = 2,
        numSensorDrivers,
        numFds,
    };

    static const size_t wake = numFds - 1;
    static const char WAKE_MESSAGE = 'W';
    struct pollfd mPollFds[numFds];
    int mWritePipeFd;
    SensorBase* mSensors[numSensorDrivers];

    //判断传感器类型并返回对应索引
    int handleToDriver(int handle) const {
        switch (handle) {
            case ID_A:
                return kxsd9;
            case ID_P:
                return proximity;
            case ID_L:
                return light;
        }
        return -EINVAL;
    }
};
/*****************************************************************************/

//构造函数。初始化各个传感器的属性
sensors_poll_context_t::sensors_poll_context_t(){
    mSensors[kxsd9] = new Kxsd9Sensor();
    mPollFds[kxsd9].fd = mSensors[kxsd9]->getFd();
    mPollFds[kxsd9].events = POLLIN;
    mPollFds[kxsd9].revents = 0;

    mSensors[light] = new LightSensor();
    mPollFds[light].fd = mSensors[light]->getFd();
    mPollFds[light].events = POLLIN;
    mPollFds[light].revents = 0;

    mSensors[proximity] = new ProximitySensor();
    mPollFds[proximity].fd = mSensors[proximity]->getFd();
    mPollFds[proximity].events = POLLIN;
    mPollFds[proximity].revents = 0;

    int wakeFds[2];
    int result = pipe(wakeFds);
    LOGE_IF(result<0, "error creating wake pipe (%s)", strerror(errno));
    fcntl(wakeFds[0], F_SETFL, O_NONBLOCK);
    fcntl(wakeFds[1], F_SETFL, O_NONBLOCK);
    mWritePipeFd = wakeFds[1];

    mPollFds[wake].fd = wakeFds[0];
    mPollFds[wake].events = POLLIN;
    mPollFds[wake].revents = 0;
}

//析构函数。关闭各个传感器，释放内存
sensors_poll_context_t::~sensors_poll_context_t(){
    for (int i=0 ; i<numSensorDrivers ; i++) {
        delete mSensors[i];
    }
    close(mPollFds[wake].fd);
    close(mWritePipeFd);
}

//激活传感器
int sensors_poll_context_t::activate(int handle, int enabled){
    //获取当前传感器类型索引
    int index = handleToDriver(handle);
    //判断获取到的的索引是合法，不在则直接返回该索引
    if (index < 0) return index;
    //使用当前传感器对应的enable函数来启用当前传感器
    int err =  mSensors[index]->enable(handle, enabled);
    //如果是要求不启用或启用不成功则提示发送唤醒消息失败
    if (enabled && !err) {
        const char wakeMessage(WAKE_MESSAGE);
        int result = write(mWritePipeFd, &wakeMessage, 1);
        LOGE_IF(result<0, "error sending wake message (%s)", strerror(errno));
    }
    //返回启用的操作结果
    return err;
}

//设定传感器的延迟值
int sensors_poll_context_t::setDelay(int handle, int64_t ns){

    //获取当前传感器类型索引
    int index = handleToDriver(handle);
    //判断获取到的的索引是合法，不在则直接返回该索引
    if (index < 0) return index;
    //使用当前传感器对应的setDelay函数来设置当前传感器的延迟
    return mSensors[index]->setDelay(handle, ns);
}

//
int sensors_poll_context_t::pollEvents(sensors_event_t* data, int count){
    int nbEvents = 0;
    int n = 0;

    do {
        // see if we have some leftover from the last poll()
        for (int i=0 ; count && i<numSensorDrivers ; i++) {
            SensorBase* const sensor(mSensors[i]);
            if ((mPollFds[i].revents & POLLIN) || (sensor->hasPendingEvents())) {
                int nb = sensor->readEvents(data, count);
                if (nb < count) {
                    // no more data for this sensor
                    mPollFds[i].revents = 0;
                }
                count -= nb;
                nbEvents += nb;
                data += nb;
            }
        }

        if (count) {
            // we still have some room, so try to see if we can get
            // some events immediately or just wait if we don't have
            // anything to return
            n = poll(mPollFds, numFds, nbEvents ? 0 : -1);
            if (n<0) {
                LOGE("poll() failed (%s)", strerror(errno));
                return -errno;
            }
            if (mPollFds[wake].revents & POLLIN) {
                char msg;
                int result = read(mPollFds[wake].fd, &msg, 1);
                LOGE_IF(result<0, "error reading from wake pipe (%s)", strerror(errno));
                LOGE_IF(msg != WAKE_MESSAGE, "unknown message on wake queue (0x%02x)", int(msg));
                mPollFds[wake].revents = 0;
            }
        }
        // if we have events and space, go read them
    } while (n && count);

    return nbEvents;
}

/*******************************以下为对外的接口*******************************/
//接口：用于关闭传感器
static int poll__close(struct hw_device_t *dev){
    sensors_poll_context_t *ctx = (sensors_poll_context_t *)dev;
    if (ctx) {
        delete ctx;
    }
    return 0;
}

//接口：用于激活传感器
static int poll__activate(struct sensors_poll_device_t *dev, int handle, int enabled){
    sensors_poll_context_t *ctx = (sensors_poll_context_t *)dev;
    return ctx->activate(handle, enabled);
}

//接口：用于设定传感器的延迟值
static int poll__setDelay(struct sensors_poll_device_t *dev, int handle, int64_t ns){
    sensors_poll_context_t *ctx = (sensors_poll_context_t *)dev;
    return ctx->setDelay(handle, ns);
}

//
static int poll__poll(struct sensors_poll_device_t *dev, sensors_event_t* data, int count){
    sensors_poll_context_t *ctx = (sensors_poll_context_t *)dev;

    return ctx->pollEvents(data, count);
}
/*****************************************************************************/
//类初始化
int init_nusensors(hw_module_t const* module, hw_device_t** device){
    int status = -EINVAL;

    sensors_poll_context_t *dev = new sensors_poll_context_t();
    memset(&dev->device, 0, sizeof(sensors_poll_device_t));

    dev->device.common.tag = HARDWARE_DEVICE_TAG;
    dev->device.common.version  = 0;
    dev->device.common.module   = const_cast<hw_module_t*>(module);
    dev->device.common.close    = poll__close;
    dev->device.activate        = poll__activate;
    dev->device.setDelay        = poll__setDelay;
    dev->device.poll            = poll__poll;

    *device = &dev->device.common;
    status = 0;
    return status;
}
