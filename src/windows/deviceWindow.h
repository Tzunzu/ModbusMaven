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
        
        const char* current_item;
        int current_item_index;

public:

      DeviceWindow(std::string WindowName);    // Constructor
      void render(); // Render the window
      
private:
    int deviceId = 0;
    int functionCode = 0;
    int numRegisters = 10;

    struct Register {
        int address;
        int value;
        const char* type;
        char* description;
    };
        
    std::vector<Register> registers;
};




#endif // DEVICEWINDOW_H
