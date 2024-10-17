/*This sets up an X input controller. I did not make this, nor am I sure of how it works.*/

//#include <iostream>
#include <windows.h>
#include <XInput.h>
//using namespace std;
#pragma once

#pragma comment(lib, "XInput.lib")

class TController
{
private:
    int n;
    XINPUT_STATE state;
    

public:
    TController(int num)
    {
        n = num;
    }

    XINPUT_STATE GetState()
    {
        ZeroMemory(&state, sizeof(XINPUT_STATE));
        XInputGetState(n, &state);
        return state;
    }

    bool IsConnected()
    {
        ZeroMemory(&state, sizeof(XINPUT_STATE));
        DWORD statenow = XInputGetState(n, &state);

        if (statenow == ERROR_SUCCESS) return true;

        return false;
    }

public:
    void vibrate(int LV = 0, int RV = 0)
    {
        XINPUT_VIBRATION vibration;
        ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

        vibration.wLeftMotorSpeed = LV;
        vibration.wRightMotorSpeed = RV;

        XInputSetState(n, &vibration);
    }
    
};