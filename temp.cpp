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
    int literal;              // 文字(变元)
    struct literalNode *next; // 指向下一个文字
} literalNode, *literalList;

/*子句节点&链表*/
typedef struct clauseNode
{
    literalList head;        // 指向子句中的第一个文字
    struct clauseNode *next; // 指向下一个子句
} clauseNode, *clauseList;

/*变量*/
int boolCount;   // 布尔变元个数
int clauseCount; // 子句个数

/*函数声明*/
void DisPlay(); // 主交互界面
void PrintMenu(); // 打印菜单
status ReadFile(clauseList &cL,char fileName[]); // 读取文件并解析cnf
status DestroyCnf(clauseList &cL); // 销毁当前解析的cnf
status PrintCnf(clauseList cL); // 打印cnf
status DPLL(clauseList &cL,int value[],int flag); // DPLL算法
status IsUnitClause(literalList l); // 判断是否为单子句
int FindUnitClause(clauseList cL); // 找到单子句并返回该文字
status DestroyClause(clauseList &cL); // 销毁子句
int ChooseLiteral_1(clauseList cL); // 选择一个未赋值的文字(未优化)
int ChooseLiteral_2(clauseList cL); // 选择一个未赋值的文字(优化)
int ChooseLiteral_3(clauseList cL); // 改进2
void Simplify(clauseList &cL, int literal); // 根据选择的文字化简
clauseList CopyCnf(clauseList cL); // 复制cnf
status SaveResult(int result, double time, double time_, int value[],char fileName[]); // 保存求解结果

int main()
{
    DisPlay();
    return 0;
}

void DisPlay()
{
    int *value = NULL; // 存储文字的真值
    clauseList cL = NULL;
    char fileName[100]; // 文件名
    PrintMenu();        // 打印菜单
    int op = 1;
    while (op)
    {
        printf("\n|--------------------------------------------|\n");
        printf("|--------Please Choose Your Operation--------|\n");
        printf("|--------------------------------------------|\n\n");
        printf("               Your choice: ");
        scanf("%d", &op);
        system("cls"); // 每次进来清屏
        PrintMenu();   // 打印菜单
        switch (op)
        {
        case 1:
        {
            if (cL != NULL) // 如果已经打开了CNF文件
            {
                printf(" The CNF has been read.\n");
                printf(" Do you want to read another? (1/0): ");
                int choice;
                scanf("%d", &choice);
                if (choice == 0)
                    break;
                else // 重新读取
                {
                    DestroyCnf(cL); // 销毁当前解析的CNF
                }
            }
            printf(" Please input the file name: ");
            scanf("%s", fileName);
            if (ReadFile(cL, fileName) == OK) // 读取文件并解析CNF
                printf(" Read successfully.\n");
            else
                printf(" Read failed.\n");
            break;
        }
        case 2:
        {
            if (cL == NULL) // 如果没有打开CNF文件
                printf(" You haven't open the CNF file.\n");
            else
                PrintCnf(cL); // 打印CNF文件
            break;
        }
        case 3:
        {
            if (cL == NULL) // 如果没有打开CNF文件
            {
                printf(" You haven't open the CNF file.\n");
                break;
            }
            else
            {
                value = (int *)malloc(sizeof(int) * (boolCount + 1));
                for (int i = 1; i <= boolCount; i++)
                    value[i] = TRUE;                    // 初始化，均赋为TRUE
                LARGE_INTEGER frequency, frequency_;    // 计时器频率
                LARGE_INTEGER start, start_, end, end_; // 设置时间变量
                double time, time_;
                // 未优化的时间
                QueryPerformanceFrequency(&frequency);
                QueryPerformanceCounter(&start); // 计时开始;
                int result = DPLL(cL, value, 3);
                QueryPerformanceCounter(&end);                                       // 结束
                time = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart; // 计算运行时间
                // 优化后的时间
                // QueryPerformanceFrequency(&frequency_);
                // QueryPerformanceCounter(&start_); // 计时开始;
                // DPLL(cL, value, 3);
                // QueryPerformanceCounter(&end_);                                          // 结束
                // time_ = (double)(end_.QuadPart - start_.QuadPart) / frequency_.QuadPart; // 计算运行时间
                // 输出SAT结果
                if (result == OK) // SAT
                {
                    printf(" SAT\n\n");
                    // 输出文字的真值
                    for (int i = 1; i <= boolCount; i++)
                    {
                        if (value[i] == TRUE)
                            printf(" %-4d: TRUE\n", i);
                        else
                            printf(" %-4d: FALSE\n", i);
                    }
                }
                else // UNSAT
                    printf(" UNSAT\n");
                // 输出优化前后的时间
                time_=0;
                printf("\n Time: %lf ms(not optimized)\n", time * 1000);
                printf("\n Time: %lf ms(optimized)\n", time_ * 1000);
                // 是否保存
                printf("\n Save the result to file? (1/0): ");
                int choice;
                scanf("%d", &choice);
                printf("\n");
                if (choice == 1)
                {
                    // 保存求解结果
                    if (SaveResult(result, time, time_, value, fileName))
                        printf(" Save successfully.\n");
                    else
                        printf(" Save failed.\n");
                }
            }
            break;
        }
        case 4:
        {
            // X_Sudoku();  // X数独界面
            PrintMenu(); // 跳转回来时重新打印菜单
            break;
        }
        case 0:
        {
            printf(" Exit successfully.\n");
            return; // 退出
        }
        default:
        {
            printf(" Invalid input.\n"); // 无效输入
            break;
        }
        }
    }
    if (cL != NULL)
        DestroyCnf(cL); // 退出时销毁CNF
    return;
}


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

