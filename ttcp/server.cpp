#include <iostream>
#include "socket/tcp_socket.h"
#include "common.h"
int main()
{
    net::TcpSocket sock;
    if (!sock.CreateSocket(kPort))
        printErrorMsg("creat socket");
    sock.Listen();
    std::cout << "wait for connection \n";
    char conn_ip[32];
    UINT conn_port;
    int conn_fd = -1;

    while (true) {
        memset(conn_ip, '\0', 32);

        if ((conn_fd = sock.Accept(conn_ip, conn_port)) < 0) {
            printErrorMsg("accept");
        }
        printf("connect IP: %s ------ Port: %d\n", conn_ip, conn_port);

        net::TcpSocket client(conn_fd);
        SessionInfo session_info = {0, 0};
        client.Receive(&session_info, sizeof(session_info));
        std::cout << "buffer length is " << session_info.length << "  loop number is " << session_info.number
                  << std::endl;
        size_t payload_size = (size_t) session_info.length + sizeof(int32_t);
        PayloadInfo* payload_info = static_cast<PayloadInfo*>(::malloc(payload_size));
        assert(payload_info != NULL);

        ssize_t recv_size = 0;
        int ack = session_info.length;
        ssize_t total_size = 0;
        ssize_t a_recv_size = 0;

        for (int i = 0; i < session_info.number; ++i) {
            if (client.read_n(payload_info, payload_size) != payload_size) {
                printErrorMsg("short read");
            }
            assert(payload_info->length == session_info.length);
            if (client.write_n(&ack, sizeof(ack)) != sizeof(ack)) {
                printErrorMsg("write");
            }
            total_size += payload_size;
        }
        std::cout << "recv total size is " << total_size / (1024 * 1024) << " MB ." << std::endl;
        ::free(payload_info);
        client.closeFd();
    }
    return 0;
}