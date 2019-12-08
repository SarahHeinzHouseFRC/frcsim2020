/**
 * Copyright (c) 2019 FRC Team 3260
 */

#include "UdpNode.h"

#define MAXLINE 1024


UdpNode::UdpNode(uint16_t rxport, uint16_t txport) : _rxport(rxport), _txport(txport)
{
    // Creating socket file descriptor
    if ((_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&_servaddr, 0, sizeof(_servaddr));
    memset(&_cliaddr, 0, sizeof(_cliaddr));

    // Fill out server information
    _servaddr.sin_family = AF_INET; // IPv4
    _servaddr.sin_addr.s_addr = INADDR_ANY;
    _servaddr.sin_port = htons(_rxport);

    // Bind the socket with the server address
    if (bind(_sockfd, (const struct sockaddr *)&_servaddr,
    sizeof(_servaddr)) < 0)
    {
        perror("UdpNode: Bind failed");
        exit(EXIT_FAILURE);
    }
}



UdpNode::~UdpNode()
{
    close(_sockfd);
}



void UdpNode::send(std::string msg)
{
    _cliaddr.sin_family = AF_INET;
    _cliaddr.sin_port = htons(_txport);
    _cliaddr.sin_addr.s_addr = INADDR_ANY;

    int n, len;
    const char *msg_c = msg.c_str();
    sendto(_sockfd, (const char *)msg_c, strlen(msg_c),
           MSG_CONFIRM, (const struct sockaddr *) &_cliaddr,
           sizeof(_cliaddr));
}



std::string UdpNode::receive()
{
    struct sockaddr_in cliaddr;
    memset(&cliaddr, 0, sizeof(cliaddr));
    char buffer[MAXLINE];
    int len, n;
    n = recvfrom(_sockfd, (char *) buffer, MAXLINE, MSG_DONTWAIT, ( struct sockaddr *) &cliaddr, (socklen_t*) &len);
    buffer[n] = '\0';
    return buffer;
}
