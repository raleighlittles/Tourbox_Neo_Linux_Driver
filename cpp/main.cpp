/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-20
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include <array>
#include <string>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <termios.h>
#include <unistd.h>
#include <iomanip>

#include <linux/uinput.h>

int main(int /* argc */, char** /* argv[] */ )
{

    /// ---------- ///
    /// Setup and open a serial port ///

    const std::string serialPortFile = "/dev/ttyUSB0";

    const int serialPortFileDescriptor = open(serialPortFile.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);

    if (serialPortFileDescriptor == -1) {
        std::cerr << "Failed to open serial port file " << serialPortFile << std::endl;
        return 1;
    }

    struct termios term_options;
    memset(&term_options, 0, sizeof(struct termios));

    term_options.c_cflag = B115200 | CS8 | CREAD;

    if ((tcsetattr(serialPortFileDescriptor, TCSANOW, &term_options)) != 0) {

        std::cerr << "Failed to set termios settings";
        close(serialPortFileDescriptor);
        return 1;
    }

    if ((tcflush(serialPortFileDescriptor, TCIOFLUSH)) != 0) {

        std::cerr << "Failed to flush termios settings";
        close(serialPortFileDescriptor);
        return 1;
    }

    // Wait for the serial port to open -- this might not be needed
    usleep(100000);

    std::array<uint8_t, 1> readBuffer;

    /// ---------- ///
    /// Setup the virtual driver. (See example.c)

    struct uinput_setup usetup;

   int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);


   /*
    * The ioctls below will enable the device that is about to be
    * created, to pass key events, in this case the space key.
    */
   ioctl(fd, UI_SET_EVBIT, EV_KEY);
   ioctl(fd, UI_SET_KEYBIT, KEY_SPACE);

   memset(&usetup, 0, sizeof(usetup));
   usetup.id.bustype = BUS_USB;
   usetup.id.vendor = 0x0483; /* Same vendor as default device */
   usetup.id.product = 0xBEEF; /* Bogus product ID */
   strcpy(usetup.name, "Custom Tourbox TBG_H Driver");

   ioctl(fd, UI_DEV_SETUP, &usetup);
   ioctl(fd, UI_DEV_CREATE);

   sleep (1);

    while (true) {

        ssize_t bytesRead = read(serialPortFileDescriptor, readBuffer.begin(), 1);

        if (bytesRead < 0) {
            std::cerr << "Error reading from serial port" << std::endl;
            return 2;
        }

        if (bytesRead > 0) {

            std::cout << std::hex << std::setfill('0') << std::setw(2) << readBuffer[0] << " " << readBuffer[1] << std::endl;
            
            switch (readBuffer[0]) {

                case 0x80:
                    std::cout << "Button 7 pressed" << std::endl;
                    break;

                case 0x81:
                    std::cout << "Side button pressed" << std::endl;
                    break;

                case 0x82:
                    std::cout << "Button 3 pressed " << std::endl;
                    break;

                case 0x83:
                    std::cout << "Button 6 pressed" << std::endl;
                    break;

                case 0x84:
                    std::cout << "Big center wheel moved counterclockwise" << std::endl;
                    break;

                case 0x8F:
                    std::cout << "iPod wheel moved clockwise" << std::endl;
                    break;

                case 0x90:
                    std::cout << "D-Pad up pressed" << std::endl;
                    break;

                case 0x91:
                    std::cout << "D-pad down pressed" << std::endl;
                    break;

                case 0x92:
                    std::cout << "D-pad left pressed" << std::endl;
                    break;

                case 0x93:
                    std::cout << "D-Pad right pressed" << std::endl;
                    break;

                case 0x0A:
                        std::cout << "Scroll wheel clicked" << std::endl;
                    break;

                case 0x09:
                        std::cout << "Scroll wheel down used" << std::endl;
                    break;

                case 0x2A:
                        std::cout << "Button 11 pressed" << std::endl;
                    break;

                case 0x22:
                        std::cout << "Button A pressed" << std::endl;
                    break;

                case 0x23:
                        std::cout << "Button B pressed" << std::endl;
                    break;

                case 0x44:
                        std::cout << "Big center wheel moved clockwise" << std::endl;
                    break;

                case 0x49:
                        std::cout << "Scroll wheel up used" << std::endl;
                    break;

                case 0x4F:
                        std::cout << "iPod wheel moved counterclockwise" << std::endl;
                    break;

                // Don't put a default statement here, since the bytes you'd catch would just be the 'RELEASED' version of the keypress signal 
                // (since above you're only listening to the 'PRESSED' version)
            }

            usleep(1000);

        }

        usleep(1000);
        
    }

    /// ---------- ///
    // Clean up
    ioctl(fd, UI_DEV_DESTROY);
    close(fd);


    return 0;
}