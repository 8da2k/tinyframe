#include <sys/epoll.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#include "event.h"
#include "debug.h"
#include "connection.h"

/* �¼�����ʱ�ķַ����� */
void event_handler(event *ev)
{
	//char time_string[40];
	//struct tm *ptm;
	//ptm = localtime(&ev->time.tv_sec);
	//strftime(time_string, sizeof(time_string), "event ready time: %Y-%m-%d %H:%M:%S", ptm);
	//debug_msg("%s", time_string);

	if (ev->actives & EPOLLRDHUP)
	{
		/* �Զ˹ر������� */
		connection_free(ev->conn);
	}
	if (ev->actives & (EPOLLIN | EPOLLPRI))
	{
		if (ev->event_read_handler)
			ev->event_read_handler(ev->fd, ev, ev->r_arg);
	}
	if (ev->actives & EPOLLOUT)
	{
		if (ev->event_write_handler)
			ev->event_write_handler(ev->fd, ev, ev->w_arg);
	}
}

event* event_create(int fd, short events, event_callback_pt read_cb,
					void *r_arg, event_callback_pt write_cb, void *w_arg)
{
	event *ev = malloc(sizeof(event));
	if (ev == NULL)
	{
		debug_ret("file: %s, line: %d", __FILE__, __LINE__);
		return NULL;
	}

	ev->fd = fd;
	ev->events = events;
	ev->actives = 0;
	ev->is_listening = 0;
	ev->conn = NULL;
	
	ev->event_read_handler = read_cb;
	ev->r_arg = r_arg;
	
	ev->event_write_handler = write_cb;
	ev->w_arg = w_arg;

	return ev;
}

void io_event_add(event_loop *loop, event *ev)
{
	ev->type = IO_EVENT;
	ev->belong.loop = loop;
}

void accept_event_add(server_manager *manager, event *ev)
{
	ev->type = ACCEPT_EVENT;
	ev->belong.manager = manager;
}

/* ���¼�����epoll������,���¼����ڼ���״̬ */
int event_start(event *ev)
{
	if (ev->type == IO_EVENT)
		epoller_add(ev->belong.loop->epoll_fd, ev);
	else
		epoller_add(ev->belong.manager->epoll_fd, ev);
	
	//hash_table_insert(manager->events, ev->fd, ev);
	ev->is_listening = 1;

	return 0;
}

/* ��ʼ�����¼� */
void event_stop(event *ev)
{
	/* �ж��¼�ev�Ƿ���epoll��,��ֹ�ظ�ɾ��ͬһ�¼� */
	if (ev->is_listening == 0)
		return;

	if (ev->type == IO_EVENT)
		epoller_del(ev->belong.loop->epoll_fd, ev);
	else
		epoller_del(ev->belong.manager->epoll_fd, ev);
	
	//hash_table_remove(manager->events, ev->fd);
	ev->is_listening = 0;
}

/* �����¼�,ͬʱclose��Ӧ���ļ������� */
void event_free(event *ev)
{
	event_stop(ev);
	close(ev->fd);
	free(ev);
}

#if 0
/* ���ļ�������ӳ�䵽event�ṹ�� */
event *fd_to_event(server_manager *manager, int fd)
{
	hash_node *hn;

	hn = hash_table_find(manager->events, fd);
	if (hn)
		return hn->value;
	
	return NULL;
}
#endif

/* �޸��¼��������� */
static int event_modify(event *ev)
{
	if (ev->type == IO_EVENT)
		epoller_modify(ev->belong.loop->epoll_fd, ev);
	else
		epoller_modify(ev->belong.manager->epoll_fd, ev);
	
	return 0;
}

/* ��������д�¼� */
void write_event_enable(event *ev)
{
	ev->events |= EPOLLOUT;
	event_modify(ev);
}

/* ֹͣ����д�¼� */
void write_event_disable(event *ev)
{
	ev->events &= ~EPOLLOUT;
	event_modify(ev);
}

