
# Uncomment this if you're using STL in your project
# See CPLUSPLUS-SUPPORT.html in the NDK documentation for more information
# APP_STL := stlport_static 

APP_ABI := armeabi armeabi-v7a

APP_CFLAGS  += -D__ANDROID_LOG_OFF__

APP_CPPFLAGS += -frtti -std=c++11 -fexceptions

APP_PLATFORM := android-15
APP_STL := gnustl_static 
