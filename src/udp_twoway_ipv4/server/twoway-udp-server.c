#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <errno.h>

#define BUFFERSIZE  100
#define TRUE        1
#define STD_CONFIG  0

int main (int argc, char * argv[]) 
{
    /** message buffer */
    char buffer[BUFFERSIZE]; 

    /** socket file descriptor */
    int sockfd;

    /** socket structs */
    struct sockaddr_in local, remote; 

    /** listening port */
    char* listening_port; 

    /** command status */
    int command_status; 

    /** remote struct len */
    socklen_t socklen = sizeof(struct sockaddr_in);

    /** bytes read from remote */
    int msg_length; 

    if (argc < 2) {
        printf("[usage] %s <listening port> \n", argv[0]); 
        exit(-1); 
    } 
    else listening_port = argv[1]; 

    /** opening socket & getting file descriptor */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); 

    if (sockfd < 0) {
        perror("[error] "); 
        exit(-1);
    }

    memset((char *) &remote, 0, sizeof( remote ));
    memset((char *) &local, 0, sizeof( local ));
    local.sin_family = AF_INET; 
    local.sin_port = htons(atoi(listening_port));  

    /** binding the socket */ 
    command_status = bind(sockfd, (struct sockaddr *) &local, sizeof(local)); 

    if (command_status < 0) {
        perror("[error] "); 
        exit(-1);
    }

    while (TRUE) {

        /** clear the buffer */
        memset(buffer, 0, sizeof(buffer)); 

        msg_length = recvfrom(
            sockfd, 
            buffer, 
            BUFFERSIZE, 
            STD_CONFIG, 
            (struct sockaddr *) &remote, 
            &socklen
        ); 

        printf(
            "[%s:%d => local:%s] %s \n", 
            inet_ntoa(remote.sin_addr), 
            ntohs(remote.sin_port), 
            listening_port, 
            buffer
        ); 

        sendto(
            sockfd,
            buffer, 
            msg_length, 
            STD_CONFIG, 
            (struct sockaddr *) &remote, 
            socklen
        ); 
    }
}