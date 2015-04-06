/********************************************************************
 * File: funcrec.C
 *
 * Author: AlexWoo <wj19840501@gmail.com>
 *
 */

#include "funcrec.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcstack.hpp"

FuncManager* FuncManager::m_instance = NULL;

FuncManager* FuncManager::getInstance()
{
	if (NULL == m_instance)
	{
		m_instance = new FuncManager();
	}

	return m_instance;
}

FuncManager::FuncManager():
m_df(NULL), m_af(NULL)
{
}

FuncManager::~FuncManager()
{
}

void FuncManager::init(const char* exefile, const char* dotfile, const char* analyzefile)
{
	m_exefile = exefile;

	m_df = fopen(dotfile, "w"); //dotfile
	if (NULL == m_df)
	{
		printf("open dotfile(%s) failed\n", dotfile);
		exit(-1);
	}

	m_af = fopen(analyzefile, "w"); //analyzefile
	if (NULL == m_af)
	{
		printf("open analyzefile(%s) failed\n", analyzefile);
		exit(-1);
	}

	return;
}

void FuncManager::addfunc(long address, long time)
{
	std::list<Func>::iterator pfuncs = m_funcs.begin();

	for (; pfuncs != m_funcs.end(); ++pfuncs)
	{
		if (address == pfuncs->funcaddr)
		{
			pfuncs->totaltime += time;
			++pfuncs->nrcalls;
			return;
		}
	}

	//a new func address
	Func newfunc;
	newfunc.funcaddr = address;
	newfunc.totaltime += time;
	++newfunc.nrcalls;
	m_funcs.push_front(newfunc);

	m_funcname[address] = translateFunc(address);
}

void FuncManager::callfunc(long calleraddr, long calleeaddr)
{
	std::list<Func>::iterator pfuncs = m_funcs.begin();

	for (; pfuncs != m_funcs.end(); ++pfuncs)
	{
		if (calleeaddr == pfuncs->funcaddr)
		{
			std::map<long, int>::iterator pcallsfunc
				= pfuncs->callsfunc.find(calleraddr);
			if (pfuncs->callsfunc.end() == pcallsfunc) //first time to add call relation
			{
				pfuncs->callsfunc[calleraddr] = 1;
			}
			else
			{
				++pfuncs->callsfunc[calleraddr];
			}
			return;
		}
	}
}

void FuncManager::commit()
{
	drawCallGraph();
	drawPerfGraph();

	if (NULL != m_df)
	{
		fclose(m_df);
	}

	if (NULL != m_af)
	{
		fclose(m_af);
	}
}

std::string FuncManager::translateFunc(long funcaddr)
{
	FILE* fp;
	char cmdline[256];
	char cmdres[512];
	unsigned len, i;

	snprintf(cmdline, 256, "nm %s |grep %lx |c++filt |awk '{$1=$2=\"\"; print}' | awk 'gsub(/^ *| *$/,\"\")'",
		m_exefile.c_str(), funcaddr);
	fp = popen(cmdline, "r");
	if (NULL == fp)
	{
		return std::string("");
	}
	else
	{
		len = fread(cmdline, 511, 1, fp);
		i = 0;
		while (i < strlen(cmdline))
		{
			if (cmdline[i] == 0x0d || cmdline[i] == 0x0a)
			{
				cmdres[i] = 0;
				break;
			}
			else
			{
				cmdres[i] = cmdline[i];
			}
			++i;
		}
	}
	pclose(fp);

	return std::string(cmdres);
}

void FuncManager::drawCallGraph()
{
	std::list<Func>::iterator pfuncs = m_funcs.begin();

	fprintf(m_df, "digraph CallGraph{\n\n");

	for (; pfuncs != m_funcs.end(); ++pfuncs) //draw node
	{
		fprintf(m_df, "   \"%s\"  [shape=ellipse]\n",
			m_funcname[pfuncs->funcaddr].c_str());
	}
	fprintf(m_df, "\n");

	for (pfuncs = m_funcs.begin(); pfuncs != m_funcs.end(); ++pfuncs) //draw edge
	{
		std::map<long, int>::iterator pcallsfunc = pfuncs->callsfunc.begin();

		for (; pcallsfunc != pfuncs->callsfunc.end(); ++pcallsfunc)
		{
			fprintf(m_df, "   \"%s\" -> \"%s\" [label=\"%d calls\" fontsize=\"10\"]\n",
				m_funcname[pcallsfunc->first].c_str(),
				m_funcname[pfuncs->funcaddr].c_str(),
				pcallsfunc->second);
		}
	}
	fprintf(m_df, "\n");

	fprintf(m_df, "}\n\n");
}

void FuncManager::drawPerfGraph()
{
	std::list<Func>::iterator pfuncs = m_funcs.begin();
	long base = pfuncs->totaltime;

	for (; pfuncs != m_funcs.end(); ++pfuncs)
	{
		fprintf(m_af, "%s\n\tcall %d times, spent %ldus for total\n",
			m_funcname[pfuncs->funcaddr].c_str(),
			pfuncs->nrcalls, pfuncs->totaltime);
		drawPercentage(pfuncs->totaltime, base);
		fprintf(m_af, "\n\n");
	}
}

void FuncManager::drawPercentage(long div, long base)
{
	long res = div * 100 / base;
	for (int i = 0; i < res; ++i)
	{
		fprintf(m_af, "#");
	}
}
