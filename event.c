#include <sys/epoll.h>
#include <stdlib.h>
#include <time.h>

#include "event.h"
#include "debug.h"
#include "connection.h"

/* �¼�����ʱ�ķַ����� */
static void event_handler(event *ev)
{
	char time_string[40];
	struct tm *ptm;
	ptm = localtime(&ev->time.tv_sec);
	strftime(time_string, sizeof(time_string), "event ready time: %Y-%m-%d %H:%M:%S", ptm);
	debug_msg("%s", time_string);

	if ((ev->actives & EPOLLHUP) && !(ev->actives & EPOLLIN))
	{
		/* �յ�RST�ֽ�,����event_close_callback���� */
		ev->event_close_cb(ev->fd, ev->c_arg);
	}
	if (ev->actives & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
	{
		if (ev->event_read_cb)
			ev->event_read_cb(ev->fd, ev->r_arg);
	}
	if (ev->actives & EPOLLOUT)
	{
		if (ev->event_write_cb)
			ev->event_write_cb(ev->fd, ev->w_arg);
	}
}

static void event_close_callback(int fd, event *ev)
{
	connection_free(ev->conn);
}

/* ����һ���µ��¼� */
event* create_event(server_manager *manager, int fd, short events,
					event_cb_pt read_cb, void *r_arg, event_cb_pt write_cb, void *w_arg)
{
	event *ev;
	ev = malloc(sizeof(event));
	if (ev == NULL)
	{
		debug_ret("file: %s, line: %d", __FILE__, __LINE__);
		return NULL;
	}

	ev->manager = manager;
	ev->fd = fd;
	ev->events = events;
	ev->actives = 0;
	ev->is_listening = 0;
	ev->listen_next = NULL;
	ev->listen_pre = NULL;
	ev->active_next = NULL;
	ev->active_pre = NULL;
	ev->event_handler = event_handler;
	ev->event_read_cb = read_cb;
	ev->event_write_cb = write_cb;
	ev->r_arg = r_arg;
	ev->w_arg = w_arg;
	ev->conn = NULL;

	/* �رմ�����û����ɼ� */
	ev->event_close_cb = event_close_callback;
	ev->c_arg = ev;
	
	return ev;
}

/* ���¼�����epoll������,���¼����ڼ���״̬ */
int event_add(event *ev)
{
	server_manager *manager = ev->manager;

	/* ���¼���ӵ�������server_manager��events����Ŀ�ͷ */
	ev->listen_next = manager->events;
	ev->listen_pre = NULL;
	if (ev->listen_next)
		ev->listen_next->listen_pre = ev;
	manager->events = ev;

	/* ���¼���ӵ�epoll������ */
	manager->epoller->add_event(manager->epoller, ev);

	ev->is_listening = 1;
	return 0;
}

/* �޸��¼�ev
 * �����fd�Ӽ��������Ϊ�������,��ı�ev����Ҫ���ô˺�������
 */
int event_modify(event *ev)
{
	server_manager *manager = ev->manager;

	/* epoll_modify() */
	manager->epoller->mod_event(manager->epoller, ev);
	return 0;
}

/* ��������д�¼� */
void write_event_enable(event *ev)
{
	ev->events |= EPOLLOUT;
	event_modify(ev);
}

/* �رռ���д�¼� */
void write_event_disable(event *ev)
{
	ev->events &= ~EPOLLOUT;
	event_modify(ev);
}


/* ���¼���epoll�������Ƴ� */
void event_remove(event *ev)
{
	server_manager *manager = ev->manager;

	/* epoll_del() */
	manager->epoller->del_event(manager->epoller, ev);

	/* evһ�㲻�������base��actives������,ֻ�账��base��events���� */
	if (ev->listen_pre == NULL)
	{
		manager->events = ev->listen_next;
		if (ev->listen_next != NULL)
			ev->listen_next->listen_pre = NULL;
	}
	else
	{
		event *pre_node = ev->listen_pre;
		pre_node->listen_next = ev->listen_next;
		if (ev->listen_next != NULL)
			ev->listen_next->listen_pre = pre_node;
	}
	
	ev->listen_next = NULL;
	ev->listen_pre = NULL;
	
	/* ev���ڲ������κ�����,�Ҳ���epoll�� */

	ev->is_listening = 0;
}

/* �����¼�,ͬʱclose��Ӧ���ļ������� */
void event_free(event *ev)
{
	event_remove(ev);
	close(ev->fd);
	free(ev);
}

/* ���ļ�������ӳ�䵽event�ṹ��
 * Ŀǰʹ�õ�������˳�����
 */
event *fd_to_event(server_manager *manager, int fd)
{
	event *ev = manager->events;
	while (ev)
	{
		if (fd == ev->fd)
			return ev;
		ev = ev->listen_next;
	}
	return NULL;
}


