/********************************************************************
 * File: funcstack.hpp
 *
 * Author: AlexWoo <wj19840501@gmail.com>
 *
 */

#ifndef __FUNCSTACK_HPP__
#define __FUNCSTACK_HPP__

#include <sys/time.h>
#include <map>
#include <vector>

class StackManager
{
public:
	~StackManager();

	static StackManager* getInstance();

	void pushstack(long tid, long address, long sec, int usec);
	long popstack(long tid, long address, long sec, int usec);
	long stacktop(long tid);

protected:
	StackManager();

private:
	struct StackItem
	{
		StackItem(long _address, long _sec, int _usec):
		address(_address), sec(_sec), usec(_usec)
		{
		}

		long address;
		long sec;
		int  usec;
	};

private:
	static StackManager* m_instance;

	typedef std::vector<StackItem> Stack;
	std::map<long, Stack> stacks;
};

#endif
