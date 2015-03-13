#pragma once

NS_JMW_BEGIN

class Image
{
public:
	Image();
	virtual ~Image();

private:
	GETSET(s32, texX)
	GETSET(s32, texY)
	GETSET(s32, texW)
	GETSET(s32, texH)
	GETSET(s32, texSizeW)
	GETSET(s32, texSizeH)
	GETSET(s32, Depth)
	
public:
	GLuint m_TextureID;
	static shared_ptr<Image> Create();

};

NS_JMW_END
