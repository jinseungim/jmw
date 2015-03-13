#include "jmw.h"
#include "Image.h"

NS_JMW_BEGIN

Image::Image() : INIT_ZERO(texX), INIT_ZERO(texY), INIT_ZERO(texW), INIT_ZERO(texH), INIT_ZERO(texSizeW), INIT_ZERO(texSizeH), INIT_VAR(Depth, 32), m_TextureID(0)
{
}


Image::~Image()
{
	if (m_TextureID>0)
		glDeleteTextures(1, &m_TextureID);
}

shared_ptr<Image> Image::Create()
{
	return shared_ptr<Image>(new Image());
}
NS_JMW_END
