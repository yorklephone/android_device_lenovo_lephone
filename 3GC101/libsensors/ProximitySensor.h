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

#ifndef ANDROID_PROXIMITY_SENSOR_H
#define ANDROID_PROXIMITY_SENSOR_H

#include <stdint.h>
#include <errno.h>
#include <sys/cdefs.h>
#include <sys/types.h>

#include "nusensors.h"
#include "SensorBase.h"
#include "InputEventReader.h"

#define PROXIMITY_DEVICE_NAME  "/dev/input/event4"
#define PROXIMITY_ACTIVE_FILE  "/sys/devices/platform/isl29028.0/active_proximity_sensor"
#define PROXIMITY_WAKE_FILE    "/sys/devices/platform/isl29028.0/proximity_sensor_wake"
#define REL_P_INT		0x0b
/*****************************************************************************/
struct input_event;

class ProximitySensor : public SensorBase {
public:
            ProximitySensor();
    virtual ~ProximitySensor();
    virtual int readEvents(sensors_event_t* data, int count);
    virtual bool hasPendingEvents() const;
    virtual int enable(int32_t handle, int enabled);

private:
    int mEnabled;
    InputEventCircularReader mInputReader;
    sensors_event_t mPendingEvent;
    bool mHasPendingEvent;

    int setInitialState();
    int isEnabled();
};

/*****************************************************************************/

#endif  // ANDROID_PROXIMITY_SENSOR_H