status ReadFile(clauseList &cL, char fileName[])
{
    FILE *fp = fopen(fileName, "r");
    while (fp == NULL)
    {
        printf(" File not found, please input again: ");
        scanf("%s", fileName);
        fp = fopen(fileName, "r");
    }
    char ch;
    // 跳过注释
    while ((ch = getc(fp)) == 'c')
    {
        while ((ch = getc(fp)) != '\n')
            continue;
    }
    // 跳过p cnf
    getc(fp);
    getc(fp);
    getc(fp);
    getc(fp);
    fscanf(fp, "%d%d", &boolCount, &clauseCount); // 读取布尔变元个数和子句个数
    cL = NULL;                                    // 初始化CNF
    clauseList lastClause = NULL;                 // 用于记录上一个一个子句
    for (int i = 0; i < clauseCount; i++)
    {
        // 读取子句
        clauseList newClause = (clauseList)malloc(sizeof(clauseNode));
        newClause->head = NULL;
        newClause->next = NULL;
        literalList lastLiteral = NULL; // 用于记录上一个文字
        int number;                     // 读取文字
        fscanf(fp, "%d", &number);
        while (number != 0)
        {
            literalList newLiteral = (literalList)malloc(sizeof(literalNode));
            newLiteral->literal = number;
            newLiteral->next = NULL;
            if (newClause->head == NULL) // 如果是第一个文字
                newClause->head = newLiteral;
            else
                lastLiteral->next = newLiteral;
            lastLiteral = newLiteral; // 更新上一个文字
            fscanf(fp, "%d", &number);
        }
        if (cL == NULL) // 如果是第一个子句
            cL = newClause;
        else
            lastClause->next = newClause;
        lastClause = newClause; // 更新上一个子句
    }
    fclose(fp);
    return OK;
}

status DestroyCnf(clauseList &cL)
{
    while (cL != NULL)
    {
        clauseList tempClause = cL;
        cL = cL->next;                    // 指向下一个子句
        literalList p = tempClause->head; // 指向当前子句的第一个文字
        while (p != NULL)
        {
            literalList tempLiteral = p;
            p = p->next;
            free(tempLiteral); // 释放文字
        }
        free(tempClause); // 释放子句
    }
    cL = NULL; // cL指向NULL
    return OK;
}

status PrintCnf(clauseList cL)
{
    clauseList p = cL;
    if (p == NULL) // 如果没有子句
    {
        printf(" No clauses.\n");
        return ERROR;
    }
    printf("  The CNF is:\n");
    printf("  boolCount:%d\n", boolCount);     // 打印布尔变元个数
    printf("  clauseCount:%d\n", clauseCount); // 打印子句个数
    while (p)
    {
        literalList q = p->head; // 指向子句的第一个文字
        printf("  ");
        while (q)
        {
            printf("%-5d", q->literal); // 打印文字
            q = q->next;
        }
        printf("0\n"); // 子句结束
        p = p->next;   // 指向下一个子句
    }
    return OK;
}

status IsUnitClause(literalList l)
{
	if (l != NULL && l->next == NULL) // 只有一个文字
		return TRUE;
	else
		return FALSE;
}

int FindUnitClause(clauseList cL)
{
	clauseList p = cL;
	while (p)
	{
		if (IsUnitClause(p->head))	 // 是单子句
			return p->head->literal; // 返回该文字
		p = p->next;
	}
	return 0;
}

