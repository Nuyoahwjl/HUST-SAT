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
 @ 函数名称: DPLL
 @ 接受参数: clauseList
 @ 函数功能: DPLL算法求解SAT问题
 @ 返回值: int
 */
// status DPLL(clauseList cL)
// {
    
// }
