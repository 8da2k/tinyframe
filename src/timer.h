#ifndef __TIMER_H__
#define __TIMER_H__

enum timer_options {
	TIMER_OPT_NONE,		/* ��ʱ������ */
	TIMER_OPT_ONCE,		/* ��ʱ����һ�� */
	TIMER_OPT_REPEAT	/* ��ʱ�ظ����� */
};

typedef struct timer_t timer;

typedef struct timer_t {
	struct timeval timeout_abs;		/* ��ʱ����ʱ�� */
	struct timeval timeout_rel;		/* ��ʱ���ʱ�� */
	enum timer_options option;		/* �Ƿ��ظ����� */

	/* ��ʱ�ص����� */
	void (*timeout_handler)(void *arg);
	void *arg;

	timer *prev;
	timer *next;
	
}timer;

timer *timer_new(struct timeval timeout, void (*timeout_handler)(void *arg),
					void *arg, enum timer_options option);
void timer_reset(timer *t);

long tv_to_msec(const struct timeval *tv);

#endif

