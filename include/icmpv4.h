#ifndef ICMPV4_H
#define ICMPV4_H

#include "syshead.h"
#include "netdev.h"

#define ICMP_V4_REPLY 0x00           /* 回送响应 */
#define ICMP_V4_DST_UNREACHABLE 0x03 /* 目的不可达 */
#define ICMP_V4_SRC_QUENCH 0x04      /* 源拥塞 */
#define ICMP_V4_REDIRECT 0x05        /* 重定向 */
#define ICMP_V4_ECHO 0x08            /* 回送 */
#define ICMP_V4_ROUTER_ADV 0x09      /* 路由通告 */
#define ICMP_V4_ROUTER_SOL 0x0a      /* 路由选择 */
#define ICMP_V4_TIMEOUT 0x0b         /* 超时 */
#define ICMP_V4_MALFORMED 0x0c       /*  */

struct icmp_v4 {
    uint8_t type;   /* 消息类型 */
    uint8_t code;   /* 进一步描述消息类型含义 */
    uint16_t csum;  /* 校验和: icmpv4中校验和是端到端的, 计算校验时包含有效负载 */
    uint8_t data[]; /* 负载 */
} __attribute__((packed));

struct icmp_v4_echo {
    uint16_t id;    /* 发送主机设置,已确定回显应答的目标进程(tid设置进来) */
    uint16_t seq;   /* 回显序列号, 从0开始, 每当有echo数据就加1. 用于检测echo消息在传输中是否消失或重新排序 */
    uint8_t data[]; /* 可选字段, 通常包含回显的时间戳等信息, 可以估计主机之间的往返时间 */
} __attribute__((packed));

struct icmp_v4_unreachable {
    uint8_t unused; /* 未使用字段 */
    uint8_t len;    /* 表示原始报文的长度, 对于ipv4, 以4字节为单位 */
    uint16_t var;   /* var的值取决于ICMP代码 */
    uint8_t data[]; /* 尽可能多的将导致"目标违法到达"状态的原始ip数据包放入data字段 */
} __attribute__((packed));

void icmpv4_incoming(struct netdev *netdev, struct eth_hdr *hdr);
void icmpv4_reply(struct netdev *netdev, struct eth_hdr *hdr);

#endif // ICMPV4_H