/**
 * @file main.cpp
 * @author Raleigh Littles <raleighlittles@gmail.com>
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
#include <signal.h>

// Local
#include "uinput_helper.hpp"


// Remember, can't pass data to signals
int gUinputFileDescriptor = 0;

void sigint_handler(sig_atomic_t /* s */)
{
    destroyUinput(gUinputFileDescriptor);
    exit(1);
}

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
    /// Setup the virtual driver

    gUinputFileDescriptor = setupUinput();

    // Register signal handler to make sure virtual device gets cleaned up
    signal(SIGINT, sigint_handler);

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
                    generateKeyPressEvent(gUinputFileDescriptor, KeyType::BUTTON_7);
                    break;

                case 0x81:
                    std::cout << "Side button pressed" << std::endl;
                    generateKeyPressEvent(gUinputFileDescriptor, KeyType::SIDE_BUTTON);
                    break;

                case 0x82:
                    std::cout << "Button 3 pressed " << std::endl;
                    generateKeyPressEvent(gUinputFileDescriptor, KeyType::BUTTON_3);
                    break;

                case 0x83:
                    std::cout << "Button 6 pressed" << std::endl;
                    generateKeyPressEvent(gUinputFileDescriptor, KeyType::BUTTON_6);
                    break;

                case 0x84:
                    std::cout << "Big center wheel moved counterclockwise" << std::endl;
                    generateKeyPressEvent(gUinputFileDescriptor, KeyType::IPOD_WHEEL_COUNTERCLOCKWISE);
                    break;

                case 0x8F:
                    std::cout << "iPod wheel moved clockwise" << std::endl;
                    generateKeyPressEvent(gUinputFileDescriptor, KeyType::IPOD_WHEEL_CLOCKWISE);
                    break;

                case 0x90:
                    std::cout << "D-Pad up pressed" << std::endl;
                    emit(gUinputFileDescriptor, EV_REL, REL_Y, -5);
                    emit(gUinputFileDescriptor, EV_SYN, SYN_REPORT, 0);
                    break;

                case 0x91:
                    std::cout << "D-pad down pressed" << std::endl;
                    emit(gUinputFileDescriptor, EV_REL, REL_Y, +5);
                    emit(gUinputFileDescriptor, EV_SYN, SYN_REPORT, 0);
                    break;

                case 0x92:
                    std::cout << "D-pad left pressed" << std::endl;
                    emit(gUinputFileDescriptor, EV_REL, REL_X, -5);
                    emit(gUinputFileDescriptor, EV_SYN, SYN_REPORT, 0);
                    break;

                case 0x93:
                    std::cout << "D-Pad right pressed" << std::endl;
                    emit(gUinputFileDescriptor, EV_REL, REL_X, +5);
                    emit(gUinputFileDescriptor, EV_SYN, SYN_REPORT, 0);
                    break;

                case 0x0A:
                        std::cout << "Scroll wheel clicked" << std::endl;
                        generateKeyPressEvent(gUinputFileDescriptor, KeyType::SCROLL_WHEEL_CLICK);
                    break;

                case 0x09:
                        std::cout << "Scroll wheel down used" << std::endl;
                        generateKeyPressEvent(gUinputFileDescriptor, KeyType::SCROLL_WHEEL_DOWN);
                    break;

                case 0x2A:
                        std::cout << "Button 11 pressed" << std::endl;
                        generateKeyPressEvent(gUinputFileDescriptor, KeyType::BUTTON_11);
                    break;

                case 0x22:
                        std::cout << "Button A pressed" << std::endl;
                        generateKeyPressEvent(gUinputFileDescriptor, KeyType::BUTTON_A);

                    break;

                case 0x23:
                        std::cout << "Button B pressed" << std::endl;
                        generateKeyPressEvent(gUinputFileDescriptor, KeyType::BUTTON_B);
                    break;

                case 0x44:
                        std::cout << "Big center wheel moved clockwise" << std::endl;
                        generateKeyPressEvent(gUinputFileDescriptor, KeyType::BIG_CENTER_WHEEL_CLOCKWISE);
                    break;

                case 0x49:
                        std::cout << "Scroll wheel up used" << std::endl;
                        generateKeyPressEvent(gUinputFileDescriptor, KeyType::SCROLL_WHEEL_UP);
                    break;

                case 0x4F:
                        std::cout << "iPod wheel moved counterclockwise" << std::endl;
                        generateKeyPressEvent(gUinputFileDescriptor, KeyType::IPOD_WHEEL_COUNTERCLOCKWISE);
                    break;

                // Don't put a default statement here, since the bytes you'd catch would just be the 'RELEASED' version of the keypress signal 
                // (since above you're only listening to the 'PRESSED' version)
            }

            usleep(1000);

        }

        usleep(1000);
        
    }

    // Clean up (although currently you can't get here)
    destroyUinput(gUinputFileDescriptor);


    return 0;
}