#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0

typedef int status; //状态码

/*文字节点&链表*/
typedef struct literalNode
{
    int literal; //文字(变元)
    struct literalNode *next; //指向下一个文字
}literalNode, *literalList;

/*子句节点&链表*/
typedef struct clauseNode
{
    literalList head; //指向子句中的第一个文字
    // int literalCount; //子句中文字个数
    struct clauseNode *next; //指向下一个子句
}clauseNode, *clauseList;

/*布尔变元的值*/
typedef struct boolNode
{
    bool v; //真或假
    bool flag; //是否被赋值
}boolNode;

clauseList cL=NULL; //CNF链表
int boolCount; //布尔变元个数
int clauseCount; //子句个数
char fileName[100]; //文件名
boolNode *result=NULL; //记录变元值的数组

/*函数声明*/
void DisPlay(); //交互界面
void PrintMenu(); //打印菜单
status ReadFile(clauseList &cL); //读取文件并解析cnf
status DestroyCnf(clauseList &cL); //销毁当前解析的cnf
status PrintCnf(clauseList cL); //打印cnf
status IsUnitClause(literalList l); //判断是否为单子句
int FindUnitClause(clauseList cL); //找到单子句 
void Simplify(clauseList &cL, int literal); //根据选择的文字化简
int ChooseLiteral(); //(没有单子句时的策略)选择文字
clauseList CopyCnf(clauseList cL);

status DPLL(clauseList cL); //DPLL算法

int main()
{
    DisPlay();
    return 0;
}

/*----------------------------display----------------------------*/

/*
 @ 函数名称: DisPlay
 @ 接受参数: void
 @ 函数功能: 交互界面
 @ 返回值: void
 */
void DisPlay()
{
    // clauseList cL=NULL;
    PrintMenu();
    int op=1;
    while(op)
    {
        printf("\n|--------------------------------------------|\n");
        printf(  "|--------Please Choose Your Operation--------|\n");
        printf(  "|--------------------------------------------|\n\n");
        printf("               Your choice: ");
        scanf("%d", &op);
        system("cls");
        PrintMenu();
        switch (op)
        {
            case 1:
            { 
                ReadFile(cL);
                break;
            }
            case 2:
            {
                if(cL==NULL)
                    printf(" You haven't open the CNF file.\n");
                else
                    PrintCnf(cL);
                break;
            }
            case 3:
            {
                if(cL==NULL)
                {
                    printf(" You haven't open the CNF file.\n");
                    break;
                }
                // result=(boolNode*)malloc(sizeof(boolNode)*(boolCount+1));
                // for(int i=1;i<=boolCount;i++)
                // {
                //     result[i].v=FALSE;
                //     result[i].flag=FALSE;
                // }
                else if(DPLL(cL)==OK)
                {
                    printf(" SAT\n");
                    for(int i=0;i<=boolCount;i++)
                    {
                        if(result[i].v==true)
                            printf(" %-4d: TRUE\n",i);
                        else 
                            printf(" %-4d: FALSE\n",i);
                    }
                }
                else printf(" UNSAT\n");
                break;
            }
            default:
            {
                printf(" Invalid input.\n");
                break;
            }        
        }
    }
}

/*
 @ 函数名称: PrintMenu
 @ 函数功能: 打印菜单
 @ 返回值: void
 */
void PrintMenu()
{
    printf("|================Menu for SAT================|\n");
    printf("|--------------------------------------------|\n");
    printf("|            1. Open the CNF file            |\n");
    printf("|     2. Traverse and output each clause     |\n");
    printf("|   3. Solve using DPLL and save the result  |\n");
    printf("|          4. Diagonal Sudoku game           |\n");
    printf("|                 0.  EXIT                   |\n");
    printf("|============================================|\n\n");
}

/*----------------------------cnfparser----------------------------*/

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

/*----------------------------solver----------------------------*/

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

