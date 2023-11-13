#include <stdarg.h>
#include <stdio.h>
#include "syshead.h"
#include "utils.h"

int run_cmd(char *cmd, ...)
{
    va_list ap;
    char buf[CMDBUILD];
    va_start(ap, cmd);
    vsnprintf(buf, CMDBUILD, cmd, ap);

    va_end(ap);

    printf("%s\n", buf);

    return system(buf);
}

void print_heaxdump(char *str, int len)
{
    printf("Printing hexdump:\n");
    for (int i = 0; i < len; i++) {
        if (i % 8 == 0)
            printf("\n");
        printf("%02x ", (unsigned char)str[i]);
    }
    printf("\n");
}

void print_error(char *str, ...)
{
    va_list ap;
    char buf[200];
    va_start(ap, str);
    vsnprintf(buf, 200, str, ap);

    va_end(ap);

    perror(buf);
}

/**
 * @brief 计算校验和
 * The checksum field is the 16 bit one’s complement of the one’s complement sum of all 16 bit words
 * in the header. For purposes of computing the checksum, the value of the checksum field is zero.
 * 中文来说: 发送方首先将校验字段置零，然后将ip头部的字节以16bit为单位两两相加, 若计算结果大于0xFFFF(即发生进位),
 * 则将高16位加到低16位上(循环操作), 然后取反，得到一个值并写入校验字段
 * 如果ip头部在传输过程中没有发生改变，那么接收方收到的ip头部的字节按照16bit相加, 大于0xFFFF时低16位+高16位(循环操作), 最后得到0xFFFF
 * @param addr
 * @param count
 * @return uint16_t
 */
uint16_t checksum(void *addr, int count)
{
    /* Compute Internet Checksum for "count" bytes
     *          beginning at location "addr".
     * Taken from rfc1071
     */

    register uint32_t sum = 0;
    uint16_t *ptr = addr;

    while (count > 1) {
        /* 按照16位为单位, 循环相加 */
        sum += *ptr++;
        count -= 2;
    }

    /* 奇数字节时(但是乘4放进来count不为奇数啊) */
    if (count > 0)
        sum += *(uint8_t *)ptr;

    /* 判断sum值是否大于0xFFFF。大于就执行右移再加上低16位, 循环执行 */
    while (sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }

    return ~sum;
}