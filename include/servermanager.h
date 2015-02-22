#ifndef __EVENTBASE_H__
#define __EVENTBASE_H__

#include "epoll.h"
#include "list.h"
#include "listener.h"
#include "event.h"

typedef struct epoller_t epoller;

/* server manager��,����server�� */
typedef struct server_manager_t {
	/* �¼��������� */
	epoller *epoller;

	/* �����¼����� */
	event *events;

	/* �����¼����� */
	event *actives;

}server_manager;

server_manager *create_server_manager();
void server_manager_run(server_manager *manager);

#endif
