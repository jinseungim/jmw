#pragma once

NS_JMW_BEGIN

class Object
{
public:
	Object(void);
	virtual ~Object(void);

	static u64 m_uGID;

private:
	u64 m_uID;
	u32 m_uReference;

		                    
public:
	u64  GetID(void) const;
	void Release(void);
	void Retain(void);
	void PrintClassName(void);

	static void PrintCurGID(void);
	
		
};


typedef bl (Object::*CMD_LISTENER)(Msg*);

typedef bl(Object::*TOUCH_LISTENER)(Point);



#define SET_CMDLISTENER(_SELECTOR) (CMD_LISTENER)(&_SELECTOR)
#define SET_TOUCHLISTENER(_SELECTOR) (TOUCH_LISTENER)(&_SELECTOR)


NS_JMW_END

