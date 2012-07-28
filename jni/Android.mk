LOCAL_PATH:= $(call my-dir)

ANDROID_ROOT := /mnt/data/android
CFLAGS := -Wpointer-arith -Wwrite-strings -Wunused -Winline -Wnested-externs -Wmissing-declarations -Wmissing-prototypes -Wno-long-long -Wfloat-equal -Wno-multichar -Wsign-compare -Wno-format-nonliteral -Wendif-labels -Wstrict-prototypes -Wdeclaration-after-statement -Wno-system-headers -DHAVE_CONFIG_H

include $(CLEAR_VARS)
include $(LOCAL_PATH)/curl/lib/Makefile.inc
LOCAL_MODULE := curl
LOCAL_SRC_FILES := $(addprefix curl/lib/,$(CSOURCES))
LOCAL_C_INCLUDES += $(LOCAL_PATH)/curl/include $(LOCAL_PATH)/curl/lib $(ANDROID_ROOT)/external/openssl/include
LOCAL_CFLAGS += $(CFLAGS) -DUSE_OPENSSL
LOCAL_LDFLAGS := -lssl -lcrypto -lz -L$(ANDROID_ROOT)/out/target/product/generic/system/lib
# LOCAL_COPY_HEADERS_TO := libcurl
# LOCAL_COPY_HEADERS := $(addprefix curl/include/curl/,$(HHEADERS))
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := http
LOCAL_SRC_FILES := http.c
LOCAL_STATIC_LIBRARIES := curl
LOCAL_C_INCLUDES += $(LOCAL_PATH)/curl/include $(LOCAL_PATH)/curl/lib
LOCAL_LDFLAGS += -llog -lssl -lcrypto -lz -L$(ANDROID_ROOT)/out/target/product/generic/system/lib
include $(BUILD_SHARED_LIBRARY)
