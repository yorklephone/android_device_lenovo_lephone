# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

#
# This is the product configuration for a generic GSM 3GW100,
# not specialized for any geography.
#

## (1) First, the most specific values, i.e. the aspects that are specific to GSM

PRODUCT_COPY_FILES := \
    device/lenovo/3GC101/init.3GC101.rc:root/init.3GC101.rc \
    device/lenovo/3GC101/init.rc:root/init.rc \
    device/lenovo/3GC101/initlogo.rle:root/initlogo.rle \
    device/lenovo/3GC101/ueventd.3GC101.rc:root/ueventd.3GC101.rc \
    device/lenovo/3GC101/ueventd.rc:root/ueventd.rc
    
PRODUCT_PROPERTY_OVERRIDES := \
    ro.sf.lcd_density=160 \

# Default network type.
# 0 => WCDMA preferred.
PRODUCT_PROPERTY_OVERRIDES += \
    ro.telephony.default_network=7

# The OpenGL ES API level that is natively supported by this device.
# This is a 16.16 fixed point number
PRODUCT_PROPERTY_OVERRIDES += \
    ro.opengles.version=131072

# This is a high density device with more memory, so larger vm heaps for it.
PRODUCT_PROPERTY_OVERRIDES += \
    dalvik.vm.heapsize=32m

PRODUCT_PROPERTY_OVERRIDES += \
    wifi.interface=wlan0 \
    wifi.supplicant_scan_interval=15 \
    ro.com.android.dataroaming=false

PRODUCT_PACKAGES := \
    gps.3GC101 \
    lights.3GC101 \
    sensors.3GC101 \
    gralloc.3GC101 \
    PinyinIME \
    screencap \
    librs_jni \
    copybit.3GC101 \
    libOmxCore \
    libOmxVdec \
    libOmxVidEnc \
    libmm-omxcore \
    libcyanogen-dsp \
    com.android.future.usb.accessory \
    rzscontrol
    
#ifeq ($(TARGET_PREBUILT_KERNEL),)
#LOCAL_KERNEL := vendor/lenovo/3GC101/proprietary/kernel
#else
#LOCAL_KERNEL := $(TARGET_PREBUILT_KERNEL)
#endif

#PRODUCT_COPY_FILES += \
#    $(LOCAL_KERNEL):kernel
        
## (2) Also get non-open-source GSM-specific aspects if available
$(call inherit-product-if-exists, vendor/lenovo/3GC101/3GC101-vendor.mk)

## (3)  Finally, the least specific parts, i.e. the non-GSM-specific aspects
$(call inherit-product, device/lenovo/common/common.mk)

PRODUCT_NAME := common_3GC101
PRODUCT_DEVICE := 3GC101
PRODUCT_MODEL := 3GC101
PRODUCT_BRAND := Lenovo
PRODUCT_MANUFACTURER := LENOVO
PRODUCT_OPERATOR := chinatel
PRODUCT_OPERATOR_NUMERIC := 460003
PRODUCT_3G_NETWORK := evdo
PRODUCT_TELEPHONY := cdma

