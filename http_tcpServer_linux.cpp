#include "http_tcpServer_linux.h"
#include <iostream>
#include <sstream>
#include <unistd.h>

namespace http {

    TcpServer::TcpServer(std::string ip_address, int port) 
        : m_ip_address(ip_address), m_port(port), m_socket(),
          m_new_socket(), m_incoming_message(), m_socket_address(),
          m_socket_address_len(sizeof(m_socket_address)),
          m_server_message(build_response())
    {
        start_server();
    }

    TcpServer::~TcpServer() {
        close_server();
    }

    int TcpServer::start_server() {
        m_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (m_socket < 0) {
            std::cout << "Cannot create socket.\n";
            exit(1);
            return 1;
        }
        return 0;
    }

    void TcpServer::close_server() {
        close(m_socket);
        close(m_new_socket);
        exit(0);    
    }

    std::string TcpServer::build_response() {
        return "";
    }
    
} // namespace http
