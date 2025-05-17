/*This class handles input interpretation. Mostly just returning true and false depending on the buttons held.
This also determines stick deadzones and how digital input is registered on said sticks.*/

#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#include <XInput.h>
#include <thread>

#include "Controller.cpp"

#pragma once


using namespace std;
using namespace sf;


class pController {

    TController* controller0;
    //Deadzone effects x and y
    int stickDeadzone = 5000;
    
public:pController(int n) {
    controller0 = new TController(n);
}
public:pController() {
    controller0 = new TController(0);
}

public:Vector2f checkLSTICK() {
    int x = controller0->GetState().Gamepad.sThumbLX;
    int y = controller0->GetState().Gamepad.sThumbLY;
    if (x < stickDeadzone && x > -(stickDeadzone)) {
        x = 0;
    }
    if (y < stickDeadzone && y > -(stickDeadzone)) {
        y = 0;
    }
    return Vector2f(x, y);
}
public:
    Vector2i checkRSTICK() {
        int x = controller0->GetState().Gamepad.sThumbRX;
        int y = controller0->GetState().Gamepad.sThumbRY;
        if (x < stickDeadzone && x > -(stickDeadzone)) {
            x = 0;
        }
        if (y < stickDeadzone && y > -(stickDeadzone)) {
            y = 0;
        }
        return Vector2i(x, y);
    }

    float checkRSTICK_Angle() {
        Vector2i stick = checkRSTICK();
        float angle;
        float tempX = abs(stick.x);
        float tempY = abs(stick.y);
        angle = atan((tempY / tempX)) * 57.295779513082320876798154814105;
        

        if (stick.x < 0) {
            if (stick.y <= 0) {

                angle = 90 - angle;

                angle = angle + 90;
            }
            else if (stick.y > 0) {
                angle = angle + 180;
            }
        }
        else if (stick.x >= 0) {
            if (stick.y > 0) {
                angle = 90 - angle;
                angle = angle + 270;
            }
        }
        return angle;
    }

public:bool checkA() {
    if (controller0->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A)
    {
        //controller0->vibrate(65535, 0);
        return true;
    }
    else if (Keyboard::isKeyPressed(Keyboard::Scan::Z)) {
        return true;
    }
    return false;
}

public:bool checkB() {
    if (controller0->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_B)
    {
        //controller0->vibrate(0, 65535);
        return true;

    }
    else if (Keyboard::isKeyPressed(Keyboard::Scan::X)) {
        return true;
    }
    return false;
}

public:bool checkX()
{
    if (controller0->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_X)
    {
        return true;
    }
    return false;
}
public:bool checkY()
{
    if (controller0->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_Y)
      {
        //controller0->vibrate();
        return true;
        //controller0->vibrate();
    }
      return false;
}
public:bool checkUP() {
    if (controller0->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) {
        return true;
    }
    else if (checkLSTICK().y > 20000) {
        return true;
    }
    else if (Keyboard::isKeyPressed(Keyboard::Scan::Up)) {
        return true;
    }
    return false;
}
public:bool checkDOWN() {
    if (controller0->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
        return true;
    }

    else if (checkLSTICK().y < -20000) {
        return true;
    }
    else if (Keyboard::isKeyPressed(Keyboard::Scan::Down)) {
        return true;
    }
    return false;
}
public:bool checkLEFT() {
    if (controller0->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
        return true;
    }
    else if (checkLSTICK().x < -20000) {
        return true;
    }
    else if (Keyboard::isKeyPressed(Keyboard::Scan::Left)) {
        return true;
    }
    return false;
}
public:
    bool checkRIGHT() {
        if (controller0->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
            return true;
        }
        else if (checkLSTICK().x > 20000) {
            return true;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Scan::Right)) {
            return true;
        }
        return false;
    }
    bool checkL() {
        if (controller0->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
            return true;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Scan::Space)) {
            return true;
        }
        return false;
    }
    bool checkR() {
        if (controller0->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
            return true;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Scan::C)) {
            return true;
        }
        return false;
    }




public:bool checkSELECT() {
    if (controller0->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
    {
        return true;
    }
    return false;
}
public:bool checkSTART() {
    if (controller0->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_START)
    {
        return true;
    }
    else if (Keyboard::isKeyPressed(Keyboard::Scan::Enter)) {
        return true;
    }
    return false;
}
};