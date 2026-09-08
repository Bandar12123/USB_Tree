#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

#define MAX_USB_DEPTH 7
#define CLASS_MASS_STORAGE 0x08

typedef struct {
    uint8_t bus;
    uint8_t address;
    uint8_t path[MAX_USB_DEPTH];
    int port_count;
    uint16_t vendor_id;
    uint16_t product_id;
    int is_flash_drive;
} USBDevice;

void parse_device_layout(libusb_device *dev, USBDevice *device) {
    struct libusb_device_descriptor desc;
    libusb_get_device_descriptor (dev, &desc);
    
    device->bus = libusb_get_bus_number(dev);
    device->address = libusb_get_device_address(dev);
    device->vendor_id = desc.idVendor;
    device->product_id = desc.idProduct;

    device->is_flash_drive = (desc.bDeviceClass == CLASS_MASS_STORAGE);

    device->port_count = libusb_get_port_numbers(dev, device->path, MAX_USB_DEPTH);

}

void print_device_info(const USBDevice *device) {
    printf("Bus %03d: Device %03d | Path: Root", device->bus, device->address);
    for (int i = 0; i < device->port_count; i++) {
        printf(" -> Port %d", device->path[i]);
    }
    printf(" | ID %04x:%04x", device->vendor_id, device->product_id);

    if (device->is_flash_drive) {
        printf(" [ Flash Drive]");
    }
    printf("\n");
}

int main() {
    libusb_context *ctx = NULL;
    libusb_device **device_list;

    if (libusb_init(&ctx) < 0)return 1;

    ssize_t count = libusb_get_device_list(ctx, &device_list);
    if (count < 0) {
        libusb_exit(ctx);
        return 1;
}

USBDevice *my_devices = malloc(sizeof(USBDevice) * count);

for (ssize_t i = 0; i < count; i++) {
        parse_device_layout(device_list[i], &my_devices[i]);
}

printf("--- Displaying Saved USB Tree Structure ---\n");
for (size_t i = 0; i < count; i++) {
    print_device_info(&my_devices[i]);
}

free(my_devices);
libusb_free_device_list(device_list, 1);
libusb_exit(ctx);
return 0;

}