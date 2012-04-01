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

PRODUCT_PROPERTY_OVERRIDES :=
    ro.media.dec.jpeg.memcap=20000000

PRODUCT_COPY_FILES := \
    frameworks/base/data/etc/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml \
    frameworks/base/data/etc/android.hardware.telephony.gsm.xml:system/etc/permissions/android.hardware.telephony.gsm.xml \
    frameworks/base/data/etc/android.hardware.telephony.cdma.xml:system/etc/permissions/android.hardware.telephony.cdma.xml \
    frameworks/base/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
    frameworks/base/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
    frameworks/base/data/etc/android.hardware.sensor.proximity.xml:system/etc/permissions/android.hardware.sensor.proximity.xml \
    frameworks/base/data/etc/android.hardware.sensor.light.xml:system/etc/permissions/android.hardware.sensor.light.xml \
    frameworks/base/data/etc/android.software.sip.voip.xml:system/etc/permissions/android.software.sip.voip.xml \
    frameworks/base/data/etc/android.hardware.touchscreen.multitouch.distinct.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.distinct.xml 

# media config xml file
PRODUCT_COPY_FILES += \
    device/lenovo/common/media_profiles.xml:system/etc/media_profiles.xml \
    device/lenovo/common/10hostapconf:system/etc/init.d/10hostapconf

PRODUCT_PACKAGES := \
    librs_jni

# we have enough storage space to hold precise GC data
PRODUCT_TAGS += dalvik.gc.type-precise

# Passion uses high-density artwork where available
PRODUCT_LOCALES := hdpi

PRODUCT_COPY_FILES += \
    device/lenovo/common/wpa_supplicant.conf:system/etc/wifi/wpa_supplicant.conf\
    device/lenovo/common/surf_keypad.kl:system/usr/keylayout/surf_keypad.kl \
    device/lenovo/common/AVRCP.kl:system/usr/keylayout/AVRCP.kl \
    device/lenovo/common/docking_event.kl:system/usr/keylayout/docking_event.kl \
    device/lenovo/common/8k_handset.kl:system/usr/keylayout/8k_handset.kl \
    device/lenovo/common/docking_event.kcm.bin:system/usr/keychars/docking_event.kcm.bin \
    device/lenovo/common/surf_keypad.kcm.bin:system/usr/keychars/surf_keypad.kcm.bin \
    device/lenovo/common/vold.fstab:system/etc/vold.fstab



$(call inherit-product-if-exists, vendor/lenovo/common/common-vendor.mk)


