/*----------------------------X-Sudoku----------------------------*/

#include "SAT.h"

/*
 @ 函数名称: X_Sudoku
 @ 函数功能: X数独交互界面
 @ 返回值: void
 */
void X_Sudoku()
{
    system("cls");
    PrintMenu_X();
    int board[SIZE+1][SIZE+1]={
        {0,0,0,0,0,0,0,0,0,0},
        {0,9,2,4,3,1,7,0,0,6},
        {0,0,0,1,0,0,6,4,0,3},
        {0,6,3,5,0,8,0,0,0,2},
        {0,0,0,3,0,9,8,0,0,7},
        {0,1,4,0,0,0,5,6,2,8},
        {0,2,0,7,1,0,4,0,3,0},
        {0,7,0,0,6,4,3,8,5,9},
        {0,0,5,8,9,7,1,2,6,4},
        {0,4,9,0,0,0,2,3,0,0}
    }; //数独棋盘
    bool isFixed[SIZE+1][SIZE+1]; //记录是否为提示数字
    int newBoard[SIZE+1][SIZE+1]={
        {0,0,0,0,0,0,0,0,0,0},
        {0,9,2,4,3,1,7,0,0,6},
        {0,0,0,1,0,0,6,4,0,3},
        {0,6,3,5,0,8,0,0,0,2},
        {0,0,0,3,0,9,8,0,0,7},
        {0,1,4,0,0,0,5,6,2,8},
        {0,2,0,7,1,0,4,0,3,0},
        {0,7,0,0,6,4,3,8,5,9},
        {0,0,5,8,9,7,1,2,6,4},
        {0,4,9,0,0,0,2,3,0,0}
    }; //用来求解答案
    int op=1;
    int flag=0;//是否生成数独
    int num=50;//生成数独的提示数字个数
    while(op)
    {
        printf("\n|********************************************|\n");
        printf(  "|--------Please Choose Your Operation--------|\n");
        printf(  "|********************************************|\n\n");
        printf(  "               Your choice: ");
        scanf("%d", &op);
        system("cls");
        PrintMenu_X();
        switch (op)
        {
            case 1:
            {
                printf(" Please enter the number of prompts: ");
                scanf("%d", &num);
                if(Generate_Sudoku(board,newBoard,isFixed,num))
                {
                    printf(" Generate successfully.\n");
                    flag=1;
                }
                else
                    printf(" Generate failed.\n");
                break;
            }
            case 2:
            {
                if(flag)
                {
                    Play_Sudoku(board,isFixed);
                    PrintMenu_X();
                }
                else 
                    printf(" Please generate the X_Sudoku first.\n");
                break;
            }
            case 3:
            {
                if(flag)
                {
                    printf(" Original X_Sudoku:\n");
                    Print_Sudoku(board);
                    printf("\n");
                    WriteToFile(newBoard,num);
                    int value[SIZE*SIZE*SIZE+1];
                    for(int i=1;i<=SIZE*SIZE*SIZE;i++)
                        value[i]=FALSE;
                    if(Slove(newBoard,value))
                    {
                        printf(" Reference answer:\n");
                        Print_Sudoku(newBoard);
                    }
                    else 
                        printf(" No answer.\n");
                }
                else 
                    printf(" Please generate the X_Sudoku first.\n");
                break;
            }
            case 0:
            {
                system("cls");
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
 @ 函数名称: PrintMenu_X
 @ 函数功能: 打印X数独菜单
 @ 返回值: void
 */
void PrintMenu_X()
{
    printf("|**************Menu for X_Sudoku*************|\n");
    printf("|--------------------------------------------|\n");
    printf("|            1. Generate a X_Sudoku          |\n");
    printf("|            2. Play the X_Sudoku            |\n");
    printf("|            3. Reference answer             |\n");
    printf("|                 0. EXIT                   |\n");
    printf("|********************************************|\n\n");
}

/*
 @ 函数名称: Generate_Sudoku
 @ 接受参数: int[][],bool[][],int
 @ 函数功能: 生成数独
 @ 返回值: status
 */
status Generate_Sudoku(int board[SIZE+1][SIZE+1],int newBoard[SIZE+1][SIZE+1],bool isFixed[SIZE+1][SIZE+1],int num)
{
    srand(time(NULL));
    //初始化棋盘
    for(int i=1;i<=SIZE;i++)
        for(int j=1;j<=SIZE;j++)
        {
            board[i][j]=0;
            newBoard[i][j]=0;
        }
    //初始化都不为提示数字
    for(int i=1;i<=SIZE;i++)
        for(int j=1;j<=SIZE;j++)
            isFixed[i][j]=FALSE;
    //生成数独
    while(num)
    {
        int row=rand()%SIZE+1;
        int col=rand()%SIZE+1;
        if(board[row][col]==0)
        {
            int v=rand()%SIZE+1;
            if(Is_Valid(board,row,col,v))
            {
                board[row][col]=v;
                newBoard[row][col]=v;
                isFixed[row][col]=TRUE;
                num--;
            }
        }
    }
    return OK;
    // for(int i=1;i<=SIZE;i++)
    //     for(int j=1;j<=SIZE;j++)
    //         isFixed[i][j]=FALSE;
    // Solve_Sudoku(board, 1, 1);
    // Remove_Numbers(board, 81-num);
    // for(int i=1;i<=SIZE;i++)
    // {
    //     for(int j=1;j<=SIZE;j++)
    //     {
    //         newBoard[i][j]=board[i][j];
    //         if(board[i][j]!=0)
    //             isFixed[i][j]=TRUE;
    //     }
    // }
}

/*
 @ 函数名称: Is_Valid
 @ 接受参数: int[][],int,int,int
 @ 函数功能: 判断board[row][col]是否可以填入v
 @ 返回值: status
 */
status Is_Valid(int board[SIZE+1][SIZE+1],int row, int col, int v)
{
    //检查行和列
    for(int i=1;i<=SIZE;i++)
    {
        if(board[row][i]==v || board[i][col]==v)
            return FALSE;
    }
    //检查3x3宫格
    int startRow=(row-1)/3*3+1;
    int startCol=(col-1)/3*3+1;
    for(int i=startRow;i<startRow+3;i++)
    {
        for(int j=startCol;j<startCol+3;j++)
        {
            if(board[i][j]==v)
                return FALSE;
        }
    }
    //检查对角线
    if(row==col)
    {
        for(int i=1;i<=SIZE;i++)
        {
            if(board[i][i]==v)
                return FALSE;
        }
    }
    if(row+col==SIZE+1)
    {
        for(int i=1;i<=SIZE;i++)
        {
            if(board[i][SIZE-i+1]==v)
                return FALSE;
        }
    }
    return TRUE;
}

/*
 @ 函数名称: Print_Sudoku
 @ 接受参数: int[][]
 @ 函数功能: 打印数独
 @ 返回值: void
 */
void Print_Sudoku(int board[SIZE+1][SIZE+1])
{
    for(int i=1;i<=SIZE;i++)
    {
        for(int j=1;j<=SIZE;j++)
        {
            if(board[i][j]==0)
                printf(" .");
            else
                printf("%2d",board[i][j]);
            if(j%3==0 && j!=SIZE)
                printf(" |");
        }
        printf("\n");
        if(i%3==0 && i!=SIZE)
            printf("-------+-------+-------\n");
    }
}

/*
 @ 函数名称: Play_Sudoku
 @ 接受参数: int[][], bool[][]
 @ 函数功能: 玩数独的交互界面
 @ 返回值: void
 */
void Play_Sudoku(int board[SIZE+1][SIZE+1],bool isFixed[SIZE+1][SIZE+1])
{
    system("cls");
    Print_Sudoku(board);
    printf("\n");
    while(1)
    {
        int row,col,v;
        printf(" Please enter the row, col and value(0 to EXIT): ");
        scanf("%d", &row);
        if(row==0)
        {
            system("cls");
            return;
        }
        scanf("%d%d", &col, &v);
        if(isFixed[row][col])
        {
            printf(" This is a fixed number.\n");
            continue;
        }
        if (row < 1 || row > SIZE || col < 1 || col > SIZE || v < 1 || v > SIZE)
        {
            printf(" Invalid input.\n");
            continue;
        }
        if(!Is_Valid(board,row,col,v))
        {
            printf(" Wrong answer.\n");
            continue;
        }
        else
        {
            board[row][col]=v;
            system("cls");
            Print_Sudoku(board);
            printf("\n");
        }
    }
}

/*
 @ 函数名称: WriteToFile
 @ 接受参数: int[][],int
 @ 函数功能: 将数独约束条件写入文件
 @ 返回值: status
 */
status WriteToFile(int board[SIZE+1][SIZE+1],int num)
{
    FILE *fp;
    if(fopen_s(&fp, "X_Sudoku.cnf", "w"))
    {
        printf(" Fail!\n");
        return ERROR;
    }
    fprintf(fp,"c X_Sudoku\n");
    fprintf(fp,"p cnf 729 %d\n", num+12654);
    //提示数约束
    for(int i=1;i<=SIZE;i++)
    {
        for(int j=1;j<=SIZE;j++)
        {
            if(board[i][j]!=0)
                fprintf(fp, "%d 0\n", (i-1)*SIZE*SIZE+(j-1)*SIZE+board[i][j]);
        }
    }
    //每个格子的约束
    for(int i=1;i<=SIZE;i++)
    {
        for(int j=1;j<=SIZE;j++)
        {
            for(int k=1;k<=SIZE;k++)
            {
                fprintf(fp, "%d ", (i-1)*SIZE*SIZE+(j-1)*SIZE+k);
            }
            fprintf(fp, "0\n");
        }
    }
    for(int i=1;i<=SIZE;i++)
    {
        for(int j=1;j<=SIZE;j++)
        {
            for(int k=1;k<=SIZE;k++)
            {
                for(int l=k+1;l<=SIZE;l++)
                {
                    fprintf(fp,"%d %d 0\n",0-((i-1)*SIZE*SIZE+(j-1)*SIZE+k),0-((i-1)*SIZE*SIZE+(j-1)*SIZE+l));
                }
            }
        }
    }
    //行约束
    for(int i=1;i<=SIZE;i++)
    {
        for(int j=1;j<=SIZE;j++)
        {
            for(int k=1;k<=SIZE;k++)
            {
                fprintf(fp,"%d ",(i-1)*SIZE*SIZE+(k-1)*SIZE+j);
            }
            fprintf(fp, "0\n");
        }
    }
    for(int i=1;i<=SIZE;i++)
    {
        for(int j=1;j<=SIZE;j++)
        {
            for(int k=1;k<=SIZE;k++)
            {
                for(int l=k+1;l<=SIZE;l++)
                {
                    fprintf(fp,"%d %d 0\n",0-((i-1)*SIZE*SIZE+(k-1)*SIZE+j),0-((i-1)*SIZE*SIZE+(l-1)*SIZE+j));
                }
            }
        }
    }
    //列约束
    for(int i=1;i<=SIZE;i++)
    {
        for(int j=1;j<=SIZE;j++)
        {
            for(int k=1;k<=SIZE;k++)
            {
                fprintf(fp,"%d ",(k-1)*SIZE*SIZE+(i-1)*SIZE+j);
            }
            fprintf(fp, "0\n");
        }
    }
    for(int i=1;i<=SIZE;i++)
    {
        for(int j=1;j<=SIZE;j++)
        {
            for(int k=1;k<=SIZE;k++)
            {
                for(int l=k+1;l<=SIZE;l++)
                {
                    fprintf(fp,"%d %d 0\n",0-((k-1)*SIZE*SIZE+(i-1)*SIZE+j),0-((l-1)*SIZE*SIZE+(i-1)*SIZE+j));
                }
            }
        }
    }
    //3x3宫格约束
    for(int i=1;i<=SIZE;i+=3)
    {
        for(int j=1;j<=SIZE;j+=3)
        {
            for(int k=1;k<=SIZE;k++)
            {
                for(int l=0;l<3;l++)
                {
                    for(int m=0;m<3;m++)
                    {
                        fprintf(fp,"%d ",((i+l-1)*SIZE*SIZE+(j+m-1)*SIZE+k));
                    }
                }
                fprintf(fp, "0\n");
            }
        }
    }
    for(int i=1;i<=SIZE;i+=3)
    {
        for(int j=1;j<=SIZE;j+=3)
        {
            for(int k=1;k<=SIZE;k++)
            {
                for(int l=0;l<3;l++)
                {
                    for(int m=0;m<3;m++)
                    {
                        for(int n=k+1;n<=SIZE;n++)
                        {
                            fprintf(fp,"%d %d 0\n",0-((i+l-1)*SIZE*SIZE+(j+m-1)*SIZE+k),0-((i+l-1)*SIZE*SIZE+(j+m-1)*SIZE+n));
                        }
                    }
                }
            }
        }
    }
    //对角线约束
    for(int i=1;i<=SIZE;i++)
    {
        for(int j=1;j<=SIZE;j++)
        {
            fprintf(fp,"%d ",(j-1)*SIZE*SIZE+(j-1)*SIZE+i);
        }
        fprintf(fp,"0\n");
    }
    for(int i=1;i<=SIZE;i++)
    {
        for(int j=1;j<=SIZE;j++)
        {
            for(int k=j+1;k<=SIZE;k++)
            {
                fprintf(fp,"%d %d 0\n",0-((j-1)*SIZE*SIZE+(j-1)*SIZE+i),0-((k-1)*SIZE*SIZE+(k-1)*SIZE+i));
            }
        }
    }
    for(int i=1;i<=SIZE;i++)
    {
        for(int j=1;j<=SIZE;j++)
        {
            fprintf(fp,"%d ",(j-1)*SIZE*SIZE+(SIZE-j+1)*SIZE+i);
        }
        fprintf(fp,"0\n");
    }
    for(int i=1;i<=SIZE;i++)
    {
        for(int j=1;j<=SIZE;j++)
        {
            for(int k=j+1;k<=SIZE;k++)
            {
                fprintf(fp,"%d %d 0\n",0-((j-1)*SIZE*SIZE+(SIZE-j+1)*SIZE+i),0-((k-1)*SIZE*SIZE+(SIZE-k+1)*SIZE+i));
            }
        }
    }
    fclose(fp);
    return OK;
}

/*
 @ 函数名称: Slove
 @ 接受参数: int[][],int[]
 @ 函数功能: 求解数独
 @ 返回值: status
 */
status Slove(int board[SIZE+1][SIZE+1],int value[SIZE*SIZE*SIZE+1])
{
    clauseList p=NULL;
    char name[100]="X_Sudoku.cnf";
    if(ReadFile(p,name)==OK)
    if(DPLL(p,value)==ERROR)
        return ERROR;
    for(int i=1;i<=SIZE*SIZE*SIZE+1;i++)
    {
        if(value[i]==TRUE)
        {
            int row=(i-1)/(SIZE*SIZE)+1;
            int col=(i-1)/SIZE%SIZE+1;
            int v=(i-1)%SIZE+1;
            board[row][col]=v;
        }
    }
    return OK;
}


// bool Solve_Sudoku(int board[SIZE+1][SIZE+1], int row, int col) {
//     // 如果到达最后一行，返回成功
//     if (row == SIZE+1) {
//         return true;
//     }
//     // 如果列超出范围，跳到下一行
//     if (col == SIZE+1) {
//         return Solve_Sudoku(board, row + 1, 0);
//     }
//     // 如果当前位置已填充，跳到下一列
//     if (board[row][col] != 0) {
//         return Solve_Sudoku(board, row, col + 1);
//     }
//     // 尝试填充数字 1 到 9
//     for (int num = 1; num <= SIZE; num++) {
//         if (Is_Valid(board, row, col, num)) {
//             board[row][col] = num;
//             if (Solve_Sudoku(board, row, col + 1)) {
//                 return true;
//             }
//             // 回溯
//             board[row][col] = 0;
//         }
//     }
//     return false;
// }

// void Remove_Numbers(int board[SIZE+1][SIZE+1], int count) {
//     srand(time(NULL));
//     int removed = 0;
//     while (removed < count) {
//         // 随机选择行和列
//         int row = rand() % SIZE+1;
//         int col = rand() % SIZE+1;
//         // 如果当前位置不是空的，去掉该数字
//         if (board[row][col] != 0) {
//             board[row][col] = 0;
//             removed++;
//         }
//     }
// }
