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
    int board[SIZE+1][SIZE+1]; //数独棋盘
    bool isFixed[SIZE+1][SIZE+1]; //记录是否为提示数字
    int op=1;
    int flag=0;//是否生成数独
    int num;//生成数独的提示数字个数
    while(op)
    {
        printf("\n|********************************************|\n");
        printf(  "|--------Please Choose Your Operation--------|\n");
        printf(  "|********************************************|\n\n");
        printf(  "              Your choice: ");
        scanf("%d", &op);
        system("cls");
        PrintMenu_X();
        switch (op)
        {
            case 1:
            {
                printf(" Please enter the number of prompts: ");
                scanf("%d", &num);
                if(Generate_Sudoku(board,isFixed,num))
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
                    printf("Original X_Sudoku:\n");
                    Print_Sudoku(board);
                    printf("\n");
                    printf("Generating cnf file...\n");
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
    printf("|                 0.  EXIT                   |\n");
    printf("|********************************************|\n\n");
}

/*
 @ 函数名称: Generate_Sudoku
 @ 接受参数: int
 @ 函数功能: 生成数独
 @ 返回值: status
 */
status Generate_Sudoku(int board[SIZE+1][SIZE+1],bool isFixed[SIZE+1][SIZE+1],int num)
{
    srand(time(NULL));
    //初始化棋盘
    for(int i=1;i<=SIZE;i++)
        for(int j=1;j<=SIZE;j++)
            board[i][j]=0;
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
                isFixed[row][col]=TRUE;
                num--;
            }
        }
    }
    return OK;
}

/*
 @ 函数名称: Is_Valid
 @ 接受参数: int, int, int
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
 @ 接受参数: int
 @ 函数功能: 将数独约束条件写入文件
 @ 返回值: status
 */
status WriteToFile(int board[SIZE+1][SIZE+1],int num)
{
    
}