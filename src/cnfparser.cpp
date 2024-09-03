/*----------------------------cnfparser----------------------------*/

#include "SAT.h"

/*
 @ 函数名称: ReadFile
 @ 接受参数: clauseList &
 @ 函数功能: 用文件指针fp打开用户指定的文件，并读取文件内容保存到给定参数中
 @ 返回值: int
 */
status ReadFile(clauseList &cL)
{
    if(cL!=NULL)
    {
        printf(" The CNF has been read.\n");
        printf(" Do you want to read another? (1/0): ");
        int choice;
        scanf("%d",&choice);
        if(choice==0)
            return OK;
        else
            DestroyCnf(cL);
    }
    printf(" Please input the file name: ");
    scanf("%s", fileName);
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
    {
    // cL = NULL;
    // clauseList lastClause = NULL; 
    // for (int i = 0; i < clauseCount; i++) {
    //     clauseList newClause = (clauseList)malloc(sizeof(clauseNode));
    //     newClause->literals = NULL;
    //     newClause->next = NULL;
    //     literalList lastLiteral = NULL;
    //     int number;
    //     fscanf(fp, "%d", &number);
    //     while (number != 0) 
    //     {
    //         literalList newLiteral = (literalList)malloc(sizeof(literalNode));
    //         newLiteral->literal = number;
    //         newLiteral->next = NULL;
    //         if (newClause->literals == NULL) 
    //             newClause->literals = newLiteral;
    //         else 
    //             lastLiteral->next = newLiteral;
    //         lastLiteral = newLiteral;
    //         fscanf(fp, "%d", &number);
    //     }
    //     if (cL == NULL)
    //         cL = newClause;
    //     else 
    //         lastClause->next = newClause;
    //     lastClause = newClause;
    // }
    }

    //Initialize the clause list with a head node
    cL = (clauseList)malloc(sizeof(clauseNode));
    cL->head = NULL; //Head node, no literal
    cL->next = NULL;
    
    clauseList lastClause = cL; //Start from the head node
    
    for (int i = 0; i < clauseCount; i++) {
        //Create a new clause node with a head node for literals
        clauseList newClause = (clauseList)malloc(sizeof(clauseNode));
        newClause->head = (literalList)malloc(sizeof(literalNode));
        newClause->head->next = NULL;
        newClause->next = NULL;

        literalList lastLiteral = newClause->head; //Start from the literal head node
        int number;
        fscanf(fp, "%d", &number);

        while (number != 0) {
            literalList newLiteral = (literalList)malloc(sizeof(literalNode));
            newLiteral->literal = number;
            newLiteral->next = NULL;

            lastLiteral->next = newLiteral;
            lastLiteral = newLiteral;

            fscanf(fp, "%d", &number);
        }

        lastClause->next = newClause;
        lastClause = newClause;
    }
    printf(" Read successfully.\n");
    fclose(fp);
    return OK;
}

/*
 @ 函数名称: DestroyCnf
 @ 接受参数: clauseList &
 @ 函数功能: 销毁给定的CNF文件
 @ 返回值: int
 */
status DestroyCnf(clauseList &cL)
{
    clauseList p=cL->next;
    while(p)
    {
        literalList q=p->head->next;
        while(q)
        {
            literalList temp=q;
            q=q->next;
            free(temp);
        }
        free(p->head);
        clauseList temp=p;
        p=p->next;
        free(temp);
    }

    free(cL);
    cL=NULL;
    return OK;
}

/*
 @ 函数名称: PrintCnf
 @ 接受参数: clauseList
 @ 函数功能: 打印给定的CNF文件
 @ 返回值: int
 */
status PrintCnf(clauseList cL)
{
    clauseList p = cL->next;
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
        literalList q = p->head->next;
        printf("  ");
        while(q)
        {
            printf("%-5d", q->literal);
            q = q->next;
        }
        printf(" 0\n");
        p = p->next;
    }
    return OK;
}
