// g++ src/SAT.h src/display.cpp src/solver.cpp src/cnfparser.cpp src/X-Sudoku.cpp -o SAT

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
char fileName[100]; //文件名
int *value=NULL; //记录布尔变元值(真/假)的数组

/*函数声明*/
void DisPlay(); //交互界面
void PrintMenu(); //打印菜单
status ReadFile(clauseList &cL); //读取文件并解析cnf
status DestroyCnf(clauseList &cL); //销毁当前解析的cnf
status PrintCnf(clauseList cL); //打印cnf
status DPLL(clauseList &cL); //DPLL算法
status IsUnitClause(literalList l);
int FindUnitClause(clauseList cL);
status DestroyClause(clauseList &cL);
int ChooseLiteral(clauseList cL);
void Simplify(clauseList &cL, int literal);
clauseList CopyCnf(clauseList cL);
status SaveResult(int result, double time, int value[]);

// int isUnitClause(literalList cL);
// int emptyClause(clauseList cL);
// void CopyClause(clauseList &a, clauseList b);
// int removeClause(clauseList& p, clauseList& cL);
// int removeNode(literalList& q, literalList& head);
// int addClause(clauseList cnf, clauseList& root);

// typedef struct Result
// {
//     int v; //布尔变元值
//     int flag; //是否赋值
// }Result;
// Result *value=NULL;
