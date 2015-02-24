#ifndef __ARRAY_H__
#define __ARRAY_H__

/* �ɱ䳤����,�ο�NginxԴ�� */

typedef struct {
	/* ָ�������׵�ַ */
    void *elts;

	/* ����Ԫ�ظ��� */
    unsigned int nelts;

	/* ÿ��Ԫ���ֽڴ�С */
    size_t size;

	/* ���� */
    unsigned int capacity;
}array;


array *array_create(unsigned int n, size_t size);
void array_free(array *a);
void *array_push(array *a);
void *array_push_n(array *a, unsigned int n);
int array_copy(array *from, char *to, int len);
void array_clear(array *a);

#endif
