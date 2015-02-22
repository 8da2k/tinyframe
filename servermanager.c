#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "servermanager.h"
#include "inetaddr.h"
#include "connection.h"
#include "debug.h"



/* ����server_manager�������� */
server_manager *create_server_manager()
{
	server_manager *manager = malloc(sizeof(server_manager));
	if (manager == NULL)
	{
		debug_ret("file: %s, line: %d", __FILE__, __LINE__);
		return NULL;
	}

	/* ����epoller���� */
	manager->epoller = create_epoller();
	if (manager->epoller == NULL)
	{
		debug_ret("file: %s, line: %d", __FILE__, __LINE__);
		free(manager);
		return NULL;
	}

	manager->events = NULL;
	manager->actives = NULL;
	
	return manager;
}

/* ��ʼѭ�������¼�,�¼�������������Ϻ���� */
void server_manager_run(server_manager *manager)
{
	int i;
	while (1)
	{
		/* epoll_dispatch() */
		int nfds = manager->epoller->dispatch(manager);
		for (i = 0; i < nfds; i++)
		{
			event *ev = manager->actives;

			/* �ַ��¼� */
			ev->event_handler(ev);

			/* ����������¼���actives�������Ƴ� */
			manager->actives = ev->active_next;
			if (ev->active_next)
				ev->active_next->active_pre = NULL;
			
			//ev->actives = 0;
			ev->active_next = NULL;
			ev->active_pre = NULL;
		}
	}
}


