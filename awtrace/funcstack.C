/********************************************************************
 * File: funcstack.C
 *
 * Author: AlexWoo <wj19840501@gmail.com>
 *
 */

#include "funcstack.hpp"
#include <stdio.h>

StackManager* StackManager::m_instance = NULL;

StackManager* StackManager::getInstance()
{
	if (NULL == m_instance)
	{
		m_instance = new StackManager();
	}

	return m_instance;
}

StackManager::StackManager()
{
}

StackManager::~StackManager()
{
}

void StackManager::pushstack(long tid, long address, long sec, int usec)
{
	stacks[tid].push_back(StackItem(address, sec, usec));
}

long StackManager::popstack(long tid, long address, long sec, int usec)
{
	long spendtime = (sec - stacks[tid].back().sec) * 1000000
		+ (usec - stacks[tid].back().usec);
	stacks[tid].pop_back();
	return spendtime;
}

long StackManager::stacktop(long tid)
{
	if (0 != stacks[tid].size())
	{
		return stacks[tid].back().address;
	}
	else
	{
		return 0;
	}
}
