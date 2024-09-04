/*----------------------------solver----------------------------*/

#include "SAT.h"

status IsUnitClause(literalList l)
{
    if(l!=NULL && l->next==NULL)
        return TRUE;
    else
        return FALSE;
}

int FindUnitClause(clauseList cL)
{
    clauseList p=cL;
    while(p)
    {
        if(IsUnitClause(p->head))
            return p->head->literal;
        p=p->next;
    }
    return 0;
}

status DestroyClause(clauseList &cL)
{
    literalList p=cL->head;
    while(p)
    {
        literalList temp=p;
        p=p->next;
        free(temp);
    }
    free(cL);
    cL=NULL;
    return OK;
}

void Simplify(clauseList &cL, int literal)
{
    clauseList pre=NULL,p=cL;
    while(p!=NULL)
    {
        bool clauseDeleted = false;
        literalList lpre=NULL,q=p->head;
        while(q!=NULL)
        {
            if(q->literal==literal) //删除该子句
            {
                if(pre==NULL)
                    cL=p->next;
                else
                    pre->next=p->next;
                DestroyClause(p);
                p=(pre==NULL)?cL:pre->next;
                clauseDeleted = true;
                break;
            }
            else if(q->literal==-literal) //删除该文字
            {
                if(lpre==NULL)
                    p->head=q->next;
                else
                    lpre->next=q->next;
                free(q);
                q = (lpre == NULL) ? p->head : lpre->next;
            }
            else
            {
                lpre=q;
                q=q->next;
            }
        }
        if(!clauseDeleted)
        {
            pre=p;
            p=p->next;
        }
    }
}

int removeClause(clauseList& p, clauseList& cL)
{
	clauseList lp = cL;
	if (lp == p) cL = cL->next;  //删除为头
	else
	{
		while (lp != NULL && lp->next != p) lp = lp->next;
		lp->next = lp->next->next;
	}
	free(p);
	p = NULL;
	return 1;
}

int removeNode(literalList& q, literalList& head)
{
	literalList lp = head;
	if (lp == q) head = head->next;  //删除为头
	else
	{
		while (lp != NULL && lp->next != q) lp = lp->next;
		lp->next = lp->next->next;
	}
	free(q);
	q = NULL;
	return 1;
}

int addClause(clauseList cnf, clauseList& root)
{
	//直接插入在表头
	if (cnf != NULL)
	{
		cnf->next = root;
		root = cnf;
		return 1;
	}
	return 0;
}

int isUnitClause(literalList cL)
{
	if (cL != NULL && cL->next == NULL)
		return 1;
	else
		return 0;
}

int emptyClause(clauseList cL)
{
	clauseList lp = cL;
	while (lp != NULL)
	{
		if (lp->head == NULL) return 1;
		lp = lp->next;
	}
	return 0;
}

void CopyClause(clauseList &a, clauseList b)
{
	clauseList lpa,lpb;
	literalList tpa,tpb;
	a = (clauseList)malloc(sizeof(clauseNode));
	a->head = (literalList)malloc(sizeof(literalNode));
	a->next = NULL;
	a->head->next = NULL;
	for (lpb = b, lpa = a; lpb != NULL; lpb = lpb->next, lpa = lpa->next)
	{
		for (tpb = lpb->head, tpa = lpa->head; tpb != NULL; tpb = tpb->next, tpa = tpa->next)
		{
			tpa->literal = tpb->literal;
			tpa->next = (literalList)malloc(sizeof(literalNode));
			tpa->next->next = NULL;
			if (tpb->next == NULL)
			{
				free(tpa->next);
				tpa->next = NULL;
			}
		}
		lpa->next = (clauseList)malloc(sizeof(clauseNode));
		lpa->next->head = (literalList)malloc(sizeof(literalNode));
		lpa->next->next = NULL;
		lpa->next->head->next = NULL;
		if (lpb->next == NULL)
		{
			free(lpa->next->head);
			free(lpa->next);
			lpa->next = NULL;
		}
	}
}

