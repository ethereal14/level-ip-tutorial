#include "basic.h"
#include "syshead.h"
#include "utils.h"

static int tun_fd;

static int set_if_route(char* dev, char* cidr)
{
    return run_cmd("ip route add dev %s %s", dev, cidr);
}

static int set_if_up(char* dev)
{
    return run_cmd("ip link set dev %s up", dev);
}

static int tun_alloc(char* dev)
{
    struct ifreq ifr;
    int          fd, err;

    if ((fd = open("/dev/net/tap", O_RDWR)) < 0) {
        print_error("Cannot open TUN/TAP dev");
        exit(1);
    }

    CLEAR(ifr);

    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
    if (*dev) { strncpy(ifr.ifr_name, dev, IFNAMSIZ); }

    if ((err = ioctl(fd, TUNSETIFF, (void*)&ifr)) < 0) {
        print_error("Err:Could not ioctl run: %s\n", strerror(errno));
        close(fd);
        return err;
    }

    strcpy(dev, ifr.ifr_name);

    return fd;
}

int tun_read(char* buf, int len)
{
    return read(tun_fd, buf, len);
}

int tun_write(char* buf, int len)
{
    return write(tun_fd, buf, len);
}

void tun_init(char* dev)
{
    tun_fd = tun_alloc(dev);

    if (set_if_up(dev) != 0) { print_error("ERROR when setting up if\n"); }

    if (set_if_route(dev, "10.0.0.0/24") != 0) { print_error("ERROR when setting route for if\n"); }
}