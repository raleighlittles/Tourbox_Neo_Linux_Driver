#include <cstdint>
#include <fstream>
#include <iostream>
#include <unistd.h>

#include <libusb-1.0/libusb.h>


// These constants were used in this example: https://github.com/tytouf/libusb-cdc-example/blob/master/cdc_example.c
// And were present in older versions of Linux https://elixir.bootlin.com/linux/v4.1/source/drivers/usb/class/cdc-acm.h#L34
// but are not currently present in Linux anymore for some reason
#define ACM_CTRL_DTR		0x01
#define ACM_CTRL_RTS		0x02

int main(int /* argc */, char**  /* argv[] */) {

    const uint16_t vendorId = 0xc251;
    const uint16_t productId = 0x2005;

    libusb_context* lusb_context = nullptr;

    int rc = libusb_init(&lusb_context);

    libusb_set_option(lusb_context, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_DEBUG);

    if (rc != libusb_error::LIBUSB_SUCCESS) {
        std::cerr << "ERROR: Failed to initialize libusb, libusb error# = " << rc << std::endl;
        return -1;
    }

    libusb_device_handle* lusb_dev_hndl = libusb_open_device_with_vid_pid(lusb_context, vendorId, productId);

    if (!lusb_dev_hndl) {
        std::cerr << "ERROR: libusb device handler null"  << std::endl;
        return -1;
    }

    // Device only has ___ USB interfaces (see `lsusb` output) TODO is this still true?
    const int numInterfaces = 3;

    for (int interfaceNum = 0; interfaceNum < numInterfaces; interfaceNum++) {
        // Can't claim the device if the operating system is using it
        if (libusb_kernel_driver_active(lusb_dev_hndl, interfaceNum)) {
            if (libusb_detach_kernel_driver(lusb_dev_hndl, interfaceNum) != libusb_error::LIBUSB_SUCCESS) {
                std::cerr << "ERROR: Failed to detach kernel driver for device on interface num " << interfaceNum << std::endl;
                //return -1;
            }

            else {
                if (libusb_claim_interface(lusb_dev_hndl, interfaceNum) != libusb_error::LIBUSB_SUCCESS) {
                    std::cerr << "ERROR: Couldn't claim interface num " << interfaceNum << std::endl;
                    // return -1;
                }

            }
        }
    }

    // Wireshark packet No. 3177
    rc = libusb_set_configuration(lusb_dev_hndl, 1);

    if (rc != libusb_error::LIBUSB_SUCCESS) {
        std::cerr << "Error issuing SET CONFIGURATION request, libusb error# " << rc << " | " << libusb_strerror(rc) << std::endl;
        return -1;
    }

    // https://stackoverflow.com/questions/30425918/aoa-2-0-hid-device-unsupported-control-request-on-sending-hid-event ?
    usleep(100000);

    // Wireshark packet No. 3225
    rc = libusb_control_transfer(lusb_dev_hndl, /* bmRequestType */ 0x21, /* 'Request Code' */ 0x22, /* Value */ ACM_CTRL_DTR | ACM_CTRL_RTS, /* Index */ 0, nullptr, /* Length */ 0, 0);

    if (rc < libusb_error::LIBUSB_SUCCESS) {
        std::cerr << "Error issuing SET CONTROL LINE STATE request, libusb err# " << rc << " | " << libusb_strerror(rc) << std::endl;
        //return -1;
    }

    usleep(100000);
    
    // 'Data Fragment' in Wireshark
    // Their capture uses different encoding data than mine
    // unsigned char encoding[] = { 0x80, 0x25, 0x00, 0x00, 0x00, 0x00, 0x08 }; // <-- Mine
    unsigned char encoding[] = {0x00, 0xC2, 0x01, 0x00, 0x00, 0x00, 0x08};
    
    // Wireshark packet No. 3227
    rc = libusb_control_transfer(lusb_dev_hndl, /* bmRequestType */ 0x21, /* Request Code */ 0x20, /* Value */ 0, /* Index */ 0, encoding, sizeof(encoding), 0);

        if (rc < libusb_error::LIBUSB_SUCCESS) {
        std::cerr << "Error issuing SET LINE CODING Request, libusb err# " << rc << " | " << libusb_strerror(rc) << std::endl;
        //return -1;
    }

    usleep(100000);

    // Packet #113 (theirs)
    unsigned char data_1[] = { 0x55 , 0x00 , 0x07 , 0x88 , 0x94 , 0x00 , 0x1a , 0xfe };

    rc = libusb_bulk_transfer(lusb_dev_hndl, 0x02 /* Out */, data_1, sizeof(data_1), nullptr, 0);

    if (rc < libusb_error::LIBUSB_SUCCESS) {
        std::cerr << "Error with USB BULK OUT, libusb err# " << rc << " | " << libusb_strerror(rc) << std::endl;
        //return -1;
    }

    usleep(100000);

    // Packet #117 (theirs)
    unsigned char data_2[] = { 0xb5, 0x00, 0x5d, 0x04, 0x08, 0x05, 0x08, 0x06, 0x08, 0x07, 0x08, 0x08, 0x08, 0x09, 0x08, 0x0b, 0x08, 0x0c, 0x08, 0x0d, 0x08, 0x0e, 0x08, 0x0f, 0x08, 0x26, 0x08, 0x27, 0x08, 0x28, 0x08, 0x29, 0x08, 0x3b, 0x08, 0x3c, 0x08, 0x3d, 0x08, 0x3e, 0x08, 0x3f, 0x08, 0x40, 0x08, 0x41, 0x08, 0x42, 0x08, 0x43, 0x08, 0x44, 0x08, 0x45, 0x08, 0x46, 0x08, 0x47, 0x08, 0x48, 0x08, 0x49, 0x08, 0x4a, 0x08, 0x4b, 0x08, 0x4c, 0x08, 0x4d, 0x08, 0x4e, 0x08, 0x4f, 0x08, 0x50, 0x08, 0x51, 0x08, 0x52, 0x08, 0x53, 0x08, 0x54, 0x08, 0xa8, 0x08, 0xa9, 0x08, 0xaa, 0x08, 0xab, 0x08, 0xfe };

    rc = libusb_bulk_transfer(lusb_dev_hndl, 0x02 /* Out */, data_2, sizeof(data_2), nullptr, 0);

    // Return device functionality back to the OS
    // rc = libusb_attach_kernel_driver(lusb_dev_hndl, interfaceNum);

    // if (rc < libusb_error::LIBUSB_SUCCESS) {
    //     std::cerr << "Error re-attaching kernel driver, libusb err# " << rc << " | " << libusb_strerror(rc) << std::endl;
    // }

    return 0;
}