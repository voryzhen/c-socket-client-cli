#include "client.h"

#include <utility>

namespace rv_client {

    unsigned int RVClient::server_connect = 0;
    bool RVClient::is_run_operating_loop = true;

    void * RVClient::ClientHandler() {
        while(is_run_operating_loop) {
            char msg[4096];
            int size = recv(server_connect, msg, 4096, NULL);
            msg[size] = '\0';
            std::cout << std::string(msg) << std::endl;
        }
    }

    RVClient::RVClient(std::string address, unsigned short port)
    : m_address(std::move(address)), m_port(port) {}

    RVClient::RVClient(std::string address)
    : m_address(std::move(address)) {}

    int RVClient::run_client() {
        if (init_and_connect() != 0)
            return 1;
        run_operating_loop();
        shutdown();
        return 0;
    }

    int RVClient::init_and_connect() {
#ifdef _WIN32
        WSAData wsaData;
        WORD DLLVersion = MAKEWORD(2, 1);
        if(WSAStartup(DLLVersion, &wsaData) != 0) {
            std::cerr << "WSAStartup failed with error: " << std::endl;
            return 1;
        }
#endif

        struct sockaddr_in address_in{};
        address_in.sin_family = AF_INET;
        address_in.sin_port = htons(m_port);
        inet_pton(AF_INET, m_address.c_str(), &address_in.sin_addr.s_addr);

        server_connect = socket(AF_INET, SOCK_STREAM, NULL);

        if(connect(server_connect, (SOCKADDR*)&address_in, sizeof(address_in)) != 0) {
            std::cout << "Error: failed connect to server.\n";
            return 1;
        }
        std::cout << "Connected!\n";

        // Accept hello from server
        char msg[4096];
        int size = recv(server_connect, msg, 4096, 0);
        if (size > 0) {
            msg[size] = '\0';
            std::cout << std::string(msg) << std::endl;
            return 0;
        }
        return 1;

    }

    void RVClient::run_operating_loop() {

        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);
#ifdef _WIN32
        SetConsoleOutputCP(1251);
        SetConsoleCP(1251);
#endif
        while(is_run_operating_loop) {
            std::string msg;
            if (std::cin >> msg) {
                send(server_connect, msg.c_str(), (int) (msg.size() + 1), 0);
            }
        }
    }

    void RVClient::shutdown() {
#ifdef _WIN32
        WSACleanup();
#endif
    }

}