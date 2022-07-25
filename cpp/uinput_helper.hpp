/**
 * @file uinput_helper.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-07-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <linux/uinput.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <map>


enum class KeyType
{
    SIDE_BUTTON,
    SCROLL_WHEEL_UP,
    SCROLL_WHEEL_DOWN,
    SCROLL_WHEEL_CLICK,
    BUTTON_3,
    BUTTON_A,
    BUTTON_B,
    BUTTON_6,
    BUTTON_7,
    BUTTON_DPAD_UP,
    BUTTON_DPAD_RIGHT,
    BUTTON_DPAP_DOWN,
    BUTTON_DPAP_LEFT,
    IPOD_WHEEL_CLOCKWISE,
    IPOD_WHEEL_COUNTERCLOCKWISE,
    BIG_CENTER_WHEEL_CLOCKWISE,
    BIG_CENTER_WHEEL_COUNTERCLOCKWISE,
    BUTTON_11
};

std::map<KeyType, int> keyMap = { std::make_pair(KeyType::SIDE_BUTTON, KEY_MUTE),
                                  std::make_pair(KeyType::SCROLL_WHEEL_UP, KEY_SCROLLUP),
                                  std::make_pair(KeyType::SCROLL_WHEEL_DOWN, KEY_SCROLLDOWN),
                                  std::make_pair(KeyType::SCROLL_WHEEL_CLICK, KEY_PAUSE),
                                  std::make_pair(KeyType::BUTTON_3, KEY_ENTER),
                                  std::make_pair(KeyType::BUTTON_A, KEY_A),
                                  std::make_pair(KeyType::BUTTON_B, KEY_B),
                                  std::make_pair(KeyType::BUTTON_6, KEY_DOT),
                                  std::make_pair(KeyType::BUTTON_7, KEY_MENU),
                                  std::make_pair(KeyType::IPOD_WHEEL_CLOCKWISE, KEY_VOLUMEUP),
                                  std::make_pair(KeyType::IPOD_WHEEL_COUNTERCLOCKWISE, KEY_VOLUMEDOWN),
                                  std::make_pair(KeyType::BIG_CENTER_WHEEL_CLOCKWISE, KEY_BRIGHTNESSUP),
                                  std::make_pair(KeyType::BIG_CENTER_WHEEL_COUNTERCLOCKWISE, KEY_BRIGHTNESSDOWN),
                                  std::make_pair(KeyType::BUTTON_11, KEY_ESC)
                                   };

void emit(int fileDescriptor, int type, int code, int val)
{
   struct input_event ie;

   ie.type = type;
   ie.code = code;
   ie.value = val;
   /* timestamp values below are ignored */
   ie.time.tv_sec = 0;
   ie.time.tv_usec = 0;

   write(fileDescriptor, &ie, sizeof(ie));
}


void generateKeyPressEvent(int fileDescriptor, KeyType key)
{
    emit(fileDescriptor, EV_KEY, keyMap[key], 1);
    emit(fileDescriptor, EV_SYN, SYN_REPORT, 0);
    emit(fileDescriptor, EV_KEY, keyMap[key], 0);
    emit(fileDescriptor, EV_SYN, SYN_REPORT, 0);
}


void registerKeyboardEvents(int fileDescriptor)
{
    // Get the the uinput interface ready to register key and mouse events
    ioctl(fileDescriptor, UI_SET_EVBIT, EV_KEY);

    // These are all of the following key types that the Tourbox will be able to produce using this driver
    //ioctl(fileDescriptor, UI_SET_KEYBIT, KEY_A);

    for (const auto keyType: keyMap) {
        ioctl(fileDescriptor, UI_SET_KEYBIT, keyType.second);
    }
}

void registerMouseEvents(int fileDescriptor)
{
    ioctl(fileDescriptor, UI_SET_EVBIT, EV_REL);

    // Tells the uinput interface to allow mouse movements
    ioctl(fileDescriptor, UI_SET_RELBIT, REL_X);
    ioctl(fileDescriptor, UI_SET_RELBIT, REL_Y);
}


int setupKeyboardUinput(void)
{
    struct uinput_setup usetup;

    int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);

    registerKeyboardEvents(fd);

    usleep(1000);

    memset(&usetup, 0, sizeof(usetup));
    usetup.id.bustype = BUS_USB;
    usetup.id.vendor = 0x0483; /* Same vendor as default device */

    usetup.id.product = 0xBEEF; /* Bogus product ID */

    strcpy(usetup.name,"Custom Tourbox TBG_H Driver (Keyboard)");

    ioctl(fd, UI_DEV_SETUP, &usetup);
    ioctl(fd, UI_DEV_CREATE);

    return fd;
}

void destroyUinput(int fileDescriptor)
{
    ioctl(fileDescriptor, UI_DEV_DESTROY);
    close(fileDescriptor);
}