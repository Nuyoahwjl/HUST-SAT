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
boolNode *result; //记录变元值的数组

/*函数声明*/
void disPlay(); //交互界面
void printMenu(); //打印菜单
status readFile(clauseList &cL); //读取文件并解析cnf
status destroyCnf(clauseList &cL); //销毁当前解析的cnf
status printCnf(clauseList cL); //打印cnf
status isUnitClause(literalList l); //判断是否为单子句
int findUnitClause(clauseList cL); //找到单子句 
status DPLL(clauseList cL); //DPLL算法

int main()
{
    disPlay();
    return 0;
}

void disPlay()
{
    // clauseList cL=NULL;
    printMenu();
    int op=1;
    while(op)
    {
        printf("\n|--------------------------------------------|\n");
        printf(  "|--------Please Choose Your Operation--------|\n");
        printf(  "|--------------------------------------------|\n\n");
        printf("               Your choice: ");
        scanf("%d", &op);
        system("cls");
        printMenu();
        switch (op)
        {
            case 1:
            { 
                readFile(cL);
                break;
            }
            case 2:
            {
                if(cL==NULL)
                    printf(" You haven't open the CNF file.\n");
                else
                    printCnf(cL);
                break;
            }
            case 3:
            {
                if(cL==NULL)
                {
                    printf(" You haven't open the CNF file.\n");
                    break;
                }
                result=(boolNode*)malloc(sizeof(boolNode)*(boolCount+1));
                for(int i=1;i<=boolCount;i++)
                {
                    result[i].v=FALSE;
                    result[i].flag=FALSE;
                }
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
void printMenu()
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

/*
 @ 函数名称: ReadFile
 @ 接受参数: clauseList &
 @ 函数功能: 用文件指针fp打开用户指定的文件，并读取文件内容保存到给定参数中
 @ 返回值: int
 */
status readFile(clauseList &cL)
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
            destroyCnf(cL);
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
status destroyCnf(clauseList &cL)
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
status printCnf(clauseList cL)
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