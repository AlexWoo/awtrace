/********************************************************************
 * File: funcrec.hpp
 *
 * Author: AlexWoo <wj19840501@gmail.com>
 *
 */

#ifndef __FUNCREC_HPP__
#define __FUNCREC_HPP__

#include <string>
#include <map>
#include <list>

class FuncManager
{
public:
	~FuncManager();

	static FuncManager* getInstance();

	void init(const char* exefile, const char* dotfile, const char* analyzefile);
	void addfunc(long address, long time);
	void callfunc(long calleraddr, long calleeaddr);
	void commit();

protected:
	FuncManager();

private:
	std::string translateFunc(long funcaddr);
	void drawCallGraph();
	void drawPerfGraph();
	void drawPercentage(long div, long base);

private:
	struct Func
	{
		Func(): funcaddr(0), totaltime(0), nrcalls(0)
		{
		}

		long        funcaddr;
		long        totaltime;
		int         nrcalls;
		std::map<long, int> callsfunc;
	};

private:
	static FuncManager* m_instance;

	FILE* m_df;
	FILE* m_af;

	std::string m_exefile;
	std::list<Func> m_funcs;
	std::map<long, std::string> m_funcname;
};

#endif
