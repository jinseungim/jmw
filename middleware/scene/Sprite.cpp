#include "jmw.h"
#include "Sprite.h"

NS_JMW_BEGIN



Sprite::Sprite(void)
{
}


Sprite::~Sprite(void)
{
}


Sprite::Sprite(const string resName)
{
	Sprite();

	m_Img = GetTexManager().GetImage(resName);
	if (m_Img)
	{
		SetSize(Size(m_Img->GettexW(), m_Img->GettexH()));
		
	}
}


Sprite* Sprite::Create(const string resName)
{
	return new Sprite(resName);
}
NS_JMW_END
