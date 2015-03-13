#pragma once

NS_JMW_BEGIN

class Sprite : public Drawable
{
public:
	Sprite(void);
	Sprite(const string);
	~Sprite(void);

	static Sprite* Create(const string);
};

NS_JMW_END
