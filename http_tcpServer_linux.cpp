#include "http_tcpServer_linux.h"

#include <iostream>
#include <sstream>
#include <unistd.h>

namespace {
    const int BUFFER_SIZE = 30720;
    const int INT_MAX = 2147483647; 
    
    void exit_with_error(const std::string& e_message) {
        std::cout << e_message << std::endl;
        exit(1);
    }
}

namespace http {

    TcpServer::TcpServer(std::string ip_address, int port) 
        : m_ip_address(ip_address), m_port(port), m_socket(), m_new_socket(), 
          m_incoming_message(), 
          m_socket_address(), m_socket_address_len(sizeof(m_socket_address)),
          m_server_message(build_response()), total_connections(0)
    {
        m_socket_address.sin_family = AF_INET; 
        m_socket_address.sin_port = htons(m_port);
        m_socket_address.sin_addr.s_addr = inet_addr(m_ip_address.c_str());
        
        if(start_server() != 0) {
            // If the server does not start        
            std::ostringstream ss;
            ss << "Failed to start server with PORT: " << ntohs(m_socket_address.sin_port);
            std::cout << ss.str() << std::endl;
        }

    }

    TcpServer::~TcpServer() {
        close_server();
    }

    int TcpServer::start_server() {
        /**
        * Starts the server by creating a socket, and binding it to
        * a socket address.
        */
        m_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (m_socket < 0) {
            exit_with_error("Cannot create socket.");
            return 1;
        }

        // Bind socket to address
        if (bind(m_socket, (sockaddr *) &m_socket_address, m_socket_address_len) < 0) {
            exit_with_error("Cannot connect socket to address.");
            return 1;
        }
        
        return 0;
    }

    void TcpServer::close_server() {
        /**
        * Closes all sockets then exits with code 0;
        */
        close(m_socket);
        close(m_new_socket);
        exit(0);    
    }

    void TcpServer::start_listen() {
        /** 
        * Listens on the indicated address and port for external connections.
        */
        if (listen(m_socket, 20) < 0) {
            std::cout << "Socket listen failure." << std::endl;
        }

        // Print listen info about the server 
        std::ostringstream ss;
        ss << "\n*** Listening on ADDRESS: " << inet_ntoa(m_socket_address.sin_addr)
           << " PORT: " << ntohs(m_socket_address.sin_port) << " ***\n\n";
        std::cout << ss.str() << std::endl;

        int bytes_received;

        while (true) {
            std::cout << "====== Waiting for a new connection ======\n\n\n";
            accept_connection(m_new_socket);

            char buffer[BUFFER_SIZE] = {0};

            bytes_received = read(m_new_socket, buffer, BUFFER_SIZE);
            if (bytes_received < 0) {
                exit_with_error("Failed to read bytes from client socket connection");
            }

            std::cout << "------ Received Request from client ------\n\n";
            if (total_connections == INT_MAX) {
                total_connections = INT_MAX;
            } else {
                total_connections++;
            }
            std::cout << "Total connections: " << total_connections << "\n\n";

            send_response();

            close(m_new_socket);
        } 
    }

    void TcpServer::accept_connection(int& new_socket) {
        /**
        * Sets new_socket to the socket of the incomming request. 
        */
        new_socket = accept(m_socket, (sockaddr*) &m_socket_address, &m_socket_address_len);
        
        if (new_socket < 0) {
            std::ostringstream ss;
            ss << "Server failed to accept incoming connection from ADDRESS: "
               << inet_ntoa(m_socket_address.sin_addr) 
               << "; PORT: " << ntohs(m_socket_address.sin_port);
            exit_with_error(ss.str());
        }
    }
    
    std::string TcpServer::build_response() {
        /**
        * Constructs the HTML response to the accepted connection.
        */
        std::string html_file = "<!DOCTYPE html><html lang=\"en\"><body><h1>Hello from Alfredo!</h1></body></html>";
        std::ostringstream response;
        response << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " 
                 << html_file.size() << "\n\n" << html_file;
        
        return response.str();
    }

    void TcpServer::send_response() {
        /**
        * Writes the built response to the connected socket. 
        */
        long bytes_sent;

        bytes_sent = write(m_new_socket, m_server_message.c_str(), m_server_message.size());

        if (bytes_sent == m_server_message.size()) {
            std::cout << "------ Server response sent to client ------\n\n";
        } else {
            std::cout << "Error sending resonse to client." << std::endl;
        }
    }
    
} // namespace http
