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
int ChooseLiteral()
{
    // 选择第一个未赋值的文字作为策略
    for (int i = 1; i <= boolCount; i++) {
        if (!result[i].flag) {
            return i; // 选择正文字
        }
    }
    return 0; // 所有文字已赋值
}
// int chooseLiteral(clauseList cL)
// {
//     int literal=FindUnitClause(cL);
//     if(literal!=0)
//         return literal;
//     else
//     {
//         clauseList p=cL->next;
//         int max=0;
//         while(p)
//         {
//             literalList q=p->head;
//             while(q->next)
//             {
//                 int temp=q->next->literal;
//                 if(temp>0 && result[temp].flag==FALSE)
//                 {
//                     int count=0;
//                     clauseList t=cL->next;
//                     while(t)
//                     {
//                         literalList s=t->head;
//                         while(s->next)
//                         {
//                             if(s->next->literal==temp)
//                             {
//                                 count++;
//                                 break;
//                             }
//                             s=s->next;
//                         }
//                         t=t->next;
//                     }
//                     if(count>max)
//                     {
//                         max=count;
//                         literal=temp;
//                     }
//                 }
//                 q=q->next;
//             }
//             p=p->next;
//         }
//         return literal;
//     }
// }

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
status DPLL(clauseList cL)
{
    // 初始化结果数组
    if (result == NULL) 
    {
        result = (boolNode*)malloc(sizeof(boolNode) * (boolCount + 1));
        for (int i = 1; i <= boolCount; i++) 
        {
            result[i].v = false;
            result[i].flag = false;
        }
    }

    // 1. 单子句规则
    int unitLiteral = FindUnitClause(cL);
    while (unitLiteral != 0)
    {
        result[abs(unitLiteral)].v = (unitLiteral > 0) ? TRUE : FALSE;
        result[abs(unitLiteral)].flag = TRUE;
        Simplify(cL, unitLiteral);
        printf("-------------------\n");
        PrintCnf(cL);
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

    // 2. 选择一个未赋值的文字
    int literal = ChooseLiteral();
    if (literal == 0) return OK; // 没有可以选择的文字

    // 3. 递归求解
    clauseList newCnf = CopyCnf(cL);
    Simplify(newCnf, literal);
    result[literal].v = TRUE;
    result[literal].flag = TRUE;
    if (DPLL(newCnf) == OK)
    {
        DestroyCnf(newCnf);
        return OK;
    }

    DestroyCnf(newCnf);
    newCnf = CopyCnf(cL);
    Simplify(newCnf, -literal);
    result[literal].v = FALSE;
    status res = DPLL(newCnf);
    DestroyCnf(newCnf); // 释放内存
    return res;
}
