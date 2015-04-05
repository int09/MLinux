// This is the code of Data Structure book, and reference from Zhezhao Xu
// File Name : linklist.c
// Creater	 : JackChang
// Date 	 : 2015.3.10

#include <stdio.h>
#include <stdlib.h>
#include "linklist.h"
#include "sqlist.h"			// compare function and other tools
void InitList_L(LinkList &L)
{
	L = (struct LNode *)malloc(sizeof(LNode));
	L->next = NULL;
}

void DestoryList_L(LinkList &L)
{
	free(L);
	L = NULL;
}

void ClearList_L(LinkList &L)
{
	L->next = NULL;
}

bool ListEmpty_L(LinkList L)
{
	if (NULL == L->next)
		return TRUE;
	else 
		return FALSE;
}

Status GetElem_L(LinkList L, int i, ElemType &e)
{
	// There is a head node ahead the link list, so 
	// the first element pointer is L->next
	struct LNode *p = L->next;
	int j = 1;
	while (p || j < i)
	{
		p = p->next;
		j++;
	}
	if (!p || j > i)
		return ERROR;
	e = p->data;
	return OK;
}

int LocateElem_L(LinkList L, ElemType e, Status (*compare)(ElemType,
											ElemType))
{
	LNode *p = L->next;
	int i = 0;
	while (p != NULL)
	{
		i++;
		if (compare(e, *p))
			return i;
		p = p->next;
	}
	return 0;
}

Status PriorElem_L(LinkList L, ElemType cur_e, ElemType &pre_e)
{
	if (L->next == NULL)
		return ERROR;
	int i;
	i = LocateElem_L(L, cur_e, compare_equal_Sq);
	if (i > 1 && i <L.length)
	{
		GetELem_L(L, i - 1, e);
		return TRUE;
	}
	return ERROR;
}

Status NextElem_L(LinkList L, ElemType cur_e, ElemType &next_e)
{
	if (L->next == NULL)
		return ERROR;
	int i;
	i = LocateElem_L(L, cur_e, compare_equal_Sq);
	if (i > 1 && i < L.length)
	{
		GetElem_L(L, i + 1, e);
		return TRUE;
	}
	return ERROR;
}

Status ListInsert_L(LinkList &L, int i, ElemType e)
{
	if (i < 1 || i > L.length + 1)
		return ERROR;
	struct LNode *p = L->next;
	int j = 1;
	while (p != NULL && j < i)
	{
		p = p->next;
		j++;
	}
	struct LNode *newnode = (LNode *)malloc(sizeof(LNode));
	if (newnode == NULL)
		return ERROR;
	newnode->data = e;
	newnode->next = p->next;
	p->next = newnode;
	return OK;
}

Status ListDelete_L(LinkList &L, int i, ElemType &e)
{

}

Status ListTraverse_L(LinkList L, Status (*visit)(struct LNode *&L))
{

}

Status visit_display_L(struct LNode *&L)
{

}

void CreateList_L(LinkList &L, int n)
{

}

void MergeList_L(LinkList La, LinkList Lb, LinkList &Lc)
{

}
