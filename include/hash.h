#ifndef __HASH_H__
#define __HASH_H__

typedef int ElementType;

typedef struct ListNode *Position;
struct ListNode
{
    ElementType Element;
    Position Next;
};

typedef struct HashTbl *HashTable;
struct HashTbl
{
    int TableSize;
    Position *TheLists;  /* ָ������,��Ų�ͬ��Ͱ */
};


#endif

