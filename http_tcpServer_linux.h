#ifndef INCLUDED_HTTP_TCPSERVER_LINUX
#define INCLUDED_HTTP_TCPSERVER_LINUX 

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string>

namespace http {

    class TcpServer {

        public:
            TcpServer(std::string ip_address, int port);
            ~TcpServer();

        private:
            std::string m_ip_address;
            int m_port;
            int m_socket;
            int m_new_socket;
            long m_incoming_message;
            struct sockaddr_in m_socket_address;
            unsigned int m_socket_address_len;
            std::string m_server_message;

            int start_server();
            void close_server();
            std::string build_response();
    };
    
}  // namespace http

#endif
