#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H

NS_JMW_BEGIN


typedef struct _FontInfo
{
	FT_Face face;
	u8*     data;
	size_t  size;
}FontInfo;


class FontManager
{
private:
	FontManager();
	~FontManager();


	FT_Library			 m_library;
	s32					 m_error;
	map<string, FontInfo*> m_face;
	vector<string>         m_fontName;
	s32					   m_fontCount;
	
	static FontManager* pInstance;

public:

	void		Init(void);
	FT_Face		LoadFace(const char*, s32, s32 mode=0);
	FT_Face		GetFace(s32 idx=0);
	void		UnloadFace(FT_Face);
	void		AllUnloadFace(void);
	u32			GetGlyphIndex(ul32) const;
	u32			GetGlyphIndex(FT_Face, ul32) const;
	FT_Library  GetLibrary(void) { return m_library;  }

	static FontManager& GetInstance(void);
	static FontManager* GetPtr();
	static void ReleaseInstance(void);
};

NS_JMW_END