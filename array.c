#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "debug.h"

/* ��ʼ��һ���Ѵ��ڵĶ�̬���� */
static int array_init(array *array, unsigned int n, size_t size)
{
    array->nelts = 0;
    array->size = size;
    array->capacity= n;
    array->elts = malloc(n * size);
    if (array->elts == NULL)
	{
		debug_ret("file: %s, line: %d", __FILE__, __LINE__);
        return -1;
    }
    return 0;
}

/* ������̬����,��СΪn*size�ֽ� */
array *array_create(unsigned int n, size_t size)
{
    array *a = malloc(sizeof(array));
    if (a == NULL)
	{
		debug_ret("file: %s, line: %d", __FILE__, __LINE__);
		return NULL;
    }

    if (array_init(a, n, size) < 0)
	{
		debug_ret("file: %s, line: %d", __FILE__, __LINE__);
		return NULL;
    }

    return a;
}

/* ���ٶ�̬����,��ngx_array_create���ʹ�� */
void array_free(array *a)
{
    free(a->elts);
	free(a);
}

/* ���һ��Ԫ��,������Ԫ�ص�ַ */
void *array_push(array *a)
{
    void        *elt, *new;
    size_t       size;

    if (a->nelts == a->capacity)
	{
		/* ���� */
        size = a->size * a->capacity;

        new = malloc(2 * size);
        if (new == NULL)
		{
			debug_ret("file: %s, line: %d", __FILE__, __LINE__);
            return NULL;
        }

        memcpy(new, a->elts, size);
		free(a->elts);
        a->elts = new;
        a->capacity *= 2;
    }

    elt = (unsigned char *) a->elts + a->size * a->nelts;
    a->nelts++;

    return elt;
}

/* ���n����Ԫ��,���ص�һ����Ԫ�صĵ�ַ */
void *array_push_n(array *a, unsigned int n)
{
    void        *elt, *new;
    unsigned int   nalloc;

    if (a->nelts + n > a->capacity)
	{
        /* ���� */
        nalloc = 2 * ((n >= a->capacity) ? n : a->capacity);

        new = malloc(nalloc * a->size);
        if (new == NULL)
		{
			debug_ret("file: %s, line: %d", __FILE__, __LINE__);
            return NULL;
        }

        memcpy(new, a->elts, a->nelts * a->size);
		free(a->elts);
        a->elts = new;
        a->capacity = nalloc;
    }

    elt = (unsigned char *) a->elts + a->size * a->nelts;
    a->nelts += n;

    return elt;
}

/* ��from�е��غɸ��Ƶ�����to��,���Զ���ĩβ׷��'\0' */
int array_copy(array *from, char *to, int len)
{
	int n;
	if (from->nelts * from->size < len)
		n = from->nelts * from->size;
	else
		n = len;
	
	memcpy(to, from->elts, n);
	
	/* ����buffer��� */
	from->nelts = 0;
	return n;
}

