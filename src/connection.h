#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include "event.h"
#include "inetaddr.h"
#include "server.h"
#include "array.h"

typedef struct connection_t connection;
typedef struct server_t server;
typedef struct event_t event;

/* connection��,����socket�ԵĶ�д */
typedef struct connection_t {
	int fd;
	event *conn_event;	/* ���Ӷ�Ӧ���¼��ṹ�� */
	server *server;		/* connection����server */
	
	inet_address server_addr;
	inet_address client_addr;
	
	//connection *prev;
	//connection *next;

	/* ���Ӷ�Ӧ������buffer */
	array *input_buffer;

	/* ���Ӷ�Ӧ�����buffer */
	array *output_buffer;

	int closing;	/* ���������ڱ��Զ˹ر� */
}connection;

connection *connection_create(int conn_fd, server *server);
void connection_free(connection *conn);
void connection_send(connection *conn, char *buf, size_t len);

#endif

