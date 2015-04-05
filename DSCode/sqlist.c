// The Data Structure code in the book and referenced from Zhezhao Xu.
// File Name : sqlist.c
// Creater	 : JackChang
// Date		 : 2015.3.10
// Version	 : 1.0


#include <stdio.h>
#include <stdlib.h>

#include "sqlist.h"

Status InitList_Sq(struct Sqlist &L)
{
	// use malloc to allocate a (sizeof(ElemType) * LIST_INIT_SIZE) memory
	// to storage them, return a pointer L.elem to direct access it.
	L.elem = (ElemType *)malloc(LIST_INIT_SIZE * sizeof(ElemType));
	// while malloc function error to allocate a memory, return OVERFLOW
	if (!L.elem)
		return OVERFLOW;
	// let length 0, showed there is no one element in the list
	L.length = 0;
	// but the size of the list is how many we allocated.
	L.listsize = LIST_INIT_SIZE;
	// no error return OK
	return OK;
}

void DestoryList_Sq(struct Sqlist &L);
{
	// destory the list, free the memory which we allocated.
	free(L.elem);
	L.elem = NULL;
}

void ClearList_Sq(struct Sqlist &L)
{
	// the list length will set be 0
	L.length = 0;
}

Status ListEmpty_Sq(struct Sqlist L)
{
	if (L.length == 0)
		return TRUE;
	else
		return FALSE;
}

Status ListLength_Sq(struct Sqlist L)
{
	return L.length;
}


void GetElem_Sq(struct Sqlist L, int i, ElemType &e)
{
	// get the ith element, so the index is i - 1.
	e = *(L.elem + i - 1);
}

Status compare_equal_Sq(ElemType e1, ElemType e2)
{
	if (e1 == e2)
		return TRUE;
	else
		return FALSE;
}

Status LocateElem_Sq(struct Sqlist L, ElemType e, Status (*compare)(ElemType, ElemType))
{
	// use a pointer p point to L.elem, but the pointer pointed to
	// should be const.
	ElemType *p = L.elem;
	int i;
	// locate the element from 1 to L.length, but the index
	// should be i - 1. then reutrn it
	for (i = 1; i <= L.length; i++)
	{
		if (compare(e, *(p + i - 1)))
			return i;
	}
	// if it can't be finded, then return 0
	return 0;
}

Status PriorElem_Sq(struct Sqlist L, ElemType cur_e, ElemType &pre_e)
{
	int pos;
	// locate the element cur_e
	pos = LocateElem_Sq(L, cur_e, compare_equal_Sq);
	// if the position is 0 or 1, then return NOTEXIT
	if (!pos || 1 == pos)
		return NOTEXIT;
	else
		return *(L.elem + pos - 1 - 1);
}

Status NextElem_Sq(struct Sqlist L, ElemType cur_e, ElemType &next_e)
{
	int pos;
	pos = LocateElem_Sq(L, cur_e, compare_equal_Sq);
	if (!pos || 1 == pos)
		return NOTEXIT;
	else
		return *(L.elem + pos);
}

void ListInsert_Sq(struct Sqlist &L, int i, ElemType e)
{
	// if after insert the length is bigger than list size
	// then reallocte a bigger memory, then set the newbase to L.elem
	if (i < 1 || i > L.length + 1)
		return ERROR;
	ElemType *newbase;
	if (L.length + 1 > L.listsize)
	{
		newbase = (ElemType *)realloc(L.elem, (L.listsize + LISTINCREMENT)*sizeof(ElemType));
		if (!newbase)
			exit OVERFLOW;
		L.elem = newbase;
		L.listsize += LISTINCREMENT;
	}
	// let these elements behind new element go to next position
	int j;
	for (j = L.length; j >= i; j--)
		*(L.elem + j) = *(L.elem + j - 1);
	// then set the ith element is e
	*(L.elem + i - 1) = e;
	L.length++;
}

void ListDelete_Sq(struct Sqlist &L, int i, ElemType &e)
{
	int j;
	// get the ith element
	e = *(L.elem + i - 1);
	// the elements behind ith element go to the previor position
	for (j = i; j < L.length; j++)
		*(L.elem + j - 1) = *(L.elem + j);
	// let the length miners one.
	L.length--;
}

Status ListTraverse_Sq(struct Sqlist L, Status (*visit)(ElemType))
{
	int i;
	for (i = 0; i < L.length; i++)
	{
		if (!visit(*(L.elem + i)))
			return FALSE;
	}
	return TRUE;
}

