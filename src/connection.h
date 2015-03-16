#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include "event.h"
#include "inetaddr.h"
#include "event_loop.h"
#include "array.h"

typedef struct server_manager_t server_manager;
typedef struct connection_t connection;
typedef struct event_loop_t event_loop;
typedef struct event_t event;

typedef void (*connection_callback_pt)(connection *conn);

/* connection��,����socket�ԵĶ�д */
struct connection_t {
	int fd;

	/* ���Ӷ�Ӧ���¼��ṹ�� */
	event *conn_event;
	
	/* ���Ӷ�Ӧ������buffer */
	array *input_buffer;

	/* ���Ӷ�Ӧ�����buffer */
	array *output_buffer;

	//inet_address server_addr;
	//inet_address client_addr;

	connection_callback_pt readable_callback;
};

connection *connection_create(event_loop *loop, int connfd, connection_callback_pt read_cb);
void connection_free(connection *conn);
void connection_send(connection *conn, char *buf, size_t len);

#endif

