#include "deviceWindow.h"


bool isOpen = true;

      DeviceWindow::DeviceWindow(std::string WindowName) {
        // Initialize device ID, function code, and number of registers
        deviceId = 0;
        functionCode = 0;
        numRegisters = 0;
        m_WindowName = WindowName;
        //Make sure a new window is created every time
        ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
    }

 

      void DeviceWindow::render() {
       

                   
        
          // create window
        if (!ImGui::Begin(m_WindowName.c_str(),&isOpen,0)){
            ImGui::End();
            return;
        }

        // Display device ID, function code, and number of registers
        //add input for device ID, function code, and number of registers
        ImGui::InputInt("Device ID", &deviceId,0,0,0);
        if (deviceId < 0) { deviceId = 0; }
        if (deviceId > 255) { deviceId = 255; }

        ImGui::InputInt("Nr Of Registers", &numRegisters,0,0,0);
        if (numRegisters < 0) { numRegisters = 0;}
        if (numRegisters > 120) { numRegisters = 120; }

        //function code is a drop down menu with the options Read Coils, Read Discrete Inputs, Read Holding Registers, Read Input Registers, Write Single Coil, Write Single Register, Write Multiple Coils, Write Multiple Registers, Mask Write Register, Read/Write Multiple Registers, Read FIFO Queue, and Read File Record
        const char* functionList[12] = { "Read Coils", "Read Discrete Inputs", "Read Holding Registers", "Read Input Registers", "Write Single Coil", "Write Single Register", "Write Multiple Coils", "Write Multiple Registers", "Mask Write Register", "Read/Write Multiple Registers", "Read FIFO Queue", "Read File Record" };

            current_item = functionList[0];            // Here our selection is a single pointer stored outside the object.
        if (ImGui::BeginCombo("Function Code", current_item)) // The second parameter is the label previewed before opening the combo.
        {
            bool is_selected;
            for (int n = 0; n < IM_ARRAYSIZE(functionList); n++)
            {
				 is_selected = (current_item == functionList[n]); // You can store your selection however you want, outside or inside your objects
				if (ImGui::Selectable(functionList[n], is_selected))
					current_item = functionList[n];
				if (is_selected)
					ImGui::SetItemDefaultFocus(); // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
			}
			ImGui::EndCombo();
		}

         

        
        // Display table of registers
        ImGui::Text("Registers:");
        ImGui::Columns(4, "registers_table");
        ImGui::Separator();
        ImGui::Text("Address");
        ImGui::NextColumn();
        ImGui::Text("Value");
        ImGui::NextColumn();
        ImGui::Text("Type");
        ImGui::NextColumn();
        ImGui::Text("Description");
        ImGui::NextColumn();
        ImGui::Separator();

        registers = std::vector<Register>(numRegisters);
        const char* DataTypeList[7] = { "UInt16","Int16", "UInt32","Int32","UInt64","Int64" "Float", "Double" };
        for (int i = 0; i < numRegisters; i++) {
            ImGui::Text("%d" , registers[i].address);
            ImGui::NextColumn();
            ImGui::Text("%d", registers[i].value);
            ImGui::NextColumn();

            
            registers[i].type = DataTypeList[0];

            std::string label = "##combo" + std::to_string(i);

            if (ImGui::BeginCombo(label.c_str(), registers[i].type, ImGuiComboFlags_None)) { // The second parameter is the label previewed before opening the combo.
                bool is_selected;
                for (int n = 0; n < IM_ARRAYSIZE(DataTypeList); n++)
                {
                    is_selected = (current_item == DataTypeList[n]); // You can store your selection however you want, outside or inside your objects
                    if (ImGui::Selectable(DataTypeList[n], is_selected))
                        current_item = DataTypeList[n];
                    if (is_selected)
                        ImGui::SetItemDefaultFocus(); // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
                }
                ImGui::EndCombo();
            }
                                  
            ImGui::NextColumn();
            label = "##input" + std::to_string(i);
            char buffer[80];
            strcpy(buffer, registers[i].description);
            if (ImGui::InputText(label.c_str(), buffer, 80)) {
                registers[i].description = buffer; // Update the description of the register
            }
            
            

            ImGui::NextColumn();

        }
        ImGui::Columns(1);
        
        ImGui::End();
    };



