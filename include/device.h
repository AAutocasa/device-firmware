#ifndef __DEVICE_H__
#define __DEVICE_H__

#define RoleChangeCallback std::function<void(CapabilityRoleCode)>

enum DeviceStatus {
    Inactive = 0,
    Active = 1
};
  
enum CapabilityCode {
    RGB = 0,
};

enum CapabilityRoleCode {
    RGB_STRIPE = 0,
    RGB_MATRIX = 1,
};

typedef struct Capability {
    CapabilityCode code;
    CapabilityRoleCode activeRole;
    String version;
} Capability;

void setupDevice(const char *deviceName, const char *deviceType, const DeviceStatus deviceStatus);

void addCapability(Capability capability, RoleChangeCallback callback);

#endif