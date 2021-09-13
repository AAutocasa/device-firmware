#ifndef __DEVICE_H__
#define __DEVICE_H__

enum DeviceStatus {
    Inactive = 0,
    Active = 1
};

void setupDevice(const char *deviceName, const char *deviceType, const DeviceStatus deviceStatus);

#endif