clauseList CopyCnf(clauseList cL)
{
    clauseList newCnf = (clauseList)malloc(sizeof(clauseNode));
    clauseList lpa,lpb;
	literalList tpa,tpb;
	newCnf->head = (literalList)malloc(sizeof(literalNode));
	newCnf->next = NULL;
	newCnf->head->next = NULL;
	for (lpb = cL, lpa = newCnf; lpb != NULL; lpb = lpb->next, lpa = lpa->next)
	{
		for (tpb = lpb->head, tpa = lpa->head; tpb != NULL; tpb = tpb->next, tpa = tpa->next)
		{
			tpa->literal = tpb->literal;
			tpa->next = (literalList)malloc(sizeof(literalNode));
			tpa->next->next = NULL;
			if (tpb->next == NULL)
			{
				free(tpa->next);
				tpa->next = NULL;
			}
		}
		lpa->next = (clauseList)malloc(sizeof(clauseNode));;
		lpa->next->head = (literalList)malloc(sizeof(literalNode));
		lpa->next->next = NULL;
		lpa->next->head->next = NULL;
		if (lpb->next == NULL)
		{
			free(lpa->next->head);
			free(lpa->next);
			lpa->next = NULL;
		}
	}
    return newCnf;
}

status DPLL(clauseList &cL)
{
	clauseList lp = cL;
	literalList dp;
	int* count, i, MaxWord, max; //count记录每个文字出现次数,MaxWord记录出现最多次数的文字
	count = (int*)malloc(sizeof(int) * (boolCount * 2 + 1));
// FIND:	while (tp != NULL && isUnitClause(tp->head) == 0) tp = tp->next;  //找到表中的单子句
// 	if (tp != NULL)
// 	{
// 		//单子句规则简化
// 		if (tp->head->literal > 0) value[tp->head->literal] = 1;
// 		else value[-tp->head->literal] = 0;
// 		re = tp->head->literal;
// 		for (lp = cL; lp != NULL; lp = sp)
// 		{
// 			sp = lp->next;
// 			//查找含有核心文字的句子
// 			for (dp = lp->head; dp != NULL; dp = dp->next)
// 			{
// 				if (dp->literal== re)
// 				{
// 					removeClause(lp, cL);  //删除子句，简化式子
// 					break;
// 				}
// 				if (dp->literal == -re)
// 				{
// 					removeNode(dp, lp->head);  //删除文字，简化子句
// 					break;
// 				}
// 			}
// 		}
// 		//极简化规则简化后
// 		if (cL == NULL)
// 		{
// 			free(count);
// 			return 1;
// 		}
// 		else if (emptyClause(cL))
// 		{
// 			free(count);
// 			DestroyCnf(cL);
// 			return 0;
// 		}
// 		tp = cL;
// 		goto FIND;  //继续简化
// 	}
    int unitLiteral = FindUnitClause(cL);
    while (unitLiteral != 0)
    {
        value[abs(unitLiteral)] = (unitLiteral > 0) ? TRUE : FALSE;
        Simplify(cL, unitLiteral);
        // 终止条件
        clauseList p = cL;
        if (p == NULL)
            return OK; // 所有子句都被满足了
        while (p)
        {
            if (p->head == NULL) // 空子句，返回UNSAT
                return ERROR;
            p = p->next;
        }
        unitLiteral = FindUnitClause(cL);
    }
	
    for (i = 0; i <= boolCount * 2; i++) count[i] = 0;  //初始化

	//计算子句中各文字出现次数
	for (lp = cL; lp != NULL; lp = lp->next)
	{
		for (dp = lp->head; dp != NULL; dp = dp->next)
		{
			if (dp->literal > 0) count[dp->literal]++;
			else count[boolCount - dp->literal]++;
		}
	}
	max = 0;

	//找到出现次数最多的正文字
	for (i = 2; i <= boolCount; i++)
	{
		if (max < count[i])
		{
			max = count[i];
			MaxWord = i;
		}
	}
	if (max == 0)
	{
		//若没有出现正文字,找到出现次数最多的负文字
		for (i = boolCount + 1; i <= boolCount * 2; i++)
		{
			if (max < count[i])
			{
				max = count[i];
				MaxWord = -i;
			}
		}
	}
	free(count);
    clauseList newCnf=CopyCnf(cL);
    clauseList p = (clauseList)malloc(sizeof(clauseNode));
    p->head = (literalList)malloc(sizeof(literalNode));
    p->head->literal = MaxWord;
    p->head->next = NULL;
    p->next=newCnf;
    newCnf=p;
	if (DPLL(newCnf) == 1) return 1;  //在第一分支中搜索
	DestroyCnf(newCnf);

	newCnf=CopyCnf(cL);
	clauseList q = (clauseList)malloc(sizeof(clauseNode));
	q->head = (literalList)malloc(sizeof(literalNode));
	q->head->literal = -MaxWord;
	q->head->next = NULL;
	q->next = newCnf;
	newCnf=q;
	// addClause(q, cL);
	status re = DPLL(newCnf); //回溯到执行分支策略的初态进入另一分支
	// DestroyCnf(cL);
	return re;
}

// status DPLL(clauseList &cL)
// {
// 	clauseList tp = cL, lp = cL, sp;
// 	literalList dp;
// 	int* count, i, MaxWord, max, re; //count记录每个文字出现次数,MaxWord记录出现最多次数的文字
// 	count = (int*)malloc(sizeof(int) * (boolCount * 2 + 1));
// FIND:	while (tp != NULL && isUnitClause(tp->head) == 0) tp = tp->next;  //找到表中的单子句
// 	if (tp != NULL)
// 	{
// 		//单子句规则简化
// 		if (tp->head->literal > 0) value[tp->head->literal] = 1;
// 		else value[-tp->head->literal] = 0;
// 		re = tp->head->literal;
// 		for (lp = cL; lp != NULL; lp = sp)
// 		{
// 			sp = lp->next;
// 			//查找含有核心文字的句子
// 			for (dp = lp->head; dp != NULL; dp = dp->next)
// 			{
// 				if (dp->literal == re)
// 				{
// 					removeClause(lp, cL);  //删除子句，简化式子
// 					break;
// 				}
// 				if (dp->literal == -re)
// 				{
// 					removeNode(dp, lp->head);  //删除文字，简化子句
// 					break;
// 				}
// 			}
// 		}
// 		//极简化规则简化后
// 		if (cL == NULL)
// 		{
// 			free(count);
// 			return 1;
// 		}
// 		else if (emptyClause(cL))
// 		{
// 			free(count);
// 			DestroyCnf(cL);
// 			return 0;
// 		}
// 		tp = cL;
// 		goto FIND;  //继续简化
// 	}
// 	for (i = 0; i <= boolCount * 2; i++) count[i] = 0;  //初始化
// 	//计算子句中各文字出现次数
// 	for (lp = cL; lp != NULL; lp = lp->next)
// 	{
// 		for (dp = lp->head; dp != NULL; dp = dp->next)
// 		{
// 			if (dp->literal > 0) count[dp->literal]++;
// 			else count[boolCount - dp->literal]++;
// 		}
// 	}
// 	max = 0;
// 	//找到出现次数最多的正文字
// 	for (i = 2; i <= boolCount; i++)
// 	{
// 		if (max < count[i])
// 		{
// 			max = count[i];
// 			MaxWord = i;
// 		}
// 	}
// 	if (max == 0)
// 	{
// 		//若没有出现正文字,找到出现次数最多的负文字
// 		for (i = boolCount + 1; i <= boolCount * 2; i++)
// 		{
// 			if (max < count[i])
// 			{
// 				max = count[i];
// 				MaxWord = -i;
// 			}
// 		}
// 	}
// 	free(count);
// 	lp = (clauseList)malloc(sizeof(clauseNode));
// 	lp->head = (literalList)malloc(sizeof(literalNode));
// 	lp->head->literal = MaxWord;
// 	lp->head->next = NULL;
// 	lp->next = NULL;
// 	CopyClause(tp, cL);
// 	addClause(lp, tp);
// 	if (DPLL(tp) == 1) return 1;  //在第一分支中搜索
// 	DestroyCnf(tp);
// 	lp = (clauseList)malloc(sizeof(clauseNode));
// 	lp->head = (literalList)malloc(sizeof(literalNode));
// 	lp->head->literal = -MaxWord;
// 	lp->head->next = NULL;
// 	lp->next = NULL;
// 	addClause(lp, cL);
// 	re = DPLL(cL); //回溯到执行分支策略的初态进入另一分支
// 	DestroyCnf(cL);
// 	return re;
// }