status DestroyClause(clauseList &cL)
{
	literalList p = cL->head;
	while (p)
	{
		literalList temp = p;
		p = p->next; // 指向下一个文字
		free(temp);	 // 释放文字
	}
	free(cL);  // 释放子句
	cL = NULL; // cL指向NULL
	return OK;
}

void Simplify(clauseList &cL, int literal)
{
	clauseList pre = NULL, p = cL; // pre指向前一个子句
	while (p != NULL)
	{
		bool clauseDeleted = false;			  // 是否删除子句
		literalList lpre = NULL, q = p->head; // lpre指向前一个文字
		while (q != NULL)
		{
			if (q->literal == literal) // 删除该子句
			{
				if (pre == NULL) // 删除的是第一个子句
					cL = p->next;
				else // 删除的不是第一个子句
					pre->next = p->next;
				DestroyClause(p);					// 销毁该子句
				p = (pre == NULL) ? cL : pre->next; // 指向下一个子句
				clauseDeleted = true;				// 子句已删除
				break;
			}
			else if (q->literal == -literal) // 删除该文字
			{
				if (lpre == NULL) // 删除的是第一个文字
					p->head = q->next;
				else // 删除的不是第一个文字
					lpre->next = q->next;
				free(q);								   // 释放该文字
				q = (lpre == NULL) ? p->head : lpre->next; // 指向下一个文字
			}
			else // 未删除
			{
				lpre = q;
				q = q->next;
			}
		}
		if (!clauseDeleted) // 子句未删除
		{
			pre = p;
			p = p->next;
		}
	}
}

clauseList CopyCnf(clauseList cL)
{
	// 初始化新的CNF
	clauseList newCnf = (clauseList)malloc(sizeof(clauseNode));
	clauseList lpa, lpb;  // lpa指向新的子句,lpb指向旧的子句
	literalList tpa, tpb; // tpa指向新的文字,tpb指向旧的文字
	newCnf->head = (literalList)malloc(sizeof(literalNode));
	newCnf->next = NULL;
	newCnf->head->next = NULL;
	for (lpb = cL, lpa = newCnf; lpb != NULL; lpb = lpb->next, lpa = lpa->next)
	{
		for (tpb = lpb->head, tpa = lpa->head; tpb != NULL; tpb = tpb->next, tpa = tpa->next)
		{
			tpa->literal = tpb->literal;
			tpa->next = (literalList)malloc(sizeof(literalNode));
			tpa->next->next = NULL;
			if (tpb->next == NULL) // 旧的子句中的文字已经复制完
			{
				free(tpa->next);
				tpa->next = NULL;
			}
		}
		lpa->next = (clauseList)malloc(sizeof(clauseNode));
		lpa->next->head = (literalList)malloc(sizeof(literalNode));
		lpa->next->next = NULL;
		lpa->next->head->next = NULL;
		if (lpb->next == NULL) // 旧的CNF中的子句已经复制完
		{
			free(lpa->next->head);
			free(lpa->next);
			lpa->next = NULL;
		}
	}
	return newCnf;
}

int ChooseLiteral_1(clauseList cL)
{
	return cL->head->literal;
}

int ChooseLiteral_2(clauseList cL)
{
	clauseList lp = cL;
	literalList dp;
	int *count, MaxWord, max; // count记录每个文字出现次数,MaxWord记录出现最多次数的文字
	count = (int *)malloc(sizeof(int) * (boolCount * 2 + 1));
	for (int i = 0; i <= boolCount * 2; i++)
		count[i] = 0; // 初始化
	// 计算子句中各文字出现次数
	for (lp = cL; lp != NULL; lp = lp->next)
	{
		for (dp = lp->head; dp != NULL; dp = dp->next)
		{
			if (dp->literal > 0) // 正文字
				count[dp->literal]++;
			else
				count[boolCount - dp->literal]++; // 负文字
		}
	}
	max = 0;
	// 找到出现次数最多的正文字
	for (int i = 1; i <= boolCount; i++)
	{
		if (max < count[i])
		{
			max = count[i];
			MaxWord = i;
		}
	}
	if (max == 0)
	{
		// 若没有出现正文字,找到出现次数最多的负文字
		for (int i = boolCount + 1; i <= boolCount * 2; i++)
		{
			if (max < count[i])
			{
				max = count[i];
				MaxWord = boolCount - i;
			}
		}
	}
	free(count);
	return MaxWord;
}

