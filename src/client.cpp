#include "client.h"

#include <utility>

namespace rv_client {

    RVClient::RVClient(std::string address, unsigned short port)
    : m_address(std::move(address)), m_port(port) {}

    RVClient::RVClient(std::string address)
    : m_address(std::move(address)) {}

    void RVClient::run_client() {
        std::cout << "hello";
    }

}