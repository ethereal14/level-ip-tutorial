#ifndef ETHERNET_H
#define ETHERNET_H
#include <linux/if_ether.h>
#include "syshead.h"

struct eth_hdr {
    uint8_t dmac[6];
    uint8_t smac[6];
    uint16_t ethertype;
    uint8_t payload[];
} __attribute((packed));

struct eth_hdr *init_eth_hdr(char *buf);

#endif // ETHERNET_H