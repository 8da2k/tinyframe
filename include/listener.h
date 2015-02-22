#ifndef __LISTENER_H__
#define __LISTENER_H__

#include <sys/types.h>
#include <sys/socket.h>

#include "event.h"
#include "server.h"
#include "inetaddr.h"

typedef struct listener_t listener;
typedef struct server_t server;
typedef struct event_t event;

/*
 * �����ӵ���ʱ���û��ص�����
 * @sockfd : �������׽���������
 * @sock_addr : �ͻ��˵�ַ
 */
typedef void (*accept_cb_pt)(int connfd, listener *ls);

/* listener��,��������˿� */
typedef struct listener_t {
	int listen_fd;				/* ���������� */
	server *server;				/* listener������event_base */	

	/* �����׽��ֵ�ַ */
	inet_address *listen_addr;
	
	/* �ͻ��˵�ַ,�õ�ַӦ��Ѹ�ٴ���connection���� */
	inet_address client_addr;	

	
	event *ls_event;			/* �����¼� */
	accept_cb_pt accept_cb;		/* �����ӵ���ʱ�Ļص� */
}listener;


listener *create_listener(server *srv, accept_cb_pt callback, inet_address *ls_addr);
void listener_free(listener *ls);


#endif

