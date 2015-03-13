#include "jmw.h"
#include "FontManager.h"


NS_JMW_BEGIN

FontManager *FontManager::pInstance = nullptr;

FontManager& FontManager::GetInstance(void)
{
	if (pInstance == nullptr)
		pInstance = new FontManager();

	return *pInstance;
}

FontManager* FontManager::GetPtr()
{
	return pInstance;
}


void FontManager::ReleaseInstance(void)
{
	if (pInstance != nullptr)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}

FontManager::FontManager() : m_library(nullptr), m_error(0), m_fontCount(0)
{
}


FontManager::~FontManager()
{

	AllUnloadFace();
	FT_Done_FreeType(m_library);

}

void FontManager::Init(void)
{
	m_error = FT_Init_FreeType(&m_library);
}

FT_Face  FontManager::LoadFace(const char* fontName, s32 faceIdx, s32 mode)
{
	ASSERT(fontName != nullptr);
	ASSERT(strlen(fontName)>0);

	FT_Face face=nullptr;

	auto f = m_face.find(fontName);
	if (f != m_face.cend())
		face = f->second->face;
	else
	{
		if (m_error == 0)
		{
			FontInfo* fontInfo;
						
			fontInfo = new FontInfo;
			fontInfo->data = GetData(fontName, &fontInfo->size, mode);
			m_error = -1;

			if (fontInfo->data && fontInfo->size > 0) {
				m_error = FT_New_Memory_Face(m_library, (const FT_Byte*)fontInfo->data, fontInfo->size, 0, &fontInfo->face);
			}

			if (m_error != 0)
			{
				SAFE_DELETE_NULL(&fontInfo);
				LOG("no font : %s", fontName);
			}
			else {
				face = fontInfo->face;
				m_face.insert(std::make_pair(fontName, fontInfo));

				m_fontName.push_back(fontName);
				++m_fontCount;
			}
		}
	}


	return face;
}


FT_Face FontManager::GetFace(s32 idx)
{
	FT_Face face = nullptr;

	try
	{
		auto f = m_face.find(m_fontName.at(idx));
		if (f != m_face.cend())
			face = f->second->face;
	}
	catch (const std::exception& e){
		LOG("%s\n", "no font");
	}

	return face;
}

void FontManager::UnloadFace(FT_Face face)
{
	for (auto &f : m_face)
	{
		if (f.second->face == face)
		{
			FT_Done_Face(f.second->face);
			SAFE_DELETE_NULL(&f.second->data);
			SAFE_DELETE_NULL(&f.second);
			m_face.erase(f.first);
			break;
		}
	}
}

void FontManager::AllUnloadFace(void)
{
	for (auto &f : m_face)
	{
		FT_Done_Face(f.second->face);
		SAFE_DELETE_NULL(&f.second->data);
		SAFE_DELETE_NULL(&f.second);
	}
	m_face.clear();
}


u32 FontManager::GetGlyphIndex(ul32 charcode) const
{
	u32 index = 0;
	for (auto &f : m_face)
	{
		index = FT_Get_Char_Index(f.second->face, charcode);
		if (index != 0) break;
	}


	return index;
}

u32 FontManager::GetGlyphIndex(FT_Face face, ul32 charcode) const
{
	ASSERT(face != nullptr);
	u32 index = 0;

	index =  FT_Get_Char_Index(face, charcode);
	if (index == 0)
	{
		index = GetGlyphIndex(charcode);
	}

	return index;
}

NS_JMW_END