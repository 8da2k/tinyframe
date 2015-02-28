#include <sys/time.h>
#include <stdlib.h>
#include <limits.h>

#include "timer.h"
#include "debug.h"
#include "servermanager.h"
#include "heap.h"

/* ����һ��timer����
 * @timeout : ��timer����Գ�ʱʱ��
 * @timeout_handler : ��ʱ�ص�����
 * @arg : ��ʱ�ص������Ĳ���
 * @option : timer����
 */
timer *timer_new(struct timeval timeout, void (*timeout_handler)(void *arg),
					void *arg, enum timer_options option)
{
	timer *t = malloc(sizeof(timer));
	if (t == NULL)
	{
		debug_ret("file: %s, line: %d", __FILE__, __LINE__);
		return NULL;
	}

	struct timeval now;
	gettimeofday(&now, NULL);
	t->timeout_abs.tv_sec = now.tv_sec + timeout.tv_sec;
	t->timeout_abs.tv_usec = now.tv_usec + timeout.tv_usec;
	t->timeout_rel = timeout;
	t->timeout_handler = timeout_handler;
	t->arg = arg;
	t->option = option;
	t->prev = NULL;
	t->next = NULL;

	return t;
}

/* ��timer���뵽server_manager���� */
void timer_add(server_manager *manager, timer *t)
{
	heap_insert(manager->timers, t);
}

void timer_remove(timer *t)
{
	t->option= TIMER_OPT_NONE;
}

/* ����һ��timer */
void timer_free(timer *t)
{
	free(t);
}


#define MAX_SECONDS_IN_MSEC_LONG \
	(((LONG_MAX) - 999) / 1000)

long tv_to_msec(const struct timeval *tv)
{
	if (tv->tv_usec > 1000000 || tv->tv_sec > MAX_SECONDS_IN_MSEC_LONG)
		return -1;

	return (tv->tv_sec * 1000) + ((tv->tv_usec + 999) / 1000);
}

