#include "arp.h"
#include "basic.h"
#include "ethernet.h"
#include "netdev.h"
#include "syshead.h"
#include "tuntap_if.h"
#include "utils.h"

#define BUFLEN 100

void handl_frame(struct netdev* netdev, struct eth_hdr* hdr)
{
    switch (hdr->ethertype) {
    case ETH_P_ARP:
        printf("arp get\n");
        arp_incoming(netdev, hdr);
        break;

    case ETH_P_IP: printf("Found IPv4\n"); break;

    default: printf("Unrecongnized ethertype %x\n", hdr->ethertype); break;
    }
}

int main(int argc, char** argv)
{
    char          buf[BUFLEN];
    char*         dev = calloc(10, 1);
    struct netdev netdev;

    CLEAR(buf);

    tun_init(dev);
    netdev_init(&netdev, "10.0.0.5", "00:0c:29:6d:50:14");

    printf("dev is %s\n", dev);

    arp_init();

    while (1) {
        if (tun_read(buf, BUFLEN) < 0) {
            // print_error("ERROR: Read from tun_fd: %s\n", strerror(errno));
            continue;
        }
        // print_heaxdump(buf, BUFLEN);

        struct eth_hdr* hdr = init_eth_hdr(buf);

        handl_frame(&netdev, hdr);
    }

    return 0;
}
