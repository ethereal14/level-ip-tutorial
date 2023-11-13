#include "arp.h"
#include "netdev.h"

static struct arp_cache_entry arp_cache[ARP_CACHE_LEN];

/**
 * @brief 更新arp表项
 * 遍历arp表项, 发现有ipv4相等的就更新mac地址, 没有就不更新
 * @param hdr
 * @param data
 * @return int
 */
static int update_arp_translation_table(struct arp_hdr *hdr, struct arp_ipv4 *data)
{
    struct arp_cache_entry *entry;
    for (int i = 0; i < ARP_CACHE_LEN; i++) {
        entry = &arp_cache[i];

        if (entry->state == ARP_FREE) {
            continue;
        }
        if (entry->hwtype == hdr->hwtype && entry->sip == data->sip) {
            memcpy(entry->smac, data->smac, 6);
            return 1;
        }
    }

    return 0;
}

/**
 * @brief 插入一条新的arp表项
 *
 * @param hdr
 * @param data
 * @return int
 */
static int insert_arp_translation_table(struct arp_hdr *hdr, struct arp_ipv4 *data)
{
    struct arp_cache_entry *entry;
    for (int i = 0; i < ARP_CACHE_LEN; i++) {
        entry = &arp_cache[i];

        if (entry->state == ARP_FREE) {
            entry->state = ARP_RESOLVED;

            entry->hwtype = hdr->hwtype;
            entry->sip = data->sip;
            memcpy(entry->smac, data->smac, sizeof(entry->smac));

            return 0;
        }
    }
    return -1;
}

void arp_init()
{
    memset(arp_cache, 0, ARP_CACHE_LEN * sizeof(struct arp_cache_entry));
}

void arp_incoming(struct netdev *netdev, struct eth_hdr *hdr)
{
    struct arp_hdr *arphdr;
    struct arp_ipv4 *arpdata;
    int merge = 0;

    arphdr = (struct arp_hdr *)hdr->payload;

    arphdr->hwtype = ntohs(arphdr->hwtype);
    arphdr->protype = ntohs(arphdr->protype);
    arphdr->opcode = ntohs(arphdr->opcode);

    if (arphdr->hwtype != ARP_ETHERNET) {
        printf("Unsupport HW type!\n");
        return;
    }

    if (arphdr->protype != ARP_IPV4) {
        printf("Unsupport protocol\n");
        return;
    }

    arpdata = (struct arp_ipv4 *)arphdr->data;

    merge = update_arp_translation_table(arphdr, arpdata);

    if (netdev->addr != arpdata->dip) {
        printf("ARP was not for us!\n");
        return;
    }

    if (!merge && insert_arp_translation_table(arphdr, arpdata) != 0) {
        perror("ERROR: No free space in ARP translation table\n");
    }

    switch (arphdr->opcode) {
        case ARP_REQUEST:
            arp_reply(netdev, hdr, arphdr);
            break;
        default:
            printf("Opcode not supported\n");
    }
}

void arp_reply(struct netdev *netdev, struct eth_hdr *hdr, struct arp_hdr *arphdr)
{
    struct arp_ipv4 *arpdata;
    int len;

    arpdata = (struct arp_ipv4 *)arphdr->data;

    memcpy(arpdata->dmac, arpdata->smac, 6);
    arpdata->dip = arpdata->sip;
    memcpy(arpdata->smac, netdev->hwaddr, 6);
    arpdata->sip = netdev->addr;

    arphdr->opcode = ARP_REPLY;

    arphdr->opcode = htons(arphdr->opcode);
    arphdr->hwtype = htons(arphdr->hwtype);
    arphdr->protype = htons(arphdr->protype);

    len = sizeof(struct arp_hdr) + sizeof(struct arp_ipv4);
    netdev_transmit(netdev, hdr, ETH_P_ARP, len, arpdata->dmac);
}