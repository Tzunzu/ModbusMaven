#ifndef DEVICEWINDOW_H
#define DEVICEWINDOW_H

// Include any necessary headers here
#include <imgui.h>
#include <string>
#include <vector>

//make sure noting is static


class DeviceWindow {


private: 
        std::string m_WindowName;
        bool is_selected;
        const char* current_item;
      

public:

      DeviceWindow(std::string WindowName);    // Constructor
      void render(); // Render the window
      
private:
    int deviceId = 0;
    int functionCode = 0;
    int numRegisters = 0;

    struct Register {
        int address;
        int value;
        char type[80];
        char description[80];
    };

    
    std::vector<Register> registers;
};




#endif // DEVICEWINDOW_H
