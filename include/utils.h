#ifndef UTILS_H
#define UTILS_H

#define CMDBUILD 100

int run_cmd(char *cmd, ...);
void print_heaxdump(char *str, int len);
void print_error(char *str, ...);
uint16_t checksum(void *addr, int count);

#endif // UTILS_H