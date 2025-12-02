// receiver.c - UDP receiver for random numbers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345 // Use the same port as the sender

int main() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sock);
        return 1;
    }
    char buf[128];
    printf("Listening for random numbers on UDP port %d...\n", PORT);
    while (1) {
        int len = recv(sock, buf, sizeof(buf)-1, 0);
        if (len > 0) {
            buf[len] = 0;
            printf("Received: %s\n", buf);
        }
    }
    close(sock);
    return 0;
}
