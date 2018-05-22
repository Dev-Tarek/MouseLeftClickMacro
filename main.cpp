// Author: Abdelrahman Tarek
#include <iostream>
#include <stdio.h>
#include <string.h>

#define _WIN32_WINNT 0x0500
#include <windows.h>

using namespace std;

const int MacroLimit = 100;

struct Macro{
    int waitTime;   // Sleep time before click.
    int sleepTime;  // Sleep time after click.
    int mcount;     // Number of clicks in this Macro.
    POINT CursorPos[MacroLimit];
    Macro(){
        waitTime = 50;
        sleepTime = 50;
        mcount = -1;
    }
    void Reset(){
        mcount = -1;
        memset(CursorPos, 0, sizeof(CursorPos));
        cout << "Macro Reset.\n";
    }
};

void LeftClick(){
    INPUT Input = {0};
    // Activate Left Click
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &Input, sizeof(INPUT));

    ZeroMemory(&Input, sizeof(INPUT));
    // Release Left Click
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &Input, sizeof(INPUT));
}

Macro CreateMacro(int sleeptime = 50, int waittime = 50){
    Macro macro;
    macro.sleepTime = sleeptime;
    macro.waitTime = waittime;
    return macro;
}

bool MacroInsert(Macro* macro){
    if(macro->mcount >= MacroLimit) return false;
    // Store Mouse x,y (POINT) in the Macro's cursor array.
    GetCursorPos(&macro->CursorPos[++macro->mcount]);
    cout << "Click Recorded. Remaining: " << MacroLimit - macro->mcount-1 <<" clicks\n";
    return true;
}

void RunMacro(Macro* macro){
    cout << "Macro Start.\n";
    for(int i=0; i<=macro->mcount; i++){
        SetCursorPos(macro->CursorPos[i].x, macro->CursorPos[i].y);
        Sleep(macro->waitTime);
        LeftClick();
        Sleep(macro->sleepTime);
    }
}

int main(){
    cout << "- Mouse Left Click Macro Recorder -\n";
    cout << "A: Record Click - S: Start Macro - R: Reset Macro - Q: Quit\nWait Time (ms) = ";
    int speed; cin>>speed; cout<<"Ready.\n";

    Macro macro = CreateMacro();
    bool MacroAdded = true, MacroExecuted = true, MacroReset = true;

    while(true){
        if(GetAsyncKeyState('A'))   // This function is fast.
            MacroAdded = false;     // This is done for control.
        else if(!GetAsyncKeyState('A')){
            if(!MacroAdded) MacroInsert(&macro);
            MacroAdded = true;
        }

        if(GetAsyncKeyState('S'))
            MacroExecuted = false;
        else if(!GetAsyncKeyState('S')){
            if(!MacroExecuted) RunMacro(&macro);
            MacroExecuted = true;
        }

        if(GetAsyncKeyState('R'))
            MacroReset = false;
        else if(!GetAsyncKeyState('R')){
            if(!MacroReset) macro.Reset();
            MacroReset = true;
        }

        if(GetAsyncKeyState('Q'))
            break;

        Sleep(speed);
    }
    return 0;
}
