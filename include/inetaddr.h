#ifndef __INETADDR_H__
#define __INETADDR_H__

#include <netinet/in.h>

/* ��װ��IPv4��ַ��port */
typedef struct inet_addr_t {
	struct sockaddr_in addr;
}inet_address;


inet_address *create_addr(const char *ip, int port);

#endif

