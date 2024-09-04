// g++ src/SAT.h src/display.cpp src/solver.cpp src/cnfparser.cpp src/X-Sudoku.cpp -o SAT

# pragma once

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
status DestroyClause(clauseList &cL); //销毁子句
int ChooseLiteral(); //(没有单子句时的策略)选择文字
// int ChooseLiteral(clauseList cL); //(没有单子句时的策略)选择文字
clauseList CopyCnf(clauseList cL); //复制cnf
status DPLL(clauseList cL); //DPLL算法