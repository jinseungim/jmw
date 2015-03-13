#pragma once

NS_JMW_BEGIN

class SevenZDec : public Thread
{
public:
	SevenZDec(const string, const string, const string command = "x");
	~SevenZDec();


	void Enter(void*);
	void*  Run(void*);
	void Exit(void*);


	SevenZDec(const SevenZDec&) = delete;
	SevenZDec operator=(const SevenZDec sz) = delete;

protected:
	const string m_Src;
	const string m_Dst;
	const string m_Cmd;

};

NS_JMW_END
