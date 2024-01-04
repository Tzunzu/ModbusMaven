
#ifndef MODBUS_TCP_H
#define MODBUS_TCP_H

// Include any necessary headers here
#include <string>
#include <vector>
#include <winsock2.h>



// Declare any classes or functions here
#pragma once
   class ModbusTcp{
    private:
      SOCKET mySocket;
      UINT16 SocketTimeoutSek = 1;

    public:
        SOCKET get_socket();
        // Function to open a Modbus TCP connection
        bool openModbusTcpConnection(const std::string &ipAddress, int port);

        // Function to read Modbus TCP registers
        std::vector<int> readModbusTcpRegisters(int startAddress, int numRegisters, int slaveId, int functionCode);

        // Function to write Modbus TCP registers
        bool writeModbusTcpRegisters(int startAddress, const std::vector<int> &values);

        // Function to close the Modbus TCP connection
        bool closeModbusTcpConnection();

   
   };

#endif // MODBUS_TCP_H
