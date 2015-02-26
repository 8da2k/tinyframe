#ifndef __SERVER_H__
#define __SERVER_H__

#include "inetaddr.h"
#include "listener.h"
#include "servermanager.h"
#include "connection.h"
#include "hash.h"

typedef struct listener_t listener;
typedef struct connection_t connection;
typedef struct server_manager_t server_manager;

typedef void (*connection_callback_pt)(connection *conn);

/* server����,����connection�����listener���� */
typedef struct server_t {
	server_manager *manager;

	/* ��server�����������connection���� */
	//connection *connections;
	hash_table *connections;

	/* listener�������� */
	listener *listener;

	/* server����Ļص�������connection�����
	 * ͬһserver�µ�connectionӵ����ͬ�Ļص�����
	 * д�ص����������û��趨,connection�ڲ��Զ�����
	 */
	connection_callback_pt new_connection_callback;
	connection_callback_pt readable_callback;

}server;

server *server_create(server_manager *manager, inet_address listen_addr,
		connection_callback_pt read_cb, connection_callback_pt new_conn_cb);


#endif

