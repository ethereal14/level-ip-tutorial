#ifndef TUNTAP_IF_H
#define TUNTAP_IF_H

int tun_write(char* buf, int len);
int tun_read(char* buf, int len);
void tun_init(char* dev);

#endif // TUNTAP_IF_H