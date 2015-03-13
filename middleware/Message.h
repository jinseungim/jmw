#pragma once

NS_JMW_BEGIN


class Msg
{
public:

    Msg(void);
	virtual ~Msg(void);

	s32  what;
	s64  time;
	bl   isDel;

	virtual void excute(void){};
};



class TouchPoint : public Msg
{
public:
	s32		kindOf;
	s32     touchId;
	Point	pos;
};




class Message
{
	Queue<Msg*>							m_Msg;

public:
	Message(void);
	virtual ~Message(void);

	void PutMsg(Msg*);
	Msg* PopMag();
	Msg* PeekMsg();

};




Msg* MakeMsg(s32, Msg*);




NS_JMW_END
