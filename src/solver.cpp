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
    if(l->next!=NULL && l->next->next==NULL)
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
    clauseList p=cL->next;
    while(p)
    {
        if(IsUnitClause(p->head))
            return p->head->next->literal;
        p=p->next;
    }
    return 0;
}

/*
 @ 函数名称: Simplify
 @ 接受参数: clauseList &, int
 @ 函数功能: 根据选择的文字化简
 @ 返回值: void
 */
void Simplify(clauseList &cL, int literal)
{
    clauseList pre=cL,p=cL->next;
    while(p!=NULL)
    {
        int flag=1;
        literalList lpre=p->head,q=p->head->next;
        while(q!=NULL)
        {
            if(q->literal==literal) //删除该子句
            {
                pre->next=p->next;
                p=p->next;
                flag=0;
                break;
            }
            else if(q->literal==-literal) //删除该文字
            {
                lpre->next=q->next;
                free(q);
                q=lpre->next;
            }
            else
            {
                lpre=q;
                q=q->next;
            }
        }
        if(flag)
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
//     int literal=findUnitClause(cL);
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
    clauseList newCnf = (clauseList)malloc(sizeof(clauseNode));
    newCnf->next = NULL;
    clauseList p = cL->next, q = newCnf;
    while (p) 
    {
        clauseList temp = (clauseList)malloc(sizeof(clauseNode));
        temp->head = (literalList)malloc(sizeof(literalNode));
        temp->head->next = NULL;
        literalList l = p->head->next, m = temp->head;
        while (l) 
        {
            literalList temp1 = (literalList)malloc(sizeof(literalNode));
            temp1->literal = l->literal;
            temp1->next = NULL;
            m->next = temp1;
            m = m->next;
            l = l->next;
        }
        m->next = NULL;
        temp->next = NULL;
        q->next = temp;
        q = q->next;
        p = p->next;
    }
    return newCnf;
}
// clauseList CopyCnf(clauseList cL)
// {
// clauseList newCnf = (clauseList)malloc(sizeof(clauseNode));
//     newCnf->head = NULL;
//     newCnf->next = NULL;
//     clauseList last = newCnf, p = cL->next;
//     while (p != NULL) 
//     {
//         clauseList newClause = (clauseList)malloc(sizeof(clauseNode));
//         newClause->head = (literalList)malloc(sizeof(literalNode));
//         newClause->next = NULL;
//         newClause->head->next = NULL;
//         literalList lastLiteral = newClause->head;
//         literalList q = p->head->next;
//         while (q != NULL) 
//         {
//             literalList newLiteral = (literalList)malloc(sizeof(literalNode));
//             newLiteral->literal = q->literal;
//             newLiteral->next = NULL;
//             lastLiteral->next = newLiteral;
//             lastLiteral = newLiteral;
//             q = q->next;
//         }
//         last->next = newClause;
//         last = newClause;
//         p = p->next;
//     }
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

    // 检查是否已经确定所有子句
    clauseList p = cL->next;
    while (p != NULL) 
    {
        literalList q = p->head->next;
        bool hasUnassignedLiteral = false;
        while (q != NULL) 
        {
            if (!result[abs(q->literal)].flag) 
            {
                hasUnassignedLiteral = true;
                break;
            }
            q = q->next;
        }
        if (!hasUnassignedLiteral) 
        {
            if (p->head->next == NULL) // 空子句
            {
                // printf(" UNSAT\n");
                return ERROR;
            }
        }
        p = p->next;
    }

    // 找到单子句并进行推理
    int unitLiteral = FindUnitClause(cL);
    if (unitLiteral != 0) 
    {
        result[abs(unitLiteral)].v = unitLiteral > 0 ? TRUE : FALSE;
        result[abs(unitLiteral)].flag = TRUE;
        // 对CNF进行简化
        Simplify(cL, unitLiteral);
        return DPLL(cL);
    }   

    // 选择一个未赋值的文字
    int chosenLiteral = ChooseLiteral();
    if (chosenLiteral == 0) 
        return OK;
    

    // 尝试将选择的文字设为真
    result[abs(chosenLiteral)].v = chosenLiteral > 0 ? TRUE : FALSE;
    result[abs(chosenLiteral)].flag = TRUE;
    clauseList newCnf = CopyCnf(cL);
    Simplify(newCnf, chosenLiteral);
    if (DPLL(newCnf) == OK)
        return OK;

    // 尝试将选择的文字设为假
    result[abs(chosenLiteral)].v = !(chosenLiteral > 0) ? TRUE : FALSE;
    result[abs(chosenLiteral)].flag = TRUE;
    DestroyCnf(newCnf);
    newCnf = CopyCnf(cL);
    Simplify(newCnf, -chosenLiteral);
    if (DPLL(newCnf) == OK) 
        return OK;
    
    // 无解
    // printf(" UNSAT\n");
    result[abs(chosenLiteral)].flag = FALSE;
    return ERROR;
}
