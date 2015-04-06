/********************************************************************
 * File: awtrace.C
 *
 * Author: AlexWoo <wj19840501@gmail.com>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "funcrec.hpp"
#include "funcstack.hpp"

int main(int argc, char** argv)
{
	if (5 != argc)
	{
		printf("Usage: awtrace <exefile> <recordfile> <dotfile> <analyzefile>\n");
		exit(-1);
	}

	FILE* rf = fopen(argv[2], "r"); //recordfile
	if (NULL == rf)
	{
		printf("open recordfile(%s) failed\n", argv[1]);
		exit(-1);
	}

	FuncManager::getInstance()->init(argv[1], argv[3], argv[4]);

	char type;
	long funcaddr;
	long tid;
	long sec;
	int  usec;

	while (!feof(rf))
	{
		fscanf(rf, "%c0x%lx %lu %lu.%06d\n", &type, &funcaddr, &tid, &sec, &usec);
		if ('E' == type)
		{
			StackManager::getInstance()->pushstack(tid, funcaddr, sec, usec);
		}
		else if ('X' == type)
		{
			long spendtime =
				StackManager::getInstance()->popstack(tid, funcaddr, sec, usec);
			long calleraddr = StackManager::getInstance()->stacktop(tid);
			FuncManager::getInstance()->addfunc(funcaddr, spendtime);
			if (0 != calleraddr)
			{
				FuncManager::getInstance()->callfunc(calleraddr, funcaddr);
			}
		}
	}

	FuncManager::getInstance()->commit();

	fclose(rf);

	return 0;
}
