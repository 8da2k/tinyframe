#ifndef __SERVER_H__
#define __SERVER_H__

#include "inetaddr.h"
#include "listener.h"
#include "servermanager.h"
#include "connection.h"

typedef struct listener_t listener;
typedef struct connection_t connection;
typedef struct server_manager_t server_manager;

typedef void (*readable_callback_pt)(connection *conn);
typedef void (*new_conn_callback_pt)(connection *conn);

/* server��,����connection��listener�� */
typedef struct server_t {
	server_manager *manager;
	inet_address *listen_addr;

	/* connection���� */
	connection *connections;

	/* listener�������� */
	listener *listener;

	/* server����Ļص�������connection�����
	 * ͬһserver�µ�connectionӵ����ͬ�Ļص�����
	 * д�ص����������û��趨,connection�ڲ��Զ�����
	 */
	new_conn_callback_pt new_connection_callback;
	readable_callback_pt readable_callback;

}server;

server *create_server(server_manager *manager, inet_address *listen_addr,
		readable_callback_pt read_cb, new_conn_callback_pt new_conn_cb);


#endif