Status visit_display_Sq(ElemType e)
{
	printf("%d ", e);
	return TRUE;
}

void unionList_Sq(struct Sqlist &La, struct Sqlist Lb)
{
	int La_len = ListLength_Sq(La);
	int Lb_len = ListLength_Sq(Lb);
	int i;
	ElemType e;
	for (i = 1; i <= Lb_len; i++)
	{
		GetElem_Sq(Lb, i, e);
		if (!LocateElem_Sq(La, e, compare_equal_Sq))
			ListInsert_Sq(La, ++La_len, e);
	}
}

void MergeList_Sq(struct Sqlist La, struct Sqlist Lb, struct Sqlist &Lc)
{
	InitList(Lc);
	int La_len, Lb_len;
	La_len = ListLength_Sq(La);
	Lb_len = ListLength_Sq(Lb);
	int i, j, k, ai, bj;
	while (i <= La_len && j <= Lb_len)
	{
		GetElem_Sq(La, i, ai);
		GetElem_Sq(Lb, j, bj);
		if (ai <= bj)
		{
			ListInsert_Sq(Lc, k++, ai);
			i++;
		}
		else
		{
			ListInsert_Sq(Lc, k++, bj);
			j++;
		}
	}
	while (i <= La_len)
	{
		GetElem_Sq(La, i++, ai);
		ListInsert_Sq(Lc, k++, ai);
	}
	while (i <= Lb_len)
	{
		GetElem_Sq(Lb, j++, bj);
		ListInsert_Sq(Lc, k++, bj);
	}
}

/*
 The code that in the book of Data Structure
*/
void MergeList_Sq(Sqlist La, Sqlist Lb, Sqlist &Lc)
{
	pa = La.elem;
	pb = Lb.elem;
	Lc.listsize = Lc.length = La.length + Lb.length;
	pc = Lc.elem = (ElemType *)malloc(Lc.listsize * sizeof(ElemType));
	if (!Lc.elem)
		return OVERFLOW;
	pa_last = La.elem + La.length - 1;
	pb_last = Lb.elem + Lb.length - 1;
	while (pa <= pa_last && pb <= pb_last)
	{
		if (*pa <= *pb)
			*pc++ = *pa++;
		else
			*pc++ = *pb++;
	}
	while (pa <= pa_last)
		*pc++ = *pa++;
	while (pa <= pb_last)
		*pc++ = *pb++;
}

// Find the first element that match
int LocateElem_Sq(Sqlist L, ElemType e, Status (*compare)(ElemType, ElemType))
{
	i = 1;
	p = L.elem;
	while (i <= L.length && !(*compare)(*p++, e))
		++i;
	if (i <= L.length)
		return i;
	else
		return 0;
}

Status ListInsert_Sq(Sqlist &L, int i, ElemType e)
{
	if (i < 1 || i > L.length + 1)
		return ERROR;
	if (L.length >= L.listsize)
	{
		newbase = (ElemType *)realloc(L.elem,
				(L.listsize + LISTINCREMENT) * sizeof(ElemType));
		if (!newbase)
			return OVERFLOW;
		L.elem = newbase;
		L.listsize += LISTINCREMENT;
	}
	q = &(L.elem[i - 1]);
	// q = L.elem - i - 1;
	// use pointer to trverse the list while p is behind q
	for (p = &(L.elem[L.length - 1]); p >= q; --p)
		*(p + 1) = *p;
	// let the element at q equal to e
	*q = e;
	// so the length of list will be ++
	++L.length;
	return OK;
}

Status ListDelete_Sq(Sqlist &L, int i, ElemType &e)
{
	if (i < 1 || i > L.length)
		return ERROR;
	p = &(L.elem[i - 1]);
	e = *p;
	q = L.elem + L.length - 1;
	for (++p; p <= q; ++p)
		*(p - 1) = *p;
	L.length--;
	return OK;
}

Status InitList_Sq(Sqlist &L)
{
	L.elem = (ElemType *)malloc(LIST_INIT_SIZE * sizeof(ElemType));
	if (!L.elem)
		return OVERFLOW;
	L.length = 0;
	L.listsize = LIST_INIT_SIZE;
	return OK;
}
// Now, the alogorthm ahead is the logical type that the book on.
// the C language use pointer directlly for a higher effective.
// But I think the code Grandma Yan written was ugly.
