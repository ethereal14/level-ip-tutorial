#ifndef NETDEV_H
#define NETDEV_H

#include "ethernet.h"
#include "syshead.h"

struct netdev {
    uint32_t addr;
    uint8_t hwaddr[6];
};

void netdev_init(struct netdev *dev, char *addr, char *hwaddr);
void netdev_transmit(struct netdev *dev, struct eth_hdr *hdr, uint16_t ethertype, int len, uint8_t *dst);
#endif // NETDEV_H