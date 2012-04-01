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

# config.mk
#
# Product-specific compile-time definitions.
#

TARGET_NO_RECOVERY := true
TARGET_NO_BOOTLOADER := true
USE_CAMERA_STUB := false
TARGET_BOARD_PLATFORM := qsd8k
TARGET_BOARD_PLATFORM_GPU := qcom-adreno200
CC_WITH_GOOGLE := true
TARGET_CPU_ABI := armeabi-v7a
TARGET_CPU_ABI2 := armeabi
TARGET_ARCH_VARIANT := armv7-a-neon
ARCH_ARM_HAVE_TLS_REGISTER := true
BOARD_USE_SCREENCAP := true
BOARD_USES_GENERIC_AUDIO := false
TARGET_PROVIDES_LIBAUDIO := true
BOARD_HAS_NO_MISC_PARTITION := true
# Wifi related defines
BOARD_WPA_SUPPLICANT_DRIVER := WEXT
WPA_SUPPLICANT_VERSION      := VER_0_6_X
WIFI_DRIVER_MODULE_PATH     := /system/wifi/ar6000.ko
WIFI_DRIVER_MODULE_NAME     := ar6000
BOARD_OVERLAY_FORMAT_YCrCb_420_SP := true
TARGET_USES_2G_VM_SPLIT := true

BOARD_USES_GENERIC_AUDIO := false

TARGET_USES_2G_VM_SPLIT := true

BOARD_KERNEL_BASE := 0x20000000
BOARD_KERNEL_PAGESIZE := 4096
BOARD_CAMERA_USE_GETBUFFERINFO := true
BOARD_HAVE_BLUETOOTH := true
BOARD_USE_CAF_LIBCAMERA := true
BOARD_USE_FFC_CAMERA := true
TARGET_GRALLOC_USES_QCOM_PMEM := true
BOARD_USES_QCOM_HARDWARE := true
BOARD_USES_QCOM_LIBS := true
BOARD_USE_QCOM_PMEM := true
BOARD_USE_REVERSE_FFC := true
BOARD_USES_QCOM_GPS := true
BOARD_GPS_LIBRARIES := libloc libloc_api
BOARD_USES_GPSSHIM := true
BOARD_GPS_NEEDS_XTRA := true
#TARGET_PROVIDES_LIBRIL := true
TARGET_IS_LEPHONE := true
#TARGET_LIBAGL_USE_GRALLOC_COPYBITS := true
BOARD_EGL_CFG := device/lenovo/common/egl.cfg
BOARD_NO_RGBX_8888 := true
# # cat /proc/mtd
# dev:    size   erasesize  name
# mtd0: 00500000 00020000 "boot"
# mtd1: 09100000 00020000 "system"
# mtd2: 13580000 00020000 "userdata"
# mtd3: 00080000 00020000 "fota_flag"
# mtd4: 00100000 00020000 "fota_backup"
# mtd5: 00080000 00020000 "fota_ua"
# mtd6: 00a20000 00020000 "fota"
# mtd7: 01140000 00020000 "amss"
BOARD_BOOTIMAGE_PARTITION_SIZE     := 0x00500000
BOARD_SYSTEMIMAGE_PARTITION_SIZE   := 0x09100000   # limited so we enforce room to grow
BOARD_USERDATAIMAGE_PARTITION_SIZE := 0x13580000
BOARD_FLASH_BLOCK_SIZE             := 131072
#WITH_DEXPREOPT := true
TARGET_RELEASETOOLS_EXTENSIONS := device/lenovo/common

