// OpenalCppConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <windows.h>

/**
 * Device's extended specifier string.
 *
 * If device handle is NULL, it is instead a null-character separated list of
 * strings of known extended device specifiers (list ends with an empty string).
 */
#define ALC_ALL_DEVICES_SPECIFIER                0x1013

 /**
  * Capture device specifier string.
  *
  * If device handle is NULL, it is instead a null-character separated list of
  * strings of known capture device specifiers (list ends with an empty string).
  */
#define ALC_CAPTURE_DEVICE_SPECIFIER             0x310

typedef void* (*_GetProcAddress) (const char*);
_GetProcAddress _alGetProcAddress;

typedef const char* (*_GetString)(void*, int);
_GetString _alcGetString;


void parse_device_list(const char* sourceList, std::vector<std::string>& deviceList);
void show_device_list();

void parse_device_list(const char* sourceList, std::vector<std::string>& deviceList)
{
    std::string str;
    int index = 0;

    bool canContinue = true;
    while (canContinue) {
        auto ch = *(sourceList + index);
        if (ch == '\0') {
            if (str.empty())
                break;
            else {
                deviceList.push_back(str);
                str = "";
                ++index;
            }
        }
        else {
            str += ch;
            ++index;
        }
    }
}

void show_device_list()
{
    std::vector<std::string> deviceList;
    auto allPlayingDevices = _alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER);
    parse_device_list(allPlayingDevices, deviceList);
    auto allCaptureDevices = _alcGetString(nullptr, ALC_CAPTURE_DEVICE_SPECIFIER);
    parse_device_list(allCaptureDevices, deviceList);
    for (auto& deviceName : deviceList)
        std::cout << deviceName << std::endl;
}

int main() {
    HMODULE hLib = LoadLibrary(L"OpenAL32.dll");
    if (!hLib)
        throw;
    _alGetProcAddress = (_GetProcAddress)GetProcAddress(hLib, "alGetProcAddress");
    if (!_alGetProcAddress)
        throw;
    _alcGetString = (_GetString)GetProcAddress(hLib, "alcGetString");
    if (!_alcGetString)
        throw;

    while (true)
    {
        std::string line;
        std::getline(std::cin, line);
        if (line == "exit")
            break;
        else if (line == "show")
            show_device_list();
    }
}

