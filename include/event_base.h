#ifndef __EVENTBASE_H__
#define __EVENTBASE_H__

#include "epoll.h"
#include "queue.h"

typedef struct backend_t backend;
typedef struct event_list_t event_list;

typedef struct event_base_t {
	/* �¼��������� */
	backend *epoller;

	/* �����¼����� */
	event_list *actives;
}event_base;

event_base *create_event_base();
void event_base_run(event_base *base);

#endif
