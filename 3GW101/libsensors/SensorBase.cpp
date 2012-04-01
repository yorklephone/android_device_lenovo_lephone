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

#include <cutils/log.h>

#include <linux/input.h>

#include "SensorBase.h"

/*****************************************************************************/
//构造函数，dev_fd和data_fd默认为-1
SensorBase::SensorBase( const char* dev_name, const char* data_name)
    : dev_name(dev_name), data_name(data_name), dev_fd(-1), data_fd(-1){
    data_fd = openInput(data_name);
}

//析构函数，关闭设备，释放内存
SensorBase::~SensorBase() {
    if (data_fd >= 0) {
        close(data_fd);
    }
    if (dev_fd >= 0) {
        close(dev_fd);
    }
}

//打开传感器设备
int SensorBase::open_device() {
    //如果dev_fd仍为初始值(-1<0)，同时dev_name不为空则获取设备的句柄
    if (dev_fd<0 && dev_name) {
    	//以只读方式(O_RDONLY)打开传感器设备。成功打开则返回文件描述符fd，否则返回-1
        dev_fd = open(dev_name, O_RDONLY);
        //如果设备打开失败，则输出error日志
        LOGE_IF(dev_fd<0, "Couldn't open %s (%s)", dev_name, strerror(errno));
    }
    //总是返回操作成功
    return 0;
}

//关闭传感器设备
int SensorBase::close_device() {
    //如果dev_fd不为负值，认为设备目前是打开状态，则执行关闭操作
    if (dev_fd >= 0) {
    	//关闭传感器设备
        close(dev_fd);
        //重新设置为初始值
        dev_fd = -1;
    }
    //总是返回操作成功
    return 0;
}

//fd:file descriptor。文件描述符
int SensorBase::getFd() const {
	//返回当前设备的data_fd
    return data_fd;
}

int SensorBase::setDelay(int32_t handle, int64_t ns) {
    return 0;
}

bool SensorBase::hasPendingEvents() const {
    return false;
}

int64_t SensorBase::getTimestamp() {
    struct timespec t;
    t.tv_sec = t.tv_nsec = 0;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return int64_t(t.tv_sec)*1000000000LL + t.tv_nsec;
}

//打开输入设备
int SensorBase::openInput(const char* inputName) {
    int fd = -1;
    const char *dirname = "/dev/input";
    char devname[PATH_MAX];
    char *filename;
    DIR *dir;
    struct dirent *de;
    dir = opendir(dirname);
    if(dir == NULL)
        return -1;
    strcpy(devname, dirname);
    filename = devname + strlen(devname);
    *filename++ = '/';
    /*********************************************************************/
    //循环读出指定目录下的设备名
    while((de = readdir(dir))) {
        if(de->d_name[0] == '.' && (de->d_name[1] == '\0' || (de->d_name[1] == '.' && de->d_name[2] == '\0')))
            continue;
        strcpy(filename, de->d_name);
        fd = open(devname, O_RDONLY);
        if (fd>=0) {
            char name[80];
            if (ioctl(fd, EVIOCGNAME(sizeof(name) - 1), &name) < 1) {
                name[0] = '\0';
            }
            if (!strcmp(name, inputName)) {
                break;
            } else {
                close(fd);
                fd = -1;
            }
        }
    }
    /************************************************************************/
    closedir(dir);
    LOGE_IF(fd<0, "couldn't find '%s' input device", inputName);
    return fd;
}
