#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>

#include "inetaddr.h"
#include "debug.h"

/* ����һ����ַ(IP + port)
 * ����: create_addr("192.168.1.2", 1234) 	�ض�IP��ַ
 * 		 create_addr("any", 1234)			����IP��ַ
 */
inet_address addr_create(const char *ip, int port)
{
	if (ip == NULL)
		debug_quit("file: %s, line: %d", __FILE__, __LINE__);
	
	inet_address new_addr;

	bzero(&new_addr.addr, sizeof(new_addr.addr));
	new_addr.addr.sin_family = AF_INET;
	
	if (strcmp(ip, "any") == 0)
		new_addr.addr.sin_addr.s_addr = htonl(INADDR_ANY);
	else if(inet_pton(AF_INET, ip, &new_addr.addr.sin_addr) <= 0)
		debug_quit("file: %s, line: %d", __FILE__, __LINE__);
	
	new_addr.addr.sin_port = htons(port);
	return new_addr;
}


