/*----------------------------display----------------------------*/

#include "SAT.h"

/*
 @ 函数名称: DisPlay
 @ 接受参数: void
 @ 函数功能: 交互界面
 @ 返回值: void
 */
void DisPlay()
{
    // int *value=NULL;
    clauseList cL=NULL;
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
                else
                {
                    value = (int *)malloc(sizeof(int) * (boolCount + 1));
                    for (int i = 1; i <= boolCount; i++)
                        value[i] = 1; // 初始化，均赋为1
                    int result = DPLL(cL);
                    if (result == OK)
                    {
                        for (int i = 1; i <= boolCount; i++)
                        {
                            if (value[i] == 1)
                                printf(" %-4d: TRUE\n",i);
                            else
                                printf(" %-4d: FALSE\n",i);
                        }
                    }
                    else printf(" UNSAT\n");
                }
                break;
            }
            case 4:
            {
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

