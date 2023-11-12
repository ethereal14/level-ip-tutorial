/**
 * @file arp.h
 * @author your name (you@domain.com)
 * @brief arp头文件
 * arp协议报文如下
 *   14 bytes           | 2 bytes | 2 bytes | 1 byte | 1 byte | 2 bytes | 6 bytes | 4 bytes | 6bytes | 4 bytes | 
 *   Ethernet header    | hwtype  | protype | hwsize | prosize|  opcode | SrcMac  | SrcIP  |  DstMac |  DstIP  |
 * @version 0.1
 * @date 2023-11-08
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef ARP_H
#define ARP_H
#include "ethernet.h"
#include "netdev.h"
#include "syshead.h"

#define ARP_ETHERNET 0x0001
#define ARP_IPV4 0x0800
#define ARP_REQUEST 0x0001
#define ARP_REPLY 0x0002

#define ARP_CACHE_LEN 32
#define ARP_FREE 0
#define ARP_WAITING 1
#define ARP_RESOLVED 2

struct arp_hdr
{
    uint16_t      hwtype;  /* 硬件类型: 以太网的时候为1 */
    uint16_t      protype; /* 协议类型: 要映射的协议地址类型, IPv4 == 0x0800 */
    unsigned char hwsize;  /* 硬件地址长度: Mac地址6字节 */
    unsigned char prosize; /* IP地址长度: IPv4: 4字节 */
    uint16_t      opcode;  /* 操作码: 1 req; 2 ack; 3 rarp req; 4 rarp ack*/
    unsigned char data[]; /* 数据: 发送arp请求或应答arp的Mac地址 + 发送arp或者应答arp的IPv4;
                             发送arp请求或应答arp的Mac地址 + 发送arp或者应答arp的IPv4;*/
} __attribute__((packed)); /* 编译器不要优化结构体对齐的内存排布, 尽量紧凑排布 */

struct arp_ipv4
{
    unsigned char smac[6];
    uint32_t      sip;
    unsigned char dmac[6];
    uint32_t      dip;
} __attribute__((packed));

/**
 * @brief arp表项
 * 包括: 硬件类型(以太网)、源ip、源mac、状态
 */
struct arp_cache_entry
{
    uint16_t      hwtype;
    uint32_t      sip;
    unsigned char smac[6];
    unsigned int  state;
};

void arp_init();
void arp_incoming(struct netdev* netdev, struct eth_hdr* hdr);
void arp_reply(struct netdev* netdev, struct eth_hdr* hdr, struct arp_hdr* arphdr);

#endif   // ARP_H