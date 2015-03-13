#include "jmw.h"
#include "Object.h"

NS_JMW_BEGIN

// Object를 상속받은 클래스가 생성될때 마다 고유 아이디가 발급된다.
u64 Object::m_uGID = 0;

Object::Object(void) : m_uID(0), m_uReference(1)
{
	m_uID = m_uGID;
	m_uGID++;
}


Object::~Object(void)
{
}


void Object::PrintCurGID(void)
{
	LOG("Current GID: %lld", m_uGID - 1);
}

u64 Object::GetID(void) const
{
	return m_uID;
}

void Object::Release(void)
{
	ASSERT(m_uReference > 0);
	--m_uReference;

	if (m_uReference == 0)
	{
		delete this;
	}

}

void Object::Retain(void)
{
	ASSERT(m_uReference > 0);
	++m_uReference;
}


void Object::PrintClassName(void)
{
	LOG("name is : %s", typeid(*this).name());
}


NS_JMW_END
