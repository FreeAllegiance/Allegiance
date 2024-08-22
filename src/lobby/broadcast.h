#pragma once

#include <winsock.h>

//
// This class handles opening of UDP port to send
// or receive broadcasts with cross router jumps.
// 

class UdpSocket
{
    protected:
        SOCKET m_sock;
        
    public:
        UdpSocket();
       ~UdpSocket();

        int Bind( unsigned short port, unsigned long addr = INADDR_ANY );
        int BindExcludePort( unsigned short port, unsigned long addr = INADDR_ANY  );

        int Close();    
        int EnableBroadcast();
        int SendTo(void* pBuffer, int size, unsigned short port, unsigned long addr = INADDR_BROADCAST  );
        int Recv(void* pBuffer,int size);

};
    
#if 0
class Broadcast {
public:
    Broadcast();
    ~Broadcast();
    //Semantics are 0 or if SOCKET_ERROR use
    //WSAGetLastError    


    int Bind(unsigned short port);
    int Close();
    //You shouldn't send over 512 bytes
    //This call uses blocking version of sendto call
    int Send(void* pBuffer,int size);
    
protected:
    SOCKET m_sock;
    SOCKADDR_IN m_addr;
    
};


class Listen: public Broadcast {
public:
    Listen() {;};
    //Semantics are 0 or if SOCKET_ERROR use
    //WSAGetLastError    

    // use INADDR_ANY to receive on all NICs
    int Bind(unsigned long addr, unsigned short port);
    
    int Recv(void* pBuffer,int size);
    
};

#endif