/*----------------------------solver----------------------------*/

#include "SAT.h"

/*
 @ 函数名称: IsUnitClause
 @ 接受参数: literalList
 @ 函数功能: 判断是否为单子句
 @ 返回值: status
 */
status IsUnitClause(literalList l)
{
	if (l != NULL && l->next == NULL)
		return TRUE;
	else
		return FALSE;
}

/*
 @ 函数名称: FindUnitClause
 @ 接受参数: clauseList
 @ 函数功能: 找到单子句并返回该文字
 @ 返回值: int
 */
int FindUnitClause(clauseList cL)
{
	clauseList p = cL;
	while (p)
	{
		if (IsUnitClause(p->head))
			return p->head->literal;
		p = p->next;
	}
	return 0;
}

/*
 @ 函数名称: DestroyClause
 @ 接受参数: clauseList &
 @ 函数功能: 销毁子句
 @ 返回值: status
 */
status DestroyClause(clauseList &cL)
{
	literalList p = cL->head;
	while (p)
	{
		literalList temp = p;
		p = p->next;
		free(temp);
	}
	free(cL);
	cL = NULL;
	return OK;
}

/*
 @ 函数名称: Simplify
 @ 接受参数: clauseList &, int
 @ 函数功能: 根据选择的文字化简
 @ 返回值: void
 */
void Simplify(clauseList &cL, int literal)
{
	clauseList pre = NULL, p = cL;
	while (p != NULL)
	{
		bool clauseDeleted = false;
		literalList lpre = NULL, q = p->head;
		while (q != NULL)
		{
			if (q->literal == literal) // 删除该子句
			{
				if (pre == NULL)
					cL = p->next;
				else
					pre->next = p->next;
				DestroyClause(p);
				p = (pre == NULL) ? cL : pre->next;
				clauseDeleted = true;
				break;
			}
			else if (q->literal == -literal) // 删除该文字
			{
				if (lpre == NULL)
					p->head = q->next;
				else
					lpre->next = q->next;
				free(q);
				q = (lpre == NULL) ? p->head : lpre->next;
			}
			else
			{
				lpre = q;
				q = q->next;
			}
		}
		if (!clauseDeleted)
		{
			pre = p;
			p = p->next;
		}
	}
}

/*
 @ 函数名称: CopyCnf
 @ 接受参数: clauseList
 @ 函数功能: 复制cnf
 @ 返回值: clauseList
 */
clauseList CopyCnf(clauseList cL)
{
	clauseList newCnf = (clauseList)malloc(sizeof(clauseNode));
	clauseList lpa, lpb;
	literalList tpa, tpb;
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
			if (tpb->next == NULL)
			{
				free(tpa->next);
				tpa->next = NULL;
			}
		}
		lpa->next = (clauseList)malloc(sizeof(clauseNode));
		;
		lpa->next->head = (literalList)malloc(sizeof(literalNode));
		lpa->next->next = NULL;
		lpa->next->head->next = NULL;
		if (lpb->next == NULL)
		{
			free(lpa->next->head);
			free(lpa->next);
			lpa->next = NULL;
		}
	}
	return newCnf;
}

/*
 @ 函数名称: ChooseLiteral(未优化)
 @ 接受参数: clauseList
 @ 函数功能: 选择文字(第一个文字)
 @ 返回值: int
 */
int ChooseLiteral(clauseList cL)
{
	return cL->head->literal;
}


/*
 @ 函数名称: ChooseLiteral_(优化)
 @ 接受参数: clauseList
 @ 函数功能: (没有单子句时的策略)选择文字(出现次数最多的文字)
 @ 返回值: int
 */
int ChooseLiteral_(clauseList cL)
{
	clauseList lp = cL;
	literalList dp;
	int *count, i, MaxWord, max; // count记录每个文字出现次数,MaxWord记录出现最多次数的文字
	count = (int *)malloc(sizeof(int) * (boolCount * 2 + 1));
	for (i = 0; i <= boolCount * 2; i++)
		count[i] = 0; // 初始化
	// 计算子句中各文字出现次数
	for (lp = cL; lp != NULL; lp = lp->next)
	{
		for (dp = lp->head; dp != NULL; dp = dp->next)
		{
			if (dp->literal > 0)
				count[dp->literal]++;
			else
				count[boolCount - dp->literal]++;
		}
	}
	max = 0;
	// 找到出现次数最多的正文字
	for (i = 1; i <= boolCount; i++)
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
		for (i = boolCount + 1; i <= boolCount * 2; i++)
		{
			if (max < count[i])
			{
				max = count[i];
				MaxWord = -i;
			}
		}
	}
	free(count);
	return MaxWord;
}

