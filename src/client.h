#include "iostream"

#ifdef _WIN32
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<arpa/inet.h>
#endif

namespace rv_client{

    class RVClient
    {
    public:
        RVClient(std::string  address, unsigned short port);
        RVClient(std::string  address);
        RVClient() = default;
        void run_client();

    private:
        unsigned short m_port = 8000;
        std::string m_address = "127.0.0.1";
    };
}
