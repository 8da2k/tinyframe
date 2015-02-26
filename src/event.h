#ifndef __EVENT_H__
#define __EVENT_H__

#include <sys/time.h>
#include <sys/epoll.h>

#include "servermanager.h"
#include "connection.h"

/* ��д�¼��ص�����ָ�� */
typedef void (*event_callback_pt)(int fd, void *arg);

typedef struct event_t event;
typedef struct server_manager_t server_manager;

typedef struct event_t {
	int fd;
	int events;				/* ��ע���¼� */
	int actives;			/* �����¼����� */
	int is_listening;		/* 1��ʾ���¼����ڼ���״̬��,����server_manager��eventsɢ�б��� */
	int is_active;			/* 1��ʾ���¼�������δ������,����server_manager��activesɢ�б��� */
	struct timeval time;	/* �¼�����ʱ�� */
	connection *conn;		/* �¼���Ӧ������ */

	/* �ȴ��¼����� */
	event *listen_next;
	event *listen_pre;

	/* �����¼����� */
	event *active_next;
	event *active_pre;

	/* ���¼�������server_manager */
	server_manager *manager;

	/* �¼��ص�������� */
	void (*event_handler)(event *ev);

	/* �ɶ��ص� */
	event_callback_pt event_read_handler;
	void *r_arg;

	/* ��д�ص� */
	event_callback_pt event_write_handler;
	void *w_arg;

	/* �رջص� */
	event_callback_pt event_close_handler;
	void *c_arg;
	
}event;

event* event_create(server_manager *manager, int fd, short events,
	event_callback_pt read_cb, void *r_arg, event_callback_pt write_cb, void *w_arg);
int event_start(event *ev);
void event_stop(event *ev);
void event_free(event *ev);
event *fd_to_event(server_manager *manager, int fd);

void write_event_enable(event *ev);
void write_event_disable(event *ev);

#endif

