
/* HTTP������ѹ������ */

#include <stdio.h>

#include "servermanager.h"

void request_handler(connection *conn)
{
	static char buf[] = "HTTP/1.1 200 0K\r\nConnection:keep-alive\r\nContent-length:4\r\n\r\n"
						"test";

	//static char buf[] = "HTTP/1.0 200 OK\r\n";
	
	/* ����HTTP��Ӧ */
	connection_send(conn, buf, sizeof(buf) - 1);

	/* ������뻺���� */
	array_clear(conn->input_buffer);

	//connection_free(conn);
}

int main()
{
	server_manager *manager = server_manager_create();
	inet_address addr = addr_create("any", 2016);
	listener_create(manager, addr, request_handler, NULL);
	server_manager_run(manager);

	return 0;
}


