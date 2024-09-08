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
    int num; // 提示数的个数
    bool isFixed[SIZE + 1][SIZE + 1]; // 记录是否为提示数字
    int board[SIZE + 1][SIZE + 1]; // 生成的初始数独
    int newBoard[SIZE + 1][SIZE + 1]; // 用来玩的数独
    int newBoard2[SIZE + 1][SIZE + 1]; // 保存答案的数独
    int value[SIZE * SIZE * SIZE + 1]; // 记录DPLL的结果
    for (int i = 1; i <= SIZE * SIZE * SIZE; i++)
        value[i] = FALSE;
    int op = 1; // 操作
    int flag = 0; // 是否生成数独
    while (op)
    {
        printf("\n|********************************************|\n");
        printf(  "|--------Please Choose Your Operation--------|\n");
        printf(  "|********************************************|\n\n");
        printf("               Your choice: ");
        scanf("%d", &op);
        system("cls");
        PrintMenu_X();
        switch (op)
        {
        case 1:
        {
            printf(" Please enter the number of prompts(>=18): ");
            scanf("%d", &num);
            while(num<18||num>81)
            {
                printf(" Invalid input, please enter again: ");
                scanf("%d", &num);
            }
            if (Generate_Sudoku(board, newBoard, newBoard2,isFixed, num,value))
            {
                printf(" Generate successfully.\n");
                flag = 1;
            }
            else
                printf(" Generate failed.\n");
            break;
        }
        case 2:
        {
            if (flag)
            {
                Play_Sudoku(newBoard, isFixed);
                PrintMenu_X();
            }
            else
                printf(" Please generate the X_Sudoku first.\n");
            break;
        }
        case 3:
        {
            if (flag)
            {
                printf(" Original X_Sudoku:\n");
                Print_Sudoku(board);
                printf("\n");
                if(Slove(newBoard2,value))
                {
                    printf(" Reference answer:\n");
                    Print_Sudoku(newBoard2);
                }
                // WriteToFile(newBoard2, num,"X_Sudoku.cnf");
                // int value[SIZE * SIZE * SIZE + 1];
                // for (int i = 1; i <= SIZE * SIZE * SIZE; i++)
                //     value[i] = FALSE;
                // if (Slove(newBoard2, value))
                // {
                //     printf(" Reference answer:\n");
                //     Print_Sudoku(newBoard2);
                // }
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
    printf("|                 0. EXIT                    |\n");
    printf("|********************************************|\n\n");
}

/*
 @ 函数名称: Generate_Sudoku
 @ 接受参数: int[][],bool[][],int
 @ 函数功能: 生成数独
 @ 返回值: status
 */
status Generate_Sudoku(int board[SIZE+1][SIZE+1],int newBoard[SIZE+1][SIZE+1],int newBoard2[SIZE+1][SIZE+1],bool isFixed[SIZE+1][SIZE+1],int num,int value[SIZE*SIZE*SIZE+1])
{
    char name[100] = "X_Sudoku.cnf";
START:    
    srand(time(NULL));
    //初始化棋盘
    for(int i=1;i<=SIZE;i++)
        for(int j=1;j<=SIZE;j++)
        {
            board[i][j]=0;
            newBoard[i][j]=0;
            newBoard2[i][j]=0;
            isFixed[i][j]=TRUE;
        }
    int n[SIZE];
    for(int i=0;i<SIZE;i++)
        n[i]=i+1;
    Shuffle(n,SIZE);
    int index=0;
    for(int i=1;i<=SIZE;i++)
    {
        board[i][i] = n[index];
        newBoard[i][i] = n[index];
        newBoard2[i][i] = n[index];
        index++;
    }
    for (int i = 1; i <= SIZE; i += 3) {
        Fill_Box(board,newBoard,newBoard2, i, i); // 填充 3x3 的宫格
    }
    WriteToFile(board, 27, name);
    clauseList p = NULL;
    ReadFile(p, name);
    for (int i = 1; i <= SIZE * SIZE * SIZE; i++)
        value[i] = FALSE;
    if (DPLL(p, value) == ERROR)
        goto START;
    for (int i = 1; i <= SIZE * SIZE * SIZE + 1; i++)
    {
        if (value[i] == TRUE)
        {
            int row = (i - 1) / (SIZE * SIZE) + 1;
            int col = (i - 1) / SIZE % SIZE + 1;
            int v = (i - 1) % SIZE + 1;
            board[row][col] = v;
            newBoard[row][col] = v;
            newBoard2[row][col] = v;
        }
    }
    // 挖洞,剩下num个提示数
    // int res[SIZE+1]={9};
    int remove=81-num;
    while (remove > 0) {
        int row = rand() % SIZE + 1;
        // while(res[row]<=2)
        //     row = rand() % SIZE + 1;
        int col = rand() % SIZE + 1;
        if (board[row][col] != 0) 
        {
            board[row][col] = 0;
            newBoard[row][col]=0;
            newBoard2[row][col]=0;
            isFixed[row][col] = FALSE;
            remove--;
            // res[row]--;
        }
    }
    return OK;
}

/*
 @ 函数名称: Is_Valid
 @ 接受参数: int[][],int,int,int
 @ 函数功能: 判断board[row][col]是否可以填入v
 @ 返回值: status
 */
status Is_Valid(int board[SIZE + 1][SIZE + 1], int row, int col, int v)
{
    // 检查行和列
    for (int i = 1; i <= SIZE; i++)
    {
        if (board[row][i] == v || board[i][col] == v)
            return FALSE;
    }
    // 检查3x3宫格
    int startRow = (row - 1) / 3 * 3 + 1;
    int startCol = (col - 1) / 3 * 3 + 1;
    for (int i = startRow; i < startRow + 3; i++)
    {
        for (int j = startCol; j < startCol + 3; j++)
        {
            if (board[i][j] == v)
                return FALSE;
        }
    }
    // 检查对角线
    if (row == col)
    {
        for (int i = 1; i <= SIZE; i++)
        {
            if (board[i][i] == v)
                return FALSE;
        }
    }
    if (row + col == SIZE + 1)
    {
        for (int i = 1; i <= SIZE; i++)
        {
            if (board[i][SIZE - i + 1] == v)
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
void Print_Sudoku(int board[SIZE + 1][SIZE + 1])
{
    for (int i = 1; i <= SIZE; i++)
    {
        for (int j = 1; j <= SIZE; j++)
        {
            if (board[i][j] == 0)
                printf(" .");
            else
                printf("%2d", board[i][j]);
            if (j % 3 == 0 && j != SIZE)
                printf(" |");
        }
        printf("\n");
        if (i % 3 == 0 && i != SIZE)
            printf("-------+-------+-------\n");
    }
}

/*
 @ 函数名称: Play_Sudoku
 @ 接受参数: int[][], bool[][]
 @ 函数功能: 玩数独的交互界面
 @ 返回值: void
 */
void Play_Sudoku(int board[SIZE + 1][SIZE + 1], bool isFixed[SIZE + 1][SIZE + 1])
{
    system("cls");
    Print_Sudoku(board);
    printf("\n");
    while (1)
    {
        int row, col, v;
        printf(" Please enter the row, col and value(0 to EXIT): ");
        scanf("%d", &row);
        if (row == 0)
        {
            system("cls");
            return;
        }
        scanf("%d%d", &col, &v);
        if (row < 1 || row > SIZE || col < 1 || col > SIZE || v < 1 || v > SIZE)
        {
            printf(" Invalid input.\n");
            continue;
        }
        if (isFixed[row][col])
        {
            printf(" This is a fixed number.\n");
            continue;
        }
        if (!Is_Valid(board, row, col, v))
        {
            printf(" Wrong answer.\n");
            continue;
        }
        else
        {
            board[row][col] = v;
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
status WriteToFile(int board[SIZE + 1][SIZE + 1], int num, char name[])
{
    FILE *fp;
    if (fopen_s(&fp, name, "w"))
    {
        printf(" Fail!\n");
        return ERROR;
    }
    fprintf(fp, "c %s\n", name);
    fprintf(fp, "p cnf 729 %d\n", num + 12654);
    // 提示数约束
    for (int i = 1; i <= SIZE; i++)
    {
        for (int j = 1; j <= SIZE; j++)
        {
            if (board[i][j] != 0)
                fprintf(fp, "%d 0\n", (i - 1) * SIZE * SIZE + (j - 1) * SIZE + board[i][j]);
        }
    }
    // 每个格子的约束
    for (int i = 1; i <= SIZE; i++)
    {
        for (int j = 1; j <= SIZE; j++)
        {
            for (int k = 1; k <= SIZE; k++)
            {
                fprintf(fp, "%d ", (i - 1) * SIZE * SIZE + (j - 1) * SIZE + k);
            }
            fprintf(fp, "0\n");
        }
    }
    for (int i = 1; i <= SIZE; i++)
    {
        for (int j = 1; j <= SIZE; j++)
        {
            for (int k = 1; k <= SIZE; k++)
            {
                for (int l = k + 1; l <= SIZE; l++)
                {
                    fprintf(fp, "%d %d 0\n", 0 - ((i - 1) * SIZE * SIZE + (j - 1) * SIZE + k), 0 - ((i - 1) * SIZE * SIZE + (j - 1) * SIZE + l));
                }
            }
        }
    }
    // 行约束
    for (int i = 1; i <= SIZE; i++)
    {
        for (int j = 1; j <= SIZE; j++)
        {
            for (int k = 1; k <= SIZE; k++)
            {
                fprintf(fp, "%d ", (i - 1) * SIZE * SIZE + (k - 1) * SIZE + j);
            }
            fprintf(fp, "0\n");
        }
    }
    for (int i = 1; i <= SIZE; i++)
    {
        for (int j = 1; j <= SIZE; j++)
        {
            for (int k = 1; k <= SIZE; k++)
            {
                for (int l = k + 1; l <= SIZE; l++)
                {
                    fprintf(fp, "%d %d 0\n", 0 - ((i - 1) * SIZE * SIZE + (k - 1) * SIZE + j), 0 - ((i - 1) * SIZE * SIZE + (l - 1) * SIZE + j));
                }
            }
        }
    }
    // 列约束
    for (int i = 1; i <= SIZE; i++)
    {
        for (int j = 1; j <= SIZE; j++)
        {
            for (int k = 1; k <= SIZE; k++)
            {
                fprintf(fp, "%d ", (k - 1) * SIZE * SIZE + (i - 1) * SIZE + j);
            }
            fprintf(fp, "0\n");
        }
    }
    for (int i = 1; i <= SIZE; i++)
    {
        for (int j = 1; j <= SIZE; j++)
        {
            for (int k = 1; k <= SIZE; k++)
            {
                for (int l = k + 1; l <= SIZE; l++)
                {
                    fprintf(fp, "%d %d 0\n", 0 - ((k - 1) * SIZE * SIZE + (i - 1) * SIZE + j), 0 - ((l - 1) * SIZE * SIZE + (i - 1) * SIZE + j));
                }
            }
        }
    }
    // 3x3宫格约束
    for (int i = 1; i <= SIZE; i += 3)
    {
        for (int j = 1; j <= SIZE; j += 3)
        {
            for (int k = 1; k <= SIZE; k++)
            {
                for (int l = 0; l < 3; l++)
                {
                    for (int m = 0; m < 3; m++)
                    {
                        fprintf(fp, "%d ", ((i + l - 1) * SIZE * SIZE + (j + m - 1) * SIZE + k));
                    }
                }
                fprintf(fp, "0\n");
            }
        }
    }
    for (int i = 1; i <= SIZE; i += 3)
    {
        for (int j = 1; j <= SIZE; j += 3)
        {
            for (int k = 1; k <= SIZE; k++)
            {
                for (int l = 0; l < 3; l++)
                {
                    for (int m = 0; m < 3; m++)
                    {
                        for (int n = k + 1; n <= SIZE; n++)
                        {
                            fprintf(fp, "%d %d 0\n", 0 - ((i + l - 1) * SIZE * SIZE + (j + m - 1) * SIZE + k), 0 - ((i + l - 1) * SIZE * SIZE + (j + m - 1) * SIZE + n));
                        }
                    }
                }
            }
        }
    }
    // 对角线约束
    for (int i = 1; i <= SIZE; i++)
    {
        for (int j = 1; j <= SIZE; j++)
        {
            fprintf(fp, "%d ", (j - 1) * SIZE * SIZE + (j - 1) * SIZE + i);
        }
        fprintf(fp, "0\n");
    }
    for (int i = 1; i <= SIZE; i++)
    {
        for (int j = 1; j <= SIZE; j++)
        {
            for (int k = j + 1; k <= SIZE; k++)
            {
                fprintf(fp, "%d %d 0\n", 0 - ((j - 1) * SIZE * SIZE + (j - 1) * SIZE + i), 0 - ((k - 1) * SIZE * SIZE + (k - 1) * SIZE + i));
            }
        }
    }
    for (int i = 1; i <= SIZE; i++)
    {
        for (int j = 1; j <= SIZE; j++)
        {
            fprintf(fp, "%d ", (j - 1) * SIZE * SIZE + (SIZE - j + 1) * SIZE + i);
        }
        fprintf(fp, "0\n");
    }
    for (int i = 1; i <= SIZE; i++)
    {
        for (int j = 1; j <= SIZE; j++)
        {
            for (int k = j + 1; k <= SIZE; k++)
            {
                fprintf(fp, "%d %d 0\n", 0 - ((j - 1) * SIZE * SIZE + (SIZE - j + 1) * SIZE + i), 0 - ((k - 1) * SIZE * SIZE + (SIZE - k + 1) * SIZE + i));
            }
        }
    }
    fclose(fp);
    return OK;
}

/*
 @ 函数名称: Slove
 @ 接受参数: int[][],int[]
 @ 函数功能: DPLL求解数独
 @ 返回值: status
 */
status Slove(int board[SIZE + 1][SIZE + 1], int value[SIZE * SIZE * SIZE + 1])
{
    // clauseList p=NULL;
    // char name[100]="X_Sudoku.cnf";
    // if(ReadFile(p,name)==OK)
    // if(DPLL(p,value)==ERROR)
    //     return ERROR;
    for (int i = 1; i <= SIZE * SIZE * SIZE + 1; i++)
    {
        if (value[i] == TRUE)
        {
            int row = (i - 1) / (SIZE * SIZE) + 1;
            int col = (i - 1) / SIZE % SIZE + 1;
            int v = (i - 1) % SIZE + 1;
            board[row][col] = v;
        }
    }
    return OK;
}

/*
 @ 函数名称: Fill_Box
 @ 接受参数: int[][],int[][],int[][],int,int
 @ 函数功能: 填充3x3的宫格
 @ 返回值: status
 */
status Fill_Box(int board[SIZE+1][SIZE+1],int newBoard[SIZE+1][SIZE+1],int newBoard2[SIZE+1][SIZE+1],int rowStart, int colStart) 
{
    int n[SIZE-3];
    bool flag[SIZE]={0};
    for(int i=0;i<3;i++)
    {
        flag[board[rowStart+i][colStart+i]-1]=1;
    }
    int index=0;
    for(int i=0;i<SIZE;i++)
    {
        if(!flag[i])
            n[index++]=i+1;
    }
    Shuffle(n,6);
    index=0;
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(board[rowStart+i][colStart+j]==0)
            {
                board[rowStart+i][colStart+j] = n[index];
                newBoard[rowStart+i][colStart+j] = n[index];
                newBoard2[rowStart+i][colStart+j] = n[index];
                index++;
            }
        }
    }
    return OK;
}

/*
 @ 函数名称: Shuffle
 @ 接受参数: int[],int
 @ 函数功能: 洗牌算法,打乱数组顺序
 @ 返回值: void
 */
void Shuffle(int arr[], int n) {
    srand(time(NULL));
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}


// status Generate_Sudoku(int board[SIZE + 1][SIZE + 1], int newBoard[SIZE + 1][SIZE + 1], int newBoard2[SIZE+1][SIZE+1],bool isFixed[SIZE + 1][SIZE + 1], int num)
// {
//     // char temp_name[100] = "temp.cnf";
//     // int n = num;
// // START:
//     srand(time(NULL));
//     // 初始化棋盘
//     for (int i = 1; i <= SIZE; i++)
//         for (int j = 1; j <= SIZE; j++)
//         {
//             board[i][j] = 0;
//             newBoard[i][j] = 0;
//             newBoard2[i][j] = 0;
//             isFixed[i][j] = FALSE;
//         }
//     // 生成数独
//     while (num)
//     {
//         int row = rand() % SIZE + 1;
//         int col = rand() % SIZE + 1;
//         if (board[row][col] == 0)
//         {
//             int v = rand() % SIZE + 1;
//             if (Is_Valid(board, row, col, v))
//             {
//                 board[row][col] = v;
//                 newBoard[row][col] = v;
//                 newBoard2[row][col] = v;
//                 isFixed[row][col] = TRUE;
//                 num--;
//             }
//         }
//     }
//     // WriteToFile(board, n, temp_name);
//     // clauseList p = NULL;
//     // ReadFile(p, temp_name);
//     // int v[SIZE*SIZE*SIZE+1];
//     // for (int i = 1; i <= SIZE * SIZE * SIZE; i++)
//     //     v[i] = FALSE;
//     // if (DPLL(p, v) == ERROR)
//     //     goto START;
//     return OK;
// }
