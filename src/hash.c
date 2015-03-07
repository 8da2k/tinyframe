#include <stdlib.h>
#include <stdio.h>

#include "hash.h"
#include "debug.h"

/* ����hash table */
hash_table *hash_table_create(unsigned int size)
{
    hash_table *ht;

    ht = malloc(sizeof(hash_table));
    if (ht == NULL)
	{
		debug_ret("file: %s, line: %d", __FILE__, __LINE__);
		return NULL;
	}
	
    ht->size = size;
    ht->buckets = calloc(size, sizeof(hash_node *));
    if (ht->buckets == NULL)
	{
		debug_ret("file: %s, line: %d", __FILE__, __LINE__);
		free(ht);
		return NULL;
	}
    return ht;
}

/* ɢ�к��� */
int hash(int key, unsigned int size)
{
    return key % size;
}

hash_node *hash_table_find(hash_table *ht, int key)
{
    hash_node *p;

    p = ht->buckets[hash(key, ht->size)];
    while (p != NULL && p->key!= key)
        p = p->next;

    return p;
}

/* ����Ԫ�ص�hash table */
int hash_table_insert(hash_table *ht, int key, void *value)
{
	hash_node **p;
	hash_node *new;

	if (hash_table_find(ht, key) == NULL)
    {
    	/* Ԫ�ز�����,��������ͷ�� */
        p = &(ht->buckets[hash(key, ht->size)]);
        new = malloc(sizeof(hash_node));
        new->key= key;
		new->value = value;
        new->next = *p;
        *p = new;
		return 0;
    }
	return -1;
}

/* ��hash_table���Ƴ�key��Ӧ��Ԫ��,���Ƴ���Ԫ�ز�����
 * ����ֵ:
 * 	0  : ɾ���ɹ�
 * 	-1 : ɾ��ʧ��,��ɾ��Ԫ�ز���hash_table��,
 */
int hash_table_remove(hash_table *ht, int key)
{
	hash_node **p;
	hash_node *pp;
	
	p = &(ht->buckets[hash(key, ht->size)]);
	pp = *p;
	if (pp == NULL)
		return -1;
	
	if (pp->key == key)
	{
		*p = pp->next;
		return 0;
	}
	
	while (pp->next != NULL && pp->next->key != key)
        pp = pp->next;
	if (pp->next != NULL)
	{
		/* find it */
		pp->next = pp->next->next;
		return 0;
	}
	return -1;
}

/* ��ӡhash table�е�����key */
void hash_table_print(hash_table *ht)
{
	hash_node *p;
	int i;
	for (i = 0; i < ht->size; i++)
	{
		printf("bucket %d : ", i);
		for (p = ht->buckets[i]; p != NULL; p = p->next)
			printf("%d ", p->key);
		printf("\n");
	}
}

void hash_table_free(hash_table *ht)
{
	hash_node *p;
	int i;

	for (i = 0; i < ht->size; i++)
	{
		p = ht->buckets[i];
		while (p != NULL)
		{
			ht->buckets[i] = p->next;
			free(p);
			p = ht->buckets[i];
		}
	}
	free(ht);
}

