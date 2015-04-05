// The Code is from Data Structure book, and referenced from Zhezhao Xu.
// File Name : linklist.h
// Creater 	 : JackChang
// Date		 : 2015.3.10

#ifndef LINK_LIST_H
#define LINK_LIST_H

#define OK	1
#define OVERFLOW	-1
#define TRUE		1
#define FLASE		0
#define NOTEXIT		0
#define ERROR		0
typedef int Status;
typedef int ElemType;

typedef struct LNode
{
	ElemType data;
	struct LNode *next;
} LNode, *LinkList;

void	InitList_L(LinkList &L);
void	DestoryList_L(LinkList &L);
void	ClearList_L(LinkList &L);
bool	ListEmpty_L(LinkList L);
int		ListLength_L(LinkList L);
Status	GetElem_L(LinkList L, int i, ElemType &e);
int		LocateElem_L(LinkList L, ElemType e, Status (*compare)(ElemType, ElemType));
Status	PriorElem_L(LinkList L, ElemType cur_e, ElemType &pre_e);
Status	NextElem_L(LinkList L, ElemType cur_e, ElemType &next_e);
Status	ListInsert_L(LinkList &L, int i, ElemType e);
Status	ListDelete_L(LinkList &L, int i, ElemType &e);
Status	ListTraverse_L(LinkList L, Status (*visit)(struct LNode *&));
Status	visit_display_L(struct LNode *&L);

void 	CreateList_L(LinkList &L, int n);
void 	MergeList_L(LinkList La, LinkList Lb, LinkList &Lc);

#endif
