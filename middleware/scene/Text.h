#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H

NS_JMW_BEGIN


typedef struct _CharBuf
{
	s32     kerningX;
	s32     bitmap_left, bitmap_top;
	s32     advanceX, advanceY;
	s32		width, height;
	s32     line_spacing;
	u32*	bitmap;

	FT_BBox box;
	wchar_t c;
}CharBuf;


typedef enum
{
	E_None,
	E_Size,
	E_Color,
	E_BgColor,
}TagStyle;

class Text : public Drawable
{
	FontManager* m_font;
	u8      m_R, m_G, m_B;
	u32     m_Size;
	u32		m_BgColor;
	s32     m_FontIdx;


	s32		LoadCharInfo(wchar_t*& txt, size_t len);
	size_t	IsStartTag(const wchar_t* ch) const;
	size_t  FindTag(const wchar_t* ch, wchar_t tag) const;
	size_t  GetTagInfo(const wchar_t* ch);
	static wchar_t* GetLine(sz*& txt, size_t size, u32 line, size_t* len);
public:
	Text();
	Text(wchar_t*, size_t);
	~Text();


	static Text* Create(u8*, size_t size, u32 line = 0);
	static Text* Create(sz*, u32 line = 0);
};

NS_JMW_END