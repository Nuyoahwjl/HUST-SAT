/*----------------------------solver----------------------------*/

#include "SAT.h"

/*
 @ 函数名称: IsUnitClause
 @ 接受参数: literalList
 @ 函数功能: 判断是否为单子句
 @ 返回值: int
 */
status IsUnitClause(literalList l)
{
    if(l!=NULL && l->next==NULL)
        return TRUE;
    else
        return FALSE;
}

/*
 @ 函数名称: FindUnitClause
 @ 接受参数: clauseList
 @ 函数功能: 找到单子句并返回该文字
 @ 返回值: int
 */
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

/*
 @ 函数名称: DestroyClause
 @ 接受参数: clauseList &
 @ 函数功能: 销毁子句
 @ 返回值: void
 */
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

/*
 @ 函数名称: Simplify
 @ 接受参数: clauseList &, int
 @ 函数功能: 根据选择的文字化简
 @ 返回值: void
 */
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

/*
 @ 函数名称: ChooseLiteral
 @ 接受参数: void
 @ 函数功能: (没有单子句时的策略)选择文字
 @ 返回值: int
 */
// int ChooseLiteral()
// {
//     // 选择第一个未赋值的文字作为策略
//     for (int i = 1; i <= boolCount; i++) {
//         if (!result[i].flag) {
//             return i; // 选择正文字
//         }
//     }
//     return 0; // 所有文字已赋值
// }
int ChooseLiteral(clauseList cL)
{
    clauseList lp = cL;
	literalList dp;
    int* count, i, MaxWord, max; //count记录每个文字出现次数,MaxWord记录出现最多次数的文字
	count = (int*)malloc(sizeof(int) * (boolCount * 2 + 1));
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
    return MaxWord;
}

/*
 @ 函数名称: CopyCnf
 @ 接受参数: clauseList
 @ 函数功能: 复制cnf
 @ 返回值: clauseList
 */
clauseList CopyCnf(clauseList cL)
{
    if (cL == NULL) return NULL;
    clauseList newCnf = NULL;
    clauseList *pLastClause = &newCnf;
    while (cL != NULL) 
    {
        clauseList newClause = (clauseList)malloc(sizeof(clauseNode));
        newClause->head = NULL;
        newClause->next = NULL;
        literalList l = cL->head, *pLastLiteral = &newClause->head;
        while (l != NULL) 
        {
            literalList newLiteral = (literalList)malloc(sizeof(literalNode));
            newLiteral->literal = l->literal;
            newLiteral->next = NULL;
            *pLastLiteral = newLiteral;
            pLastLiteral = &newLiteral->next;
            l = l->next;
        }
        *pLastClause = newClause;
        pLastClause = &newClause->next;
        cL = cL->next;
    }
    return newCnf;
}
// clauseList CopyCnf(clauseList cL)
// {
//     clauseList newCnf = (clauseList)malloc(sizeof(clauseNode));
//     clauseList lpa,lpb;
// 	literalList tpa,tpb;
// 	newCnf->head = (literalList)malloc(sizeof(literalNode));
// 	newCnf->next = NULL;
// 	newCnf->head->next = NULL;
// 	for (lpb = cL, lpa = newCnf; lpb != NULL; lpb = lpb->next, lpa = lpa->next)
// 	{
// 		for (tpb = lpb->head, tpa = lpa->head; tpb != NULL; tpb = tpb->next, tpa = tpa->next)
// 		{
// 			tpa->literal = tpb->literal;
// 			tpa->next = (literalList)malloc(sizeof(literalNode));
// 			tpa->next->next = NULL;
// 			if (tpb->next == NULL)
// 			{
// 				free(tpa->next);
// 				tpa->next = NULL;
// 			}
// 		}
// 		lpa->next = (clauseList)malloc(sizeof(clauseNode));;
// 		lpa->next->head = (literalList)malloc(sizeof(literalNode));
// 		lpa->next->next = NULL;
// 		lpa->next->head->next = NULL;
// 		if (lpb->next == NULL)
// 		{
// 			free(lpa->next->head);
// 			free(lpa->next);
// 			lpa->next = NULL;
// 		}
// 	}
//     return newCnf;
// }

/*
 @ 函数名称: DPLL
 @ 接受参数: clauseList
 @ 函数功能: DPLL算法求解SAT问题
 @ 返回值: int
 */
