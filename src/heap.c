#include <stdlib.h>

#include "heap.h"
#include "debug.h"
#include "array.h"


static int min_heap_elem_greater(timer *a, timer *b)
{
	return timer_cmp(&a->timeout_abs, &b->timeout_abs, >);
}


int heap_is_full(heap *h)
{
	return (h->current_size == h->max_size);
}

int heap_is_empty(heap *h)
{
	return (h->current_size == 0);
}

/* ������,size�������0 */
heap *heap_create(unsigned int size)
{
	heap *h = malloc(sizeof(heap));
	if (h == NULL)
	{
		debug_ret("file: %s, line: %d", __FILE__, __LINE__);
		return NULL;
	}

	h->data = malloc((size + 1) * sizeof(timer *));
	h->max_size = size;
	h->current_size = 0;

	/* �ڱ�timer */
	struct timeval timeout;
	h->data[0] = timer_new(timeout, NULL, NULL, TIMER_OPT_NONE);
	h->data[0]->timeout_abs.tv_sec = 0;
	h->data[0]->timeout_abs.tv_usec = 0;
	
	return h;
}

/* ������� */
void heap_insert(heap *h, timer *t)
{
	int index;
	
	if (heap_is_full(h))
	{
		timer **data;
		unsigned int max_size = h->max_size * 2;
		data = (timer **)realloc(h->data, max_size * sizeof(timer *));
		if (data == NULL)
		{
			debug_sys("file: %s, line: %d", __FILE__, __LINE__);
		}
		h->data = data;
		h->max_size= max_size;      
	}
	
	for (index = ++h->current_size; min_heap_elem_greater(h->data[index/2], t); index /= 2)
	{
		h->data[index] = h->data[index/2];		/* ��Ѩ���� */
	}

	/* ��Ҫ�����Ԫ�طŵ���ȷ��λ�� */
	h->data[index] = t;
}

/* ɾ������,���ضѶ�Ԫ�ػ�NULL */
timer *heap_delete(heap *h)
{
	int index, child;
	timer *top;
	timer *last;
	
	if (heap_is_empty(h))
	{
		debug_msg("file: %s, line: %d", __FILE__, __LINE__);
		return NULL;
	}
	
	top = h->data[1];
	last = h->data[h->current_size];
	h->current_size--;
	
	for (index = 1; 2*index <= h->current_size; index = child)
	{
		/* ��֤�����Ӳ��ܽ���ѭ�� */
		child = 2 * index;
		if (child < h->current_size && min_heap_elem_greater(h->data[child], h->data[child+1]))
			child++;	/* ��֤���Һ��Ӳ����Һ��ӱ�����С */

		if (min_heap_elem_greater(last, h->data[child]))
			h->data[index] = h->data[child];
		else
			break;
	}
	
	h->data[index] = last;
	
	return top;
}

timer *heap_top(heap *h)
{
	if (heap_is_empty(h))
		return NULL;
	else
		return h->data[1];
}

void heap_free(heap *h)
{
	timer_free(h->data[0]);
	free(h->data);
	free(h);
}

