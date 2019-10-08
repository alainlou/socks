#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char *message)
{
    perror(message);
    exit(1);
}

int main(int argc, char *argv[]) 
{
    int sockfd, port_num, n;
    struct sockaddr_in server_addr;
    struct hostent *server;

    char buffer[256];

    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    port_num = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0)
    {
        error("ERROR opening socket");
    }

    server = gethostbyname(argv[1]);

    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    memset((char *) &server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;

    memcpy((char *)server->h_addr_list[0], (char *)&server_addr.sin_addr.s_addr, server->h_length);

    server_addr.sin_port = htons(port_num);

    if(connect(sockfd, &server_addr, sizeof(server_addr)) < 0)
    {
        error("ERROR connecting");
    }

    printf("Enter your message: ");
    memset(buffer, 0, 256);
    fgets(buffer, 255, stdin);
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
    {
        error("ERROR writing to the socket");
    }
    memset(buffer, 0 , 256);
    n = read(sockfd, buffer, 266);
    if(n < 0)
    {
        error("ERROR on read");
    }
    printf("%s\n", buffer);

    return 0;
}