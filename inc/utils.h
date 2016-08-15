#pragma once

#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <netpacket/packet.h>
#include <arpa/inet.h>

typedef enum bpdu_types {
    TCN,
    CNF
} bpdu_type

int create_socket(char *device, int *sockfd, unsigned char *mac){
    int sock;
    struct ifreq ifr;
    struct sockaddr_ll sll;
    memset(&ifr, 0, sizeof(ifr));
    memset(&sll, 0, sizeof(sll));

    sock = socket (PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if(sock == 0) { printf("ERR: socket creation for device: %s\n", device); return -1; }
    strncpy(ifr.ifr_name, device, sizeof(ifr.ifr_name));
    if(ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
        memcpy(mac, ifr.ifr_hwaddr.sa_data, 6);
    }else{
        perror("error on MAC address read");
        return -1;
    }
    if(ioctl(sock, SIOCGIFINDEX, &ifr) == -1) { printf(" ERR: ioctl failed for device: %s\n", device); return -1; }

    sll.sll_family      = AF_PACKET;
    sll.sll_ifindex     = ifr.ifr_ifindex;
    sll.sll_protocol    = htons(ETH_P_ALL);
    if(bind(sock, (struct sockaddr *) &sll, sizeof(sll)) == -1) { printf("ERR: bind failed for device: %s\n", device); return -1; }
    *sockfd = sock;
    return 0;
}

void generateSTP(unsigned char *packet, bpdu_type btype, int tc, int ta, short port_id, int root_path_cost, short message_age, short max_age, short hello_time, short forward_delay, int padding){
    unsigned char dst[6] = { 0x01, 0x80, 0xc2, 0x00, 0x00, 0x00 };
    unsigned char len[2] = { 0x00, 0x26 };
    //logical link control
    unsigned char llc[3] = { 0x42, 0x42, 0x03 };
    //stp
    //protocol identifier
    unsigned char pid[2] = { 0x00, 0x00 };
    //version identifier
    unsigned char vid[1] = { 0x00 };

    //prepare variables for network byte order
    int netPath = htonl(root_path_cost);
    short netAge = htons(message_age);
    short netMax = htons(max_age);
    short netHlt = htons(hello_time);
    short netFwd = htons(forward_delay);

    unsigned char *ptrs[] = {dst, bridgeId, len, llc, pid, vid, btype, bflags, &rootPriority, &rootExtension, root, (unsigned char *) &netPath, &priority, &extension, bridgeId, port, (unsigned char *) &netAge, (unsigned char *) &netMax, (unsigned char *) &netHlt, (unsigned char *) &netFwd};
    int sizes[] = {6, 6, 2, 3, 2, 1, 1, 1, 1, 1, 6, 4, 1, 1, 6, 2, 2, 2, 2, 2};
    int offset = 0;
    for(int i=0; i<sizeof(ptrs)/sizeof(unsigned char*); i++){
        memcpy(packet+offset, ptrs[i], sizes[i]);
        offset += sizes[i];
    }
}


