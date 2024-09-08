# pragma once

/*头文件*/
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include <winnt.h>

/*常量*/
#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define SIZE 9

/*状态码*/
typedef int status; 

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
    struct clauseNode *next; //指向下一个子句
}clauseNode, *clauseList;

/*变量*/
int boolCount; //布尔变元个数
int clauseCount; //子句个数
// char fileName[100]; //文件名
// int *value=NULL; //记录布尔变元值(真/假)的数组
// int board[SIZE+1][SIZE+1]; //数独棋盘
// bool isFixed[SIZE+1][SIZE+1]; //记录是否为提示数字

/*函数声明*/
void DisPlay(); //主交互界面
void PrintMenu(); //打印菜单
status ReadFile(clauseList &cL,char fileName[]); //读取文件并解析cnf
status DestroyCnf(clauseList &cL); //销毁当前解析的cnf
status PrintCnf(clauseList cL); //打印cnf
status DPLL(clauseList &cL,int value[]); //DPLL算法
status IsUnitClause(literalList l); //判断是否为单子句
int FindUnitClause(clauseList cL); //找到单子句并返回该文字
status DestroyClause(clauseList &cL); //销毁子句
int ChooseLiteral(clauseList cL); //选择一个未赋值的文字
void Simplify(clauseList &cL, int literal); //根据选择的文字化简
clauseList CopyCnf(clauseList cL); //复制cnf
status SaveResult(int result, double time, int value[],char fileName[]); //保存求解结果
void X_Sudoku(); //X数独
void PrintMenu_X(); //打印X数独菜单
status Generate_Sudoku(int board[SIZE+1][SIZE+1],int newBoard[SIZE+1][SIZE+1],int newBoard2[SIZE+1][SIZE+1],bool isFixed[SIZE+1][SIZE+1],int num,int value[SIZE*SIZE*SIZE+1]); //生成数独
status Is_Valid(int board[SIZE+1][SIZE+1],int row, int col, int v); //判断board[row][col]是否可以填入v
void Print_Sudoku(int board[SIZE+1][SIZE+1]); //打印数独
void Play_Sudoku(int board[SIZE+1][SIZE+1],bool isFixed[SIZE+1][SIZE+1]); //玩数独
status WriteToFile(int board[SIZE+1][SIZE+1],int num,char name[]); //将数独约束条件写入文件
status Slove(int board[SIZE+1][SIZE+1],int value[SIZE*SIZE*SIZE+1]); //求解数独
status Fill_Box(int board[SIZE+1][SIZE+1], int newBoard[SIZE+1][SIZE+1],int newBoard2[SIZE+1][SIZE+1],int rowStart, int colStart);
void Shuffle(int arr[], int n);