/*
 @ 函数名称: DPLL
 @ 接受参数: clauseList,int[]
 @ 函数功能: DPLL算法求解SAT问题
 @ 返回值: status
 */
status DPLL(clauseList &cL,int value[],int flag)
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
	if (flag==1)
		literal = ChooseLiteral(cL);
	else
		literal = ChooseLiteral_(cL);
	/*3.将该文字赋值为真，递归求解*/
	clauseList newCnf = CopyCnf(cL);
	clauseList p = (clauseList)malloc(sizeof(clauseNode));
	p->head = (literalList)malloc(sizeof(literalNode));
	p->head->literal = literal;
	p->head->next = NULL;
	p->next = newCnf;
	newCnf = p; // 插入到表头
	if (DPLL(newCnf,value,flag) == 1)
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
	status re = DPLL(newCnf,value,flag); // 回溯到执行分支策略的初态进入另一分支
	// DestroyCnf(cL);
	return re;
}

/*
 @ 函数名称: SaveResult
 @ 接受参数: int,double,int[],char[]
 @ 函数功能: 保存求解结果
 @ 返回值: status
 */
status SaveResult(int result, double time, double time_, int value[],char fileName[])
{
	FILE* fp;
	char name[100];
	for (int i = 0; fileName[i] != '\0'; i++)
	{
		//修改拓展名
		if (fileName[i] == '.' && fileName[i + 4] == '\0')
		{
			name[i]='.';
			name[i + 1] = 'r';
			name[i + 2] = 'e';
			name[i + 3] = 's';
			name[i + 4] = '\0';
			break;
		}
		name[i] = fileName[i];
	}
	if(fopen_s(&fp, name, "w"))
	{
		printf(" Fail!\n");
		return ERROR;
	}
	fprintf(fp, "s %d", result);  //求解结果
	if (result == 1)
	{
		//保存解值
		for (int i = 1,cnt=1; i <= boolCount; i++,cnt++)
		{
			if(cnt==1) fprintf(fp,"\nv ");
			if (value[i] == TRUE) fprintf(fp, "%d ", i);
				else fprintf(fp, "%d ", -i);
			if(cnt==20) // 每20个一行
				cnt=0;
		}
	}
	fprintf(fp, "\nt %lfms(not optimized)", time * 1000);  //运行时间/毫秒
	fprintf(fp, "\nt %lfms(optimized)", time_ * 1000);  //运行时间/毫秒
	double optimization_rate = ((time - time_) / time) * 100;
    fprintf(fp, "\nOptimization Rate: %.2lf%%", optimization_rate);  // 优化率百分比
	fclose(fp);
	return OK;
}


// status DPLL(clauseList &cL)
// {
// 	clauseList tp = cL, lp = cL, sp;
// 	literalList dp;
// 	int* count, i, MaxWord, max, re; //count记录每个文字出现次数,MaxWord记录出现最多次数的文字
// 	count = (int*)malloc(sizeof(int) * (boolCount * 2 + 1));
// FIND:	while (tp != NULL && isUnitClause(tp->head) == 0) tp = tp->next;  //找到表中的单子句
// 	if (tp != NULL)
// 	{
// 		//单子句规则简化
// 		if (tp->head->literal > 0) value[tp->head->literal] = 1;
// 		else value[-tp->head->literal] = 0;
// 		re = tp->head->literal;
// 		for (lp = cL; lp != NULL; lp = sp)
// 		{
// 			sp = lp->next;
// 			//查找含有核心文字的句子
// 			for (dp = lp->head; dp != NULL; dp = dp->next)
// 			{
// 				if (dp->literal == re)
// 				{
// 					removeClause(lp, cL);  //删除子句，简化式子
// 					break;
// 				}
// 				if (dp->literal == -re)
// 				{
// 					removeNode(dp, lp->head);  //删除文字，简化子句
// 					break;
// 				}
// 			}
// 		}
// 		//极简化规则简化后
// 		if (cL == NULL)
// 		{
// 			free(count);
// 			return 1;
// 		}
// 		else if (emptyClause(cL))
// 		{
// 			free(count);
// 			DestroyCnf(cL);
// 			return 0;
// 		}
// 		tp = cL;
// 		goto FIND;  //继续简化
// 	}
// 	for (i = 0; i <= boolCount * 2; i++) count[i] = 0;  //初始化
// 	//计算子句中各文字出现次数
// 	for (lp = cL; lp != NULL; lp = lp->next)
// 	{
// 		for (dp = lp->head; dp != NULL; dp = dp->next)
// 		{
// 			if (dp->literal > 0) count[dp->literal]++;
// 			else count[boolCount - dp->literal]++;
// 		}
// 	}
// 	max = 0;
// 	//找到出现次数最多的正文字
// 	for (i = 2; i <= boolCount; i++)
// 	{
// 		if (max < count[i])
// 		{
// 			max = count[i];
// 			MaxWord = i;
// 		}
// 	}
// 	if (max == 0)
// 	{
// 		//若没有出现正文字,找到出现次数最多的负文字
// 		for (i = boolCount + 1; i <= boolCount * 2; i++)
// 		{
// 			if (max < count[i])
// 			{
// 				max = count[i];
// 				MaxWord = -i;
// 			}
// 		}
// 	}
// 	free(count);
// 	lp = (clauseList)malloc(sizeof(clauseNode));
// 	lp->head = (literalList)malloc(sizeof(literalNode));
// 	lp->head->literal = MaxWord;
// 	lp->head->next = NULL;
// 	lp->next = NULL;
// 	CopyClause(tp, cL);
// 	addClause(lp, tp);
// 	if (DPLL(tp) == 1) return 1;  //在第一分支中搜索
// 	DestroyCnf(tp);
// 	lp = (clauseList)malloc(sizeof(clauseNode));
// 	lp->head = (literalList)malloc(sizeof(literalNode));
// 	lp->head->literal = -MaxWord;
// 	lp->head->next = NULL;
// 	lp->next = NULL;
// 	addClause(lp, cL);
// 	re = DPLL(cL); //回溯到执行分支策略的初态进入另一分支
// 	DestroyCnf(cL);
// 	return re;
// }

