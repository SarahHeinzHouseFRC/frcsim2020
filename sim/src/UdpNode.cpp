/**
 * Copyright (c) 2020 Team 3260
 */

#include "UdpNode.h"

#define MAXLINE 1024


UdpNode::UdpNode(uint16_t rxPort, const std::string& txIp, uint16_t txPort) : _rxAddr{0}, _txAddr{0}
{
    // Creating socket file descriptor
    if ((_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Fill out server information
    memset(&_rxAddr, 0, sizeof(_rxAddr));
    _rxAddr.sin_family = AF_INET; // IPv4
    _rxAddr.sin_addr.s_addr = INADDR_ANY;
    _rxAddr.sin_port = htons(rxPort);

    // Fill out client information
    memset(&_txAddr, 0, sizeof(_txAddr));
    _txAddr.sin_family = AF_INET; // IPv4
    _txAddr.sin_addr.s_addr = inet_addr(txIp.c_str());
    _txAddr.sin_port = htons(txPort);

    // Bind the socket with the server address
    if (bind(_sockfd, (const struct sockaddr *)&_rxAddr,
    sizeof(_rxAddr)) < 0)
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
    const char *msg_c = msg.c_str();
    sendto(_sockfd, (const char *)msg_c, strlen(msg_c),
           MSG_CONFIRM, (const struct sockaddr *) &_txAddr,
           sizeof(_txAddr));
}



std::string UdpNode::receive()
{
    struct sockaddr_in cliaddr;
    memset(&cliaddr, 0, sizeof(cliaddr));
    char buffer[MAXLINE];
    int len = sizeof(struct sockaddr_in);
    int n = recvfrom(_sockfd, (char *) buffer, MAXLINE, MSG_DONTWAIT, ( struct sockaddr *) &cliaddr, (socklen_t*) &len);
    if (cliaddr.sin_port == _txAddr.sin_port)
    {
        if (n >= MAXLINE)
        {
            printf("UdpNode: Large message received, buffer overflow!\n");
        }
        buffer[n] = '\0';
        return buffer;
    }
    else
    {
        return "";
    }
}
