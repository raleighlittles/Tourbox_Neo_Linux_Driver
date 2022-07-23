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

int main(int /* argc */, char** /* argv[] */ )
{
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

    std::array<uint8_t, 2> readBuffer;

    while (true) {

        ssize_t bytesRead = read(serialPortFileDescriptor, readBuffer.begin(), 1);

        if (bytesRead < 0) {
            std::cerr << "Error reading from serial port" << std::endl;
            return 2;
        }

        if (bytesRead > 0) {

            std::cout << std::hex << std::setfill('0') << std::setw(2) << readBuffer[0] << " " << readBuffer[1];
            
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

                // End 'simple' buttons

                case 0x0A:
                    // if (readBuffer[1] == 0x0A) {
                        std::cout << "Scroll wheel clicked" << std::endl;
                    // }
                    break;

                case 0x09:
                    //if (readBuffer[1] == 0x89) {
                        std::cout << "Scroll wheel down used" << std::endl;
                    //}
                    break;

                case 0x2A:
                    // if (readBuffer[1] == 0xAA) {
                        std::cout << "Button 11 pressed" << std::endl;
                    // }
                    break;

                case 0x22:
                    // if (readBuffer[1] == 0xA2) {
                        std::cout << "Button A pressed" << std::endl;
                    // }
                    break;

                case 0x23:
                    // if (readBuffer[1] == 0xA3) {
                        std::cout << "Button B pressed" << std::endl;
                    // }
                    break;

                case 0x44:
                    //if (readBuffer[1] == 0xC4) {
                        std::cout << "Big center wheel moved clockwise" << std::endl;
                   // }
                    break;

                case 0x49:
                    //if (readBuffer[1] == 0xC9) {
                        std::cout << "Scroll wheel up used" << std::endl;
                    // }
                    break;

                case 0x4F:
                    // if (readBuffer[1] == 0xCF) {
                        std::cout << "iPod wheel moved counterclockwise" << std::endl;
                    // }
                    break;


            }

            usleep(1000);

        }

        usleep(1000);
        
    }

}