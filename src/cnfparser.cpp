/*----------------------------cnfparser----------------------------*/

#include "SAT.h"

/*
 @ 函数名称: ReadFile
 @ 接受参数: clauseList &,char[]
 @ 函数功能: 用文件指针fp打开用户指定的文件，并读取文件内容保存到给定参数中
 @ 返回值: status
 */
status ReadFile(clauseList &cL,char fileName[])
{
    FILE *fp = fopen(fileName, "r");
    while(fp==NULL)
    {
        printf(" File not found, please input again: ");
        scanf("%s", fileName);
        fp=fopen(fileName,"r");
    }
    char ch;
    while((ch=getc(fp))=='c')
    {
        while((ch=getc(fp))!='\n')
            continue;
    }
    getc(fp); getc(fp); getc(fp); getc(fp); //读取cnf
    fscanf(fp,"%d%d",&boolCount,&clauseCount);
    
    cL = NULL;
    clauseList lastClause = NULL; 
    for (int i = 0; i < clauseCount; i++) {
        clauseList newClause = (clauseList)malloc(sizeof(clauseNode));
        newClause->head = NULL;
        newClause->next = NULL;
        literalList lastLiteral = NULL;
        int number;
        fscanf(fp, "%d", &number);
        while (number != 0) 
        {
            literalList newLiteral = (literalList)malloc(sizeof(literalNode));
            newLiteral->literal = number;
            newLiteral->next = NULL;
            if (newClause->head == NULL) 
                newClause->head = newLiteral;
            else 
                lastLiteral->next = newLiteral;
            lastLiteral = newLiteral;
            fscanf(fp, "%d", &number);
        }
        if (cL == NULL)
            cL = newClause;
        else 
            lastClause->next = newClause;
        lastClause = newClause;
    }
    fclose(fp);
    return OK;
}

/*
 @ 函数名称: DestroyCnf
 @ 接受参数: clauseList &
 @ 函数功能: 销毁给定的CNF文件
 @ 返回值: status
 */
status DestroyCnf(clauseList &cL)
{
    while (cL != NULL)
    {
        clauseList tempClause = cL;
        cL = cL->next;

        literalList p = tempClause->head;
        while (p != NULL)
        {
            literalList tempLiteral = p;
            p = p->next;
            free(tempLiteral);
        }
        
        free(tempClause);
    }

    cL = NULL;
    return OK;
}

/*
 @ 函数名称: PrintCnf
 @ 接受参数: clauseList
 @ 函数功能: 打印给定的CNF文件
 @ 返回值: status
 */
status PrintCnf(clauseList cL)
{
    clauseList p = cL;
    if(p==NULL)
    {
        printf(" No clauses.\n");
        return ERROR;
    }
    printf("  The CNF is:\n");
    printf("  boolCount:%d\n",boolCount);
    printf("  clauseCount:%d\n",clauseCount);
    while(p)
    {
        literalList q = p->head;
        printf("  ");
        while(q)
        {
            printf("%-5d", q->literal);
            q = q->next;
        }
        printf("0\n");
        p = p->next;
    }
    return OK;
}

