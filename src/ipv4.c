#include "arp.h"
#include "utils.h"
#include "netdev.h"
#include "ipv4.h"
#include "icmpv4.h"

void ipv4_incoming(struct netdev *netdev, struct eth_hdr *hdr)
{
    struct iphdr *iphdr = (struct iphdr *)hdr->payload;
    uint16_t csum = -1;

    if (iphdr->version != IPV4) {
        perror("Datagram version was not ipv4\n");
        return;
    }

    /* 5：代表5行，每行32位(4字节)，最短ipv4头部就是20字节 */
    if (iphdr->ihl < 5) {
        perror("IPv4 header length must be at least 5\n");
        return;
    }

    if (iphdr->ttl == 0) {
        // TODO:Send ICMP error
        perror("Time to live of datagram reached 0\n");
        return;
    }

    csum = checksum(iphdr, iphdr->ihl * 4);
    if (csum != 0) {
        // Invalid checksum, drop packet handing
        return;
    }

    // TODO: Check fragmentation, possibly reassemble
    iphdr->len = ntohs(iphdr->len);
    switch (iphdr->proto) {
        case ICMPV4:
            icmpv4_incoming(netdev, hdr);
            break;
        default:
            break;
    }
}

void ipv4_outgoing(struct netdev *netdev, struct eth_hdr *hdr)
{
    struct iphdr *iphdr = (struct iphdr *)hdr->payload;
    uint32_t tmpaddr;
    uint16_t csum;
    uint8_t len = iphdr->len;

    /* 交换源ip和目的ip, 现在不查找arp表项 */
    tmpaddr = iphdr->saddr;
    iphdr->daddr = tmpaddr;
    iphdr->saddr = netdev->addr;

    /* 长度使用网络字节序 */
    iphdr->len = htons(iphdr->len);

    /* 计算头部校验值 */
    iphdr->csum = 0;
    csum = checksum(iphdr, iphdr->ihl * 4);
    iphdr->csum = csum;

    netdev_transmit(netdev, hdr, ETH_P_IP, len, hdr->smac);
}