#include "http_tcpServer_linux.h"

int main() {
    using namespace http;

    TcpServer server = TcpServer("0.0.0.0", 8080);
    server.start_listen();
        
    return 0;
}
