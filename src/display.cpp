/*----------------------------display----------------------------*/

#include "SAT.h"

/*
 @ 函数名称: disPlay
 @ 接受参数: void
 @ 函数功能: 交互界面
 @ 返回值: void
 */
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

