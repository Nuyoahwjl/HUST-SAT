/*----------------------------solver----------------------------*/

#include "SAT.h"

/*
 @ 函数名称: isUnitClause
 @ 接受参数: literalList
 @ 函数功能: 判断是否为单子句
 @ 返回值: int
 */
status isUnitClause(literalList l)
{
    if(l->next!=NULL && l->next->next==NULL)
        return TRUE;
    else
        return FALSE;
}

/*
 @ 函数名称: findUnitClause
 @ 接受参数: clauseList
 @ 函数功能: 找到单子句并返回该文字
 @ 返回值: int
 */
int findUnitClause(clauseList cL)
{
    clauseList p=cL->next;
    while(p)
    {
        if(isUnitClause(p->head))
            return p->head->next->literal;
        p=p->next;
    }
    return 0;
}







/*
 @ 函数名称: DPLL
 @ 接受参数: clauseList
 @ 函数功能: DPLL算法求解SAT问题
 @ 返回值: int
 */
status DPLL(clauseList cL)
{

}
