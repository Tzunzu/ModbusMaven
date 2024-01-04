
#include <iostream>
#include <string>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "Modbus_Tcp.h"

// Declare any classes or functions here

SOCKET ModbusTcp::get_socket(){
    return mySocket;
 }   

    // Function to establish a Modbus TCP connection
    bool ModbusTcp::openModbusTcpConnection(const std::string &ipAddress, int port)
    {

        // Initialize Winsock
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0)
        {
            std::cout << "Failed to initialize Winsock." << std::endl;
            return false;
        }

        // Create a socket
        mySocket = socket(AF_INET, SOCK_STREAM, 0);
        if (mySocket == INVALID_SOCKET)
        {
            std::cout << "Failed to create socket." << std::endl;
            WSACleanup();
            return false;
        }

        // Set the socket timeout
        int timeoutMillis = SocketTimeoutSek * 1000; // Convert to milliseconds
        if (setsockopt(mySocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeoutMillis, sizeof(timeoutMillis)) < 0)
        {
            std::cout << "Failed to set socket timeout." << std::endl;
            closesocket(mySocket);
            WSACleanup();
            return false;
        }

        // Connect to the server
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(port);
        serverAddress.sin_addr.s_addr = inet_addr(ipAddress.c_str());
        if (connect(mySocket, (sockaddr *)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
        {
            std::cout << "Failed to connect to server." << std::endl;
            return false;
        }
        return true;
    }

    // Function to read Modbus TCP registers
    std::vector<int> ModbusTcp::readModbusTcpRegisters(int startAddress, int numRegisters, int slaveId, int functionCode)
    {
        // Send request to server
        std::vector<int> values;
        char request[12];
        request[0] = 0x00;
        request[1] = 0x01;
        request[2] = 0x00;
        request[3] = 0x00;
        request[4] = 0x00;
        request[5] = 0x06;
        request[6] = slaveId;
        request[7] = functionCode;
        request[8] = startAddress >> 8;
        request[9] = startAddress & 0x00FF;
        request[10] = numRegisters >> 8;
        request[11] = numRegisters & 0x00FF;
        
        send(mySocket, request, sizeof(request), 0);

        // Receive response
        char response[256];
        recv(mySocket, response, sizeof(response), 0);

        // Check the response
        if (response[7] == 0x83)
        {
            std::cout << "Modbus exception response." << std::endl;
            return values;
        }
        else if (response[7] != 0x03)
        {
            std::cout << "Modbus response error." << std::endl;
            return values;
        }
        else if (response[8] != numRegisters * 2)
        {
            std::cout << "Modbus response error." << std::endl;
            return values;
        }
        else
        {
            // Extract the values from the response
            for (int i = 0; i < numRegisters; i++)
            {
                int value = response[9 + i * 2] << 8;
                value += response[10 + i * 2];
                values.push_back(value);
            }
            return values;
        }
       
    };

    // Function to close the Modbus TCP connection
    bool ModbusTcp::closeModbusTcpConnection()
    {
        // Close the socket
        closesocket(mySocket);

        // Cleanup Winsock
        WSACleanup();
        return true;
    }

    // Function to write Modbus TCP registers
    bool ModbusTcp::writeModbusTcpRegisters(int startAddress, const std::vector<int> &values)
    {
        // TODO: Implement the write logic here
        // Return true if the write operation is successful, false otherwise
        return false;
    }

