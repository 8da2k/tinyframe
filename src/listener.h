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

typedef void (*accept_callback_pt)(int connfd, listener *ls);

/* listener��,��server�������,��������˿�,�û����ɼ� */
typedef struct listener_t {

	/* listener������event_base */
	server *server;

	/* �����׽��ֵ�ַ */
	inet_address listen_addr;
	
	/* �ͻ��˵�ַ,�õ�ַӦ��Ѹ�ٴ���connection���� */
	inet_address client_addr;	

	/* �����¼� */
	event *ls_event;

	/* �����ӵ���ʱ�Ļص� */
	accept_callback_pt accept_callback;
}listener;


listener *listener_create(server *srv, accept_callback_pt callback, inet_address ls_addr);
void listener_free(listener *ls);

#endif

