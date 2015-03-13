#include "jmw.h"
#include "Message.h"

NS_JMW_BEGIN

Msg::Msg(void) : what(0), time(0), isDel(0)
{

}
Msg::~Msg(void)
{
}



Message::Message(void)
{
}


Message::~Message(void)
{
}



void Message::PutMsg(Msg* m)
{
	ASSERT(m != nullptr);
	m_Msg.Enqueue(m);
}


Msg* Message::PopMag(void)
{
	Msg* msg = nullptr;
	if (m_Msg.Size() > 0)
		msg = m_Msg.Dequeue();

	return msg;
}


Msg* Message::PeekMsg(void)
{
	Msg* msg = nullptr;
	if (m_Msg.Size() > 0)
		msg = m_Msg.Peek();

	return msg;
}

Msg* MakeMsg(s32 cmd, Msg* msg)
{

	msg->what = cmd;
	msg->time = RunTime::GetInstance().GetCurTime();
	msg->isDel = false;

	return msg;

}
NS_JMW_END