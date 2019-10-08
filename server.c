#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(char *message)
{
    perror(message);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, port_num, client_len;
    char buffer[256];
    struct sockaddr_in server_addr, client_addr;
    int n;

    // Improper setup
    if(argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
    }

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0)
    {
        error("ERROR opening socket");
    }

    memset((char *) &server_addr, 0, sizeof(server_addr));

    port_num = atoi(argv[1]);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_num);

    // Bind the socket to an address
    if (bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
        error("ERROR on binding");
    }

    // Listen for connections
    listen(sockfd, 4);
    client_len = sizeof(client_addr);

    // Accept the connection
    newsockfd = accept(sockfd, (struct sockaddr *) &client_addr, &client_len);

    if(newsockfd < 0)
    {
        error("ERROR on accept");
    }

    // Read the data
    memset(buffer, 0, 256);
    
    n = read(newsockfd, buffer, 255);

    if(n < 0)
    {
        error("ERROR reading from socket");
    }

    printf("Message: %s\n", buffer);

    n = write(newsockfd, "Received the message", 20);

    if(n < 0)
    {
        error("ERROR writing to the socket");
    }

    return 0;
}