int ChooseLiteral_3(clauseList cL)
{
	clauseList p = cL;
	int *count = (int *)calloc(boolCount * 2 + 1, sizeof(int));
	int minSize = INT_MAX; // 初始化为大于可能的最大子句长度
	int literal = 0;
	clauseList temp = NULL;
	// 遍历子句，找到最小子句并统计其文字
	while (p != NULL)
	{
		literalList q = p->head;
		int clauseSize = 0;
		while (q != NULL)
		{
			clauseSize++;
			q = q->next;
		}
		if (clauseSize < minSize)
		{
			minSize = clauseSize; // 更新最小子句大小
			temp = p;
		}
		p = p->next;
	}
	// 遍历子句，统计最小子句中各文字出现次数
	literalList q = temp->head;
	while (q != NULL)
	{
		count[q->literal + boolCount]++;
		q = q->next;
	}
	// 找到最频繁的文字
	int maxCount = 0;
	for (int i = 0; i < boolCount * 2 + 1; i++)
	{
		if (count[i] > maxCount)
		{
			maxCount = count[i];
			literal = i - boolCount;
		}
	}
	free(count);
	return literal;
}

status DPLL(clauseList &cL, int value[], int flag)
{
	/*1.单子句规则*/
	int unitLiteral = FindUnitClause(cL);
	while (unitLiteral != 0)
	{
		value[abs(unitLiteral)] = (unitLiteral > 0) ? TRUE : FALSE;
		Simplify(cL, unitLiteral);
		// 终止条件
		clauseList p = cL;
		if (p == NULL)
			return OK; // 所有都被满足了
		while (p)
		{
			if (p->head == NULL) // 空子句，返回UNSAT
				return ERROR;
			p = p->next;
		}
		unitLiteral = FindUnitClause(cL);
	}
	/*2.选择一个未赋值的文字*/
	int literal;
	if (flag == 1)
		literal = ChooseLiteral_1(cL); // 未优化
	else if (flag == 2)
		literal = ChooseLiteral_2(cL); // 优化
	else
		literal = ChooseLiteral_3(cL);
	/*3.将该文字赋值为真，递归求解*/
	clauseList newCnf = CopyCnf(cL);
	clauseList p = (clauseList)malloc(sizeof(clauseNode));
	p->head = (literalList)malloc(sizeof(literalNode));
	p->head->literal = literal;
	p->head->next = NULL;
	p->next = newCnf;
	newCnf = p; // 插入到表头
	if (DPLL(newCnf, value, flag) == 1)
		return 1; // 在第一分支中搜索
	DestroyCnf(newCnf);
	/*4.将该文字赋值为假，递归求解*/
	newCnf = CopyCnf(cL);
	clauseList q = (clauseList)malloc(sizeof(clauseNode));
	q->head = (literalList)malloc(sizeof(literalNode));
	q->head->literal = -literal;
	q->head->next = NULL;
	q->next = newCnf;
	newCnf = q;
	status re = DPLL(newCnf, value, flag); // 回溯到执行分支策略的初态进入另一分支
	// DestroyCnf(cL);
	return re;
}

status SaveResult(int result, double time, double time_, int value[], char fileName[])
{
	FILE *fp;
	char name[100];
	for (int i = 0; fileName[i] != '\0'; i++)
	{
		// 修改拓展名.res
		if (fileName[i] == '.' && fileName[i + 4] == '\0')
		{
			name[i] = '.';
			name[i + 1] = 'r';
			name[i + 2] = 'e';
			name[i + 3] = 's';
			name[i + 4] = '\0';
			break;
		}
		name[i] = fileName[i];
	}
	if (fopen_s(&fp, name, "w"))
	{
		printf(" Fail!\n");
		return ERROR;
	}
	fprintf(fp, "s %d", result); // 求解结果
	if (result == 1)
	{
		// 保存解值
		for (int i = 1, cnt = 1; i <= boolCount; i++, cnt++)
		{
			if (i == 1)
				fprintf(fp, "\nv "); // 第一行
			if (value[i] == TRUE)
				fprintf(fp, "%-d ", i);
			else
				fprintf(fp, "%-d ", -i);
			if (cnt == 20) // 每20个一行
				cnt = 0;
		}
	}
	fprintf(fp, "\nt %lfms(not optimized)", time * 1000);	  // 运行时间/毫秒
	fprintf(fp, "\nt %lfms(optimized)", time_ * 1000);		  // 运行时间/毫秒
	double optimization_rate = ((time - time_) / time) * 100; // 优化率
	fprintf(fp, "\nOptimization Rate: %.2lf%%", optimization_rate);
	fclose(fp);
	return OK;
}

