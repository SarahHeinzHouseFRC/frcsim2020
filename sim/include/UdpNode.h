/**
 * Copyright (c) 2019 FRC Team 3260
 */

#ifndef ROBOT_SIM_UDPNODE_H
#define ROBOT_SIM_UDPNODE_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


class UdpNode
{
public:
    /**
     * Constructor
     */
    UdpNode(uint16_t rx_port, uint16_t tx_port);

    ~UdpNode();

    void send(std::string msg);

    std::string receive();

private:
    int _sockfd;
    int _rxport, _txport;
    struct sockaddr_in _servaddr, _cliaddr;
};


#endif //ROBOT_SIM_UDPNODE_H
