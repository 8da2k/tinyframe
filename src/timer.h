#ifndef __TIMER_H__
#define __TIMER_H__

#include "servermanager.h"

enum timer_options {
	TIMER_OPT_NONE,		/* ��ʱ������ */
	TIMER_OPT_ONCE,		/* ��ʱ����һ�� */
	TIMER_OPT_REPEAT	/* ��ʱ�ظ����� */
};

typedef struct server_manager_t server_manager;
typedef struct timer_t timer;
typedef void (*timeout_callback_pt)(timer *t, void *arg);

struct timer_t {
	struct timeval timeout_abs;		/* ��ʱ����ʱ�� */
	struct timeval timeout_rel;		/* ��ʱ���ʱ�� */
	enum timer_options option;		/* �Ƿ��ظ����� */

	/* ��ʱ�ص����� */
	timeout_callback_pt timeout_handler;
	void *arg;

	timer *prev;
	timer *next;
};

timer *timer_new(struct timeval timeout, timeout_callback_pt timeout_handler,
					void *arg, enum timer_options option);
void timer_reset(timer *t);
long tv_to_msec(const struct timeval *tv);
void timer_add(server_manager *manager, timer *t);
void timer_remove(timer *t);

#endif

