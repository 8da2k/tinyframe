#include "hash.h"

/* ɢ�б�,�ο�<<���ݽṹ���㷨����>> */

HashTable InitializeTable(int TableSize)
{
    HashTable ht;
    int i;

    ht = malloc(sizeof(struct HashTbl));
    if (ht == NULL)
        return -1;

    ht->TableSize = TableSize;
    ht->TheLists = calloc(TableSize, sizeof(Position));
    if (ht->TheLists == NULL)
        return -1;

    return ht;
}

// ɢ�к���
int Hash(ElementType Key, int TableSize)
{
    return Key % TableSize;
}

Position Find(HashTable H, ElementType Key)
{
    Position p;

    p = H->TheLists[Hash(Key, H->TableSize)];
    while (p != NULL && p->Element != Key)  // ����ֻ�ܶ���ֵ��key���бȽ�
        p = p->Next;

    return p;
}

// ��ֵ�������ظ�
void Insert(HashTable H, ElementType Key)
{
    Position *p;    // �����ʹ�����˫��ָ�룬��ô��Ҫ������ɢ�к���
    Position tmp;

    if (Find(H, Key) == NULL)    // Ԫ�ز�����
    {   // ��������ͷ��
        p = &(H->TheLists[Hash(Key, H->TableSize)]);
        tmp = malloc(sizeof(struct ListNode));
        tmp->Element = Key;
        tmp->Next = *p;
        *p = tmp;
    }
}

/*

int main(void)
{
    HashTable table;
    Position p;

    table = InitializeTable(100);

    Insert(table, 21);
    Insert(table, 24);
    Insert(table, 43);
    Insert(table, 35);

    p = Find(table, 43);
    if (p != NULL)
        printf("Find it!\n");
    else
        printf("Not found!\n");

    return 0;
}

*/
