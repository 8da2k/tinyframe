#ifndef __EVENTBASE_H__
#define __EVENTBASE_H__

#include "epoll.h"
#include "listener.h"
#include "event.h"
#include "hash.h"
#include "array.h"
#include "heap.h"

typedef struct heap_t heap;
typedef struct timer_t timer;

struct server_manager_t {

	/* ��װ���¼��������� */
	int epoll_fd;

	/* ��ʱ���õ�С���� */
	heap *timers;

	/* �ѳ�ʱtimer���� */
	//timer *timeout_timers;
};

server_manager *server_manager_create();
void server_manager_run(server_manager *manager);
void print_running_events(server_manager *manager);

#endif

