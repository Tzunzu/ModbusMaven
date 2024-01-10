#include <iostream>

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <glad/glad.h>

#include "../vendor/imgui_impl_glfw.h"
#include "../vendor/imgui_impl_opengl3.h"
#include "Modbus_Tcp.h"
#include "windows/deviceWindow.h"

#define GLSL_VERSION "#version 330"

static ModbusTcp modbusTcp;
static int deviceWindowCounter = 0;
static std::vector<DeviceWindow> deviceWindows;
int main()
{

    bool show_demo_window = false;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(600, 600, "Modbus Maven",
                                          nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Could not create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Could not initialize GLAD" << std::endl;
        return -1;
    }

    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.IniFilename = "imgui.ini";
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // Create a full viewport for the docking space
        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus;
        ImGui::Begin("ModbusMavenDock", nullptr, window_flags);

        ImGui::PopStyleVar(3);

        // Dockspace
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

        // create a menu bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit"))
                {
                    glfwSetWindowShouldClose(window, true);
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Connect"))
            {
                if (ImGui::MenuItem("Conncet to Modbus TCP/IP"))
                {
                    std::cout << "Trying to connect: " << std::endl;
                    bool connected = modbusTcp.openModbusTcpConnection("127.0.0.1", 502);

                    if (connected)
                    {
                        std::vector<int> values = modbusTcp.readModbusTcpRegisters(0, 10, 1, 3);
                        for (int i = 0; i < values.size(); i++)
                        {
                            std::cout << "Register " << i << ": " << values[i] << std::endl;
                        }
                    }

                    // Open a  window to configure the connection
                    // ImGui::OpenPopup("Connect to Modbus TCP/IP");
                }
                if (ImGui::MenuItem("Conncet to Modbus RTU"))
                {
                    // Open demo window
                    show_demo_window = true;
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Now you can create your window
        ImGui::Begin("Device List");
        {
            // In your render loop:
            if (ImGui::Button("Add Device", ImVec2(100, 20)))
            {
                

                std::string windowName;
                    
                windowName = "Device " + std::to_string(deviceWindows.size());
					
                // Create a new device window
                   DeviceWindow Window =  DeviceWindow(windowName);
                
                  deviceWindows.push_back(Window);

            }
            
            for (int i = 0; i < deviceWindows.size(); i++)
            {
				// Render the device window 
                deviceWindows[i].render();
                
            }

        }
        ImGui::End(); // End of Hello, ImGUI! window
        ImGui::End(); // End of DockSpace



        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update Platform and Renderer bindings
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow *backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }

    modbusTcp.closeModbusTcpConnection();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