// status DPLL(clauseList cL)
// {
//     // 初始化结果数组
//     if (result == NULL) 
//     {
//         result = (boolNode*)malloc(sizeof(boolNode) * (boolCount + 1));
//         for (int i = 1; i <= boolCount; i++) 
//         {
//             result[i].v = true;
//         }
//     }
//     // clauseList p = cL;
//     //     if (p == NULL)
//     //         return OK; // 所有子句都被满足了
//     //     while (p)
//     //     {
//     //         if (p->head == NULL) // 空子句，返回UNSAT
//     //             return ERROR;
//     //         p = p->next;
//     //     }
//     // 1. 单子句规则
//     int unitLiteral = FindUnitClause(cL);
//     while (unitLiteral != 0)
//     {
//         result[abs(unitLiteral)].v = (unitLiteral > 0) ? TRUE : FALSE;
//         // result[abs(unitLiteral)].flag = TRUE;
//         Simplify(cL, unitLiteral);
//         printf("-------------------\n");
//         PrintCnf(cL);
//         // 终止条件
//         clauseList p = cL;
//         if (p == NULL)
//             return OK; // 所有子句都被满足了
//         while (p)
//         {
//             if (p->head == NULL) // 空子句，返回UNSAT
//                 return ERROR;
//             p = p->next;
//         }
//         unitLiteral = FindUnitClause(cL);
//     }
//     // 2. 选择一个未赋值的文字
//     int literal = ChooseLiteral(cL);
//     if (literal == 0) return OK; // 没有可以选择的文字
//     // 3. 递归求解
//     result[abs(literal)].v = (literal > 0) ? TRUE : FALSE;
//     // result[abs(literal)].flag = TRUE;
//     clauseList newCnf = CopyCnf(cL);
//     clauseList p = (clauseList)malloc(sizeof(clauseNode));
//     p->head = (literalList)malloc(sizeof(literalNode));
//     p->head->literal = literal;
//     p->head->next = NULL;
//     p->next = newCnf;
//     newCnf = p;
//     if (DPLL(newCnf) == OK)
//     {
//         DestroyCnf(newCnf);
//         return OK;
//     }
//     result[literal].v = FALSE;
//     DestroyCnf(newCnf);
//     newCnf = CopyCnf(cL);
//     clauseList q = (clauseList)malloc(sizeof(clauseNode));
//     q->head = (literalList)malloc(sizeof(literalNode));
//     q->head->literal = -literal;
//     q->head->next = NULL;
//     q->next = newCnf;
//     newCnf = q;
//     status res = DPLL(newCnf);
//     DestroyCnf(newCnf); // 释放内存
//     return res;
// }

status DPLL(clauseList cnf)
{
    if (value == NULL) 
    {
        value = (int*)malloc(sizeof(int) * (boolCount + 1));
        for (int i = 1; i <= boolCount; i++) 
        {
            value[i] = 1;
        }
    }

    clauseList tp = cnf, lp = cnf, sp;
	literalList dp;
	int* count, i, MaxWord, max, re; //count记录每个文字出现次数,MaxWord记录出现最多次数的文字
	count = (int*)malloc(sizeof(int) * (boolCount * 2 + 1));
FIND:	while (tp != NULL && IsUnitClause(tp->head) == 0) tp = tp->next;  //找到表中的单子句
	if (tp != NULL)
	{
		//单子句规则简化
		if (tp->head->literal > 0) value[tp->head->literal] = 1;
		else value[-tp->head->literal] = 0;
		re = tp->head->literal;
		for (lp = cnf; lp != NULL; lp = sp)
		{
			sp = lp->next;

			//查找含有核心文字的句子
			for (dp = lp->head; dp != NULL; dp = dp->next)
			{
				if (dp->literal == re)
				{
					removeClause(lp, cnf);  //删除子句，简化式子
					break;
				}
				if (dp->literal == -re)
				{
					removeNode(dp, lp->head);  //删除文字，简化子句
					break;
				}
			}
		}
		//极简化规则简化后
        clauseList p = cnf;
		if (p == NULL)
		{
			free(count);
			return 1;
		}
		else 
		{
            while (p)
            {
                if (p->head == NULL)
                {
                    free(count);
                    DestroyCnf(cnf);
                    return 0;
                }
                p = p->next;
            }
        }

		tp = cnf;
		goto FIND;  //继续简化
	}
	for (i = 0; i <= boolCount * 2; i++) count[i] = 0;  //初始化

	//计算子句中各文字出现次数
	for (lp = cnf; lp != NULL; lp = lp->next)
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
	lp = (clauseList)malloc(sizeof(clauseNode));
	lp->head = (literalList)malloc(sizeof(literalNode));
	lp->head->literal = MaxWord;
	lp->head->next = NULL;
	lp->next = NULL;
	tp=CopyCnf(cnf);
	addClause(lp, tp);
	if (DPLL(tp) == 1) return 1;  //在第一分支中搜索
	DestroyCnf(tp);
	lp = (clauseList)malloc(sizeof(clauseNode));
	lp->head = (literalList)malloc(sizeof(literalNode));
	lp->head->literal = -MaxWord;
	lp->head->next = NULL;
	lp->next = NULL;
	addClause(lp, cnf);
	re = DPLL(cnf); //回溯到执行分支策略的初态进入另一分支
	DestroyCnf(cnf);
	return re;
}

int removeClause(clauseList& cnf, clauseList& root)
{
	clauseList lp = root;
	if (lp == cnf) root = root->next;  //删除为头
	else
	{
		while (lp != NULL && lp->next != cnf) lp = lp->next;
		lp->next = lp->next->next;
	}
	free(cnf);
	cnf = NULL;
	return 1;
}

int removeNode(literalList& cnf, literalList& head)
{
	literalList lp = head;
	if (lp == cnf) head = head->next;  //删除为头
	else
	{
		while (lp != NULL && lp->next != cnf) lp = lp->next;
		lp->next = lp->next->next;
	}
	free(cnf);
	cnf = NULL;
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
