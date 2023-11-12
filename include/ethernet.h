#ifndef ETHERNET_H
#define ETHERNET_H
#include "syshead.h"
#include <linux/if_ether.h>
struct eth_hdr
{
    unsigned char dmac[6];
    unsigned char smac[6];
    uint16_t      ethertype;
    unsigned char payload[];
} __attribute((packed));

struct eth_hdr* init_eth_hdr(char* buf);

#endif   // ETHERNET_H