// int removeClause(clauseList& p, clauseList& cL)
// {
// 	clauseList lp = cL;
// 	if (lp == p) cL = cL->next;  //删除为头
// 	else
// 	{
// 		while (lp != NULL && lp->next != p) lp = lp->next;
// 		lp->next = lp->next->next;
// 	}
// 	free(p);
// 	p = NULL;
// 	return 1;
// }

// int removeNode(literalList& q, literalList& head)
// {
// 	literalList lp = head;
// 	if (lp == q) head = head->next;  //删除为头
// 	else
// 	{
// 		while (lp != NULL && lp->next != q) lp = lp->next;
// 		lp->next = lp->next->next;
// 	}
// 	free(q);
// 	q = NULL;
// 	return 1;
// }

// int addClause(clauseList cnf, clauseList& root)
// {
// 	//直接插入在表头
// 	if (cnf != NULL)
// 	{
// 		cnf->next = root;
// 		root = cnf;
// 		return 1;
// 	}
// 	return 0;
// }

// int isUnitClause(literalList cL)
// {
// 	if (cL != NULL && cL->next == NULL)
// 		return 1;
// 	else
// 		return 0;
// }

// int emptyClause(clauseList cL)
// {
// 	clauseList lp = cL;
// 	while (lp != NULL)
// 	{
// 		if (lp->head == NULL) return 1;
// 		lp = lp->next;
// 	}
// 	return 0;
// }

// void CopyClause(clauseList &a, clauseList b)
// {
// 	clauseList lpa,lpb;
// 	literalList tpa,tpb;
// 	a = (clauseList)malloc(sizeof(clauseNode));
// 	a->head = (literalList)malloc(sizeof(literalNode));
// 	a->next = NULL;
// 	a->head->next = NULL;
// 	for (lpb = b, lpa = a; lpb != NULL; lpb = lpb->next, lpa = lpa->next)
// 	{
// 		for (tpb = lpb->head, tpa = lpa->head; tpb != NULL; tpb = tpb->next, tpa = tpa->next)
// 		{
// 			tpa->literal = tpb->literal;
// 			tpa->next = (literalList)malloc(sizeof(literalNode));
// 			tpa->next->next = NULL;
// 			if (tpb->next == NULL)
// 			{
// 				free(tpa->next);
// 				tpa->next = NULL;
// 			}
// 		}
// 		lpa->next = (clauseList)malloc(sizeof(clauseNode));
// 		lpa->next->head = (literalList)malloc(sizeof(literalNode));
// 		lpa->next->next = NULL;
// 		lpa->next->head->next = NULL;
// 		if (lpb->next == NULL)
// 		{
// 			free(lpa->next->head);
// 			free(lpa->next);
// 			lpa->next = NULL;
// 		}
// 	}
// }
