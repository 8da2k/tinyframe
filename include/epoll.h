#ifndef __EPOLL_H__
#define __EPOLL_H__

#include "servermanager.h"
#include "event.h"

typedef struct epoller_t epoller;
typedef struct event_t event;
typedef struct server_manager_t server_manager;

/* ����¼���������,Ŀǰֻʵ��epoll */
typedef struct epoller_t {
	const char *name;
	int fd;			/* epoll�ļ������� */
	int (*add_event)(epoller *ep, event *ev);
	int (*del_event)(epoller *ep, event *ev);
	int (*mod_event)(epoller *ep, event *ev);
	int (*dispatch)(server_manager *manager);
}epoller;

/* ��������¼��������� */
epoller* create_epoller();

/* ���ٺ���¼��������� */
void epoller_free(epoller *ep);

#endif

