/**
 * @file ipv4.h
 * @author your name (you@domain.com)
 * @brief ipv4头文件
 * ipv4协议报文如下
 *      20 bytes
 *              0 ------- 4 ------- 8 -------- 16 ----- 19 ----- 24 -------- 31
 *              ———————————————————————————————————————————————————————————————
 *              | version |   ihl   |    tos    |             len             |
 *              ———————————————————————————————————————————————————————————————
 *              |               id              | flags |     frag_offset     |
 *              ———————————————————————————————————————————————————————————————
 *              |        ttl        |   proto   |             csum            |
 *              ———————————————————————————————————————————————————————————————
 *              |                            saddr                            |
 *              ———————————————————————————————————————————————————————————————
 *              |                            daddr                            |
 *              ———————————————————————————————————————————————————————————————
 *              |                           选项字段                           |
 *              ———————————————————————————————————————————————————————————————
 *              |                             data                            |
 *              ———————————————————————————————————————————————————————————————
 *
 * @version 0.1
 * @date 2023-11-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef IPV4_H
#define IPV4_H

#include "syshead.h"
#include "netdev.h"

#define IPV4 0x04
#define ICMPV4 0x01

struct iphdr {
    uint8_t version : 4;       /* ip协议的版本号。4bit */
    uint8_t ihl : 4;           /* ipv4的首部长度。4bit */
    uint8_t tos;               /* 区分服务。8bit */
    uint16_t len;              /* 总长度：首部长度和数据之和长度。16bit */
    uint16_t id;               /* 标识，ipv4维护的一个计数器，没产生一个数据报，计数器就加1，并将该值赋给标识字段。16bit */
    uint16_t flags : 3;        /* 标志，最低位为1表示后边还有分片的数据包，为0表示这是最后一个分片数据报；中间一位为1表示不能分片，为1表示分片。3bit */
    uint16_t frag_offset : 13; /* 片位移，表示片段在数据报中的位置。第一个数据报将该字段设置为1。13bit */
    uint8_t ttl;               /* 生存时间，表示数据报在网络中的寿命，先是跳数。8bit */
    uint8_t proto;             /* 协议，表示携带的数据是哪种协议。8bit */
    uint16_t csum;             /* 首部校验和。16bit */
    uint32_t saddr;            /* 源ip。32bit */
    uint32_t daddr;            /* 目的ip。32bit */
    uint8_t data[];            /* 数据。32bit */
} __attribute__((packed));

void ipv4_incoming(struct netdev *netdev, struct eth_hdr *hdr);
void ipv4_outgoing(struct netdev *netdev, struct eth_hdr *hdr);

#endif // IPV4_H