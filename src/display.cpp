/*----------------------------display----------------------------*/

#include "SAT.h"

/*
 @ 函数名称: DisPlay
 @ 函数功能: 交互界面
 @ 返回值: void
 */
void DisPlay()
{
    int *value=NULL;
    clauseList cL=NULL;
    char fileName[100];
    PrintMenu();
    int op=1;
    while(op)
    {
        printf("\n|--------------------------------------------|\n");
        printf(  "|--------Please Choose Your Operation--------|\n");
        printf(  "|--------------------------------------------|\n\n");
        printf(  "               Your choice: ");
        scanf("%d", &op);
        system("cls");
        PrintMenu();
        switch (op)
        {
            case 1:
            {
                if (cL != NULL)
                {
                    printf(" The CNF has been read.\n");
                    printf(" Do you want to read another? (1/0): ");
                    int choice;
                    scanf("%d", &choice);
                    if (choice == 0)
                        break;
                    else
                    {
                        DestroyCnf(cL);
                    }
                }
                printf(" Please input the file name: ");
                scanf("%s", fileName);
                if(ReadFile(cL, fileName)==OK)
                    printf(" Read successfully.\n");
                else
                    printf(" Read failed.\n");
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
                        {
                            value[i]= TRUE; //初始化，均赋为1
                        }
                    LARGE_INTEGER frequency,frequency_; //计时器频率
                    LARGE_INTEGER start, start_, end ,end_; //设置时间变量
                    double time,time_;
                    // 未优化的时间
                    QueryPerformanceFrequency(&frequency);
                    QueryPerformanceCounter(&start); //计时开始;
                    int result = DPLL(cL,value,1);
                    QueryPerformanceCounter(&end); //结束
                    time = (double)(end.QuadPart-start.QuadPart)/frequency.QuadPart; //计算运行时间
                    // 优化后的时间
                    QueryPerformanceFrequency(&frequency_);
                    QueryPerformanceCounter(&start_); //计时开始;
                    DPLL(cL,value,2);
                    QueryPerformanceCounter(&end_); //结束
                    time_ = (double)(end_.QuadPart-start_.QuadPart)/frequency_.QuadPart; //计算运行时间
                    // 输出SAT结果
                    if (result == OK)
                    {
                        printf(" SAT\n\n");
                        for (int i = 1; i <= boolCount; i++)
                        {
                            if (value[i]== TRUE)
                                printf(" %-4d: TRUE\n",i);
                            else
                                printf(" %-4d: FALSE\n",i);
                        }
                    }
                    else 
                        printf(" UNSAT\n");
                    // 输出优化前后的时间
                    printf("\n Time: %lf ms(not optimized)\n", time*1000);
                    printf("\n Time: %lf ms(optimized)\n", time_*1000);
                    // 是否保存
                    printf("\n Save the result to file? (1/0): ");
                    int choice;
                    scanf("%d", &choice);
                    printf("\n");
                    if (choice == 1)
                    {
                        if(SaveResult(result, time, time_, value,fileName))
                            printf(" Save successfully.\n");
                        else
                            printf(" Save failed.\n");
                    }
                }
                break;
            }
            case 4:
            {
                X_Sudoku();
                PrintMenu();
                break;
            }
            case 0:
            {
                printf(" Exit successfully.\n");
                break;
            }
            default:
            {
                printf(" Invalid input.\n");
                break;
            }        
        }
    }
    if(cL!=NULL)
        DestroyCnf(cL);
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
    printf("|              4. X_Sudoku game              |\n");
    printf("|                 0.  EXIT                   |\n");
    printf("|============================================|\n\n");
}

