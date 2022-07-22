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

        ssize_t bytesRead = read(serialPortFileDescriptor, readBuffer.begin(), 2);

        if (bytesRead < 0) {
            std::cerr << "Error reading from serial port" << std::endl;
            return 2;
        }

        if (bytesRead > 0) {

            std::cout << std::hex << std::setfill('0') << std::setw(2) << readBuffer[0] << " " << readBuffer[1];
            
            switch (readBuffer[0]) {

                case 0x81:
                    std::cout << "Side button pressed" << std::endl;

            }

            usleep(1000);

        }

        usleep(1000);
        
    }

}