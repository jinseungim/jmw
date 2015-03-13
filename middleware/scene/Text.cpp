#include "jmw.h"
#include "Text.h"
#include <fstream>

NS_JMW_BEGIN



Text::Text() 
{	
}


Text::Text(wchar_t* txt, size_t len) : m_R(255), m_G(255), m_B(255), m_Size(22), m_BgColor(ABGR(0, 0, 0, 0)), m_FontIdx(0)
{

#if defined(__ANDROID__)
	FontManager::GetInstance().LoadFace("/system/fonts/DroidSansFallback.ttf", 0, 1);
#elif defined(WIN32) || defined(__MINGW__)
	FontManager::GetInstance().LoadFace("DroidSansFallback.ttf", 0);
	FontManager::GetInstance().LoadFace("cour.ttf", 0);
#endif

	m_font = FontManager::GetPtr();
	s32 strLen = LoadCharInfo(txt, len);
}

Text::~Text()
{
}


s32 Text::LoadCharInfo(wchar_t*& wbuf, size_t len)
{

	size_t strLen = 0;
	strLen = len;

	vector<shared_ptr<CharBuf>> vch;
	
	FT_Error		error;
	unsigned		left = 0, right = 0;
	FT_Vector		kerning;
	s32				outline_type = 0;
	FT_GlyphSlot	slot = nullptr;
	FT_Bitmap		ft_bitmap;
	FT_Glyph		ft_glyph = nullptr;

	FT_BitmapGlyph	ft_bitmap_glyph = nullptr;
	s32 baseLine = 0;
	s32 line_spacing = 0;
	m_FontIdx = 0;

	for (size_t idx = 0; idx < strLen; ++idx)
	{
		FT_Int32 flags = 0;
		
		
		s32 tagOffset;
		tagOffset = GetTagInfo(&wbuf[idx]);
		if (tagOffset > 0)
		{
			idx += tagOffset;
			continue;
		}

		FT_Face face = m_font->GetFace(m_FontIdx);
		if (face == nullptr) continue;

		shared_ptr<CharBuf> ch = shared_ptr<CharBuf>(new CharBuf);

		ch->c = wbuf[idx];
		right = FontManager::GetInstance().GetGlyphIndex(face, wbuf[idx]);

		FT_Set_Char_Size(face, To26Dot6(m_Size), 0, GetXdpi(), GetYdpi());

		ch->kerningX = 0;
		if (left != 0) {
			FT_Get_Kerning(face, left, right, FT_KERNING_DEFAULT, &kerning);
			ch->kerningX = From26Dot6(kerning.x);
		}
		if (outline_type > 0)
		{
			flags |= FT_LOAD_NO_BITMAP;
		}
		else
		{
			flags |= FT_LOAD_RENDER;
		}

		flags |= FT_LOAD_FORCE_AUTOHINT;

		error = FT_Load_Glyph(face, right, flags);

		if (outline_type == 0)
		{
			slot = face->glyph;
			ft_bitmap = slot->bitmap;
		}
		else
		{
			FT_Stroker stroker;
			error = FT_Stroker_New(FontManager::GetInstance().GetLibrary(), &stroker);


			FT_Stroker_Set(stroker,
				(int)To26Dot6(1),
				FT_STROKER_LINECAP_ROUND,
				FT_STROKER_LINEJOIN_ROUND,
				0);

			error = FT_Get_Glyph(face->glyph, &ft_glyph);

			if (outline_type == 1)
			{
				error = FT_Glyph_Stroke(&ft_glyph, stroker, 1);
			}
			else if (outline_type == 2)
			{
				error = FT_Glyph_StrokeBorder(&ft_glyph, stroker, 0, 1);
			}
			else if (outline_type == 3)
			{
				error = FT_Glyph_StrokeBorder(&ft_glyph, stroker, 1, 1);
			}
			error = FT_Glyph_To_Bitmap(&ft_glyph, FT_RENDER_MODE_NORMAL, 0, 1);

			ft_bitmap_glyph = (FT_BitmapGlyph)ft_glyph;
			ft_bitmap = ft_bitmap_glyph->bitmap;

			FT_Stroker_Done(stroker);
		}

		
		if (ft_glyph == nullptr)
		{
			FT_Get_Glyph(face->glyph, &ft_glyph);
			FT_Glyph_Get_CBox(ft_glyph, FT_GLYPH_BBOX_PIXELS, &ch->box);
		}


		int width = ft_bitmap.width;
		int height = ft_bitmap.rows;
		ch->width = ft_bitmap.width;
		ch->height = ft_bitmap.rows;

		ch->bitmap = new u32[width * height];
		memset(ch->bitmap, 0, sizeof(u32)*width*height);

		u8  nMask = 0x80;
		int nIndex = 0;
		int color;

		u8  alpha = 0;

		for (int j = 0; j <ft_bitmap.rows; j++) {
			for (int i = 0; i < ft_bitmap.width; i++){

				s32 idx0 = (j*ft_bitmap.width) + i;

				if (ft_bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
				{
					color = 255 - ((ft_bitmap.buffer[nIndex] & nMask) ? 0xFF : 0x00);
					if (!color) alpha = 255;

					nMask = nMask >> 1;
					if (!nMask)
					{
						nMask = 0x80;
						nIndex++;
					}
				}
				else
				{
					color = 255 - ft_bitmap.buffer[idx0];
					alpha = ft_bitmap.buffer[idx0];
				}


				if (color != 255) {
					ch->bitmap[idx0] = ABGR(m_R, m_G, m_B, alpha);
				}
			}
		}

		line_spacing = face->size->metrics.x_ppem;
		baseLine = MAX(face->size->metrics.x_ppem, baseLine);		// x_ppem: dip에 따른 실제 픽셀크기

		ch->line_spacing	= From26Dot6(slot->metrics.height);
		ch->advanceX		= From26Dot6(face->glyph->advance.x);	// 좌우여백을포함한폭
		ch->advanceY		= From26Dot6(face->glyph->advance.y);
		ch->bitmap_left		= slot->bitmap_left;					// bitmap_left 펜의 x로 부터의 왼쪽여백
		ch->bitmap_top		= slot->bitmap_top;						// bitmap_top 베이스라인에서 상단까지의 픽셀

		vch.push_back(ch);

		left = right;

		if (ft_glyph != nullptr)
		{
			FT_Done_Glyph(ft_glyph);
			ft_glyph = nullptr;
		}
	}


	strLen = vch.size();
	if (strLen > 0)
	{
		s32 width0 = 0, width1 = 0;
		s32 height0 = 0;
		s32 w0 = 0, h0 = 0, h1 = 0;
		s32 LFcnt = 1;
		
		

		h1 = baseLine;
		
		for (size_t idx = 0; idx < strLen; ++idx)
		{
			if (vch[idx]->c == L'\\' && vch[idx+1]->c == L'n')
			{
				LFcnt++;
				++idx;
				// 개행이 발생했을때 가장긴줄의 폭이 얼마인지 계산한다.
				width1 = MAX(width1, width0);
				width0 = 0;
				continue;
			}
			else
				width0 += vch[idx]->advanceX;

			height0 = MAX(vch[idx]->height, height0);

			h0 = MIN(h0, vch[idx]->box.yMin);         // yMin 베이스라인에서 밑에까지 내려가는 높이

			
		}

		// 좌우 정렬을 위해서 한라인의 폭을 얻어온다.
		// LFcnt값이 필요한 이유로 루프를 따로 돌려야한다.
		s32  lineInfoIndex = 0;
		s32 *lineInfo = new s32[LFcnt];
		memset(lineInfo, 0, sizeof(s32)*LFcnt);

		for (size_t idx = 0; idx < strLen; ++idx)
		{
			if (vch[idx]->c == L'\\' && vch[idx + 1]->c == L'n')
			{
				lineInfoIndex++;
				++idx;
				continue;
			}

			lineInfo[lineInfoIndex] += vch[idx]->advanceX;
		}


		// 최하단까지 내려가는 글자를 기준으로 베이스라인을 잡는다.
		baseLine = baseLine + h0;
		// 베이스라인위로 올라가는 글자들이 삐져나가지 않도록 베이스라인에서 가장큰높이를 빼고 그의 반을 또 빼줘서 상하정렬을 해준다.
		baseLine -= (baseLine - height0) >> 1;

		width0 = MAX(width1, width0);
		w0  = width0;
		h1 *= LFcnt;
		width0 = NextP2(width0);
		height0 = NextP2(h1);


		m_Img = Image::Create();
		m_Img->SettexW(w0);
		m_Img->SettexH(h1);
		m_Img->SettexSizeW(width0);
		m_Img->SettexSizeH(height0);
		m_Img->SetDepth(4);

		SetSize(Size(w0, h1));

		u32* bitmap = new u32[width0*height0];
		memset(bitmap, 0, sizeof(u32)*width0*height0);

		// background color
		for (int j = 0; j < h1; j++) {
			for (int i = 0; i < w0; i++){
				s32 idx0 = (j*width0) + i;
				bitmap[idx0] = m_BgColor;
			}
		}


		s32 penx = 0;
		s32 peny = 0;
		bl  islf = false;
		lineInfoIndex = 0;
		
		for (size_t idx = 0; idx < strLen; ++idx)
		{
			if (vch[idx]->c == L'\\' && vch[idx + 1]->c == L'n')
			{
				SAFE_DELETE2_NULL(&vch[idx]->bitmap);
				SAFE_DELETE2_NULL(&vch[idx + 1]->bitmap);
				++idx;
				islf = true;
				lineInfoIndex++;
				continue;
			}

			// 개행처리
			if (islf)
			{
				penx = 0;
				baseLine += line_spacing;
			}
			
			if (lineInfo[lineInfoIndex] > 0)
			{
				// 가운데 정렬
				//penx = (w0>>1) - (lineInfo[lineInfoIndex]>>1);

				// 우측정렬
				penx = w0 - lineInfo[lineInfoIndex];

				// 좌측정렬
				//penx = 0;

				lineInfo[lineInfoIndex] = 0;

			

			}



			penx += vch[idx]->bitmap_left + vch[idx]->kerningX;
			peny = vch[idx]->advanceY + vch[idx]->bitmap_top;

			for (int j = 0; j < vch[idx]->height; j++) {
				for (int i = 0; i < vch[idx]->width; i++){

					s32 idx0 = (j*vch[idx]->width) + i;
					s32 offset = (j*width0) + (penx + i) + (width0 * (baseLine - peny));

					// 배경색이 없다면
					if (GETALPAH(bitmap[offset]) == 0)
					{
						if (GETALPAH(vch[idx]->bitmap[idx0]) != 0)  // 커닝정보에 의해 서로 겹쳐지는 부분에 알파값이 적용되지 않도록 알파값이 0이상인값만 복사한다.
							bitmap[offset] = vch[idx]->bitmap[idx0];
					}
					else {
						if (GETALPAH(vch[idx]->bitmap[idx0]) != 0)
							bitmap[offset] = AlphaBlending(vch[idx]->bitmap[idx0], bitmap[offset], Normal); // 전경, 배경

					}
				}
			}
			penx += (vch[idx]->advanceX - (vch[idx]->bitmap_left + vch[idx]->kerningX));

			SAFE_DELETE2_NULL(&vch[idx]->bitmap);
			islf = false;
		}


		glGenTextures(1, &m_Img->m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_Img->m_TextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width0, height0, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);


		
		SAFE_DELETE2_NULL(&lineInfo);
		SAFE_DELETE2_NULL(&bitmap);
	}


	return strLen;
}


size_t  Text::IsStartTag(const wchar_t* ch) const
{
	size_t istag = 0;
	if (*ch == '<' && *(ch+1) == '#')
	{
		istag = 2;
	}

	return istag;
}


size_t Text::FindTag(const wchar_t* ch, wchar_t tag) const
{
	size_t isfind = 0;
	s32 idx = 0;

	do
	{
		if (*(ch + idx) == tag)
		{
			isfind = (ch + idx) - ch;
			break;
		}

	} while (*(ch + idx++) != 0);

	return isfind;
}



size_t Text::GetTagInfo(const wchar_t* ch)
{
	size_t colonsize = 1;
	size_t istag = 0;
	size_t isendtag = 0;
	size_t iscolon = 0;
	wchar_t tag[10] = { 0, };
	wchar_t v0[10] = { 0, };
	char    v1[10] = { 0, };

	istag		= IsStartTag(ch);
	isendtag	= FindTag(ch + 2, L'>');
	iscolon		= FindTag(ch + 2, L':');

	if (istag>0 && isendtag>0 && iscolon > 0)
	{
		memcpy(tag, ch + 2, sizeof(wchar_t)*iscolon);

		if (UTF8Comp(tag, L"s") == 0)
		{
			s32 len = isendtag - iscolon - colonsize;
			memcpy(v0, ch + 2 + iscolon + colonsize, sizeof(wchar_t)*len);
			
			wtoa(v0, v1, len);
			m_Size = atoi(v1);
		}
		else if (UTF8Comp(tag, L"c") == 0)
		{
			s32 len = isendtag - iscolon - colonsize;
			memcpy(v0, ch + 2 + iscolon + colonsize, sizeof(wchar_t)*len);

			wtoa(v0, v1, len);
			u32 color = strtoul(v1, NULL, 16);

			m_R = ((unsigned)color>>16) & 0xff;
			m_G = ((unsigned)color>>8) & 0xff;
			m_B = (unsigned)color& 0xff;
			
		}
		else if (UTF8Comp(tag, L"bg") == 0)
		{
			s32 len = isendtag - iscolon - colonsize;
			memcpy(v0, ch + 2 + iscolon + colonsize, sizeof(wchar_t)*len);

			wtoa(v0, v1, len);
			m_BgColor = strtoi16(v1);
		}
		else if (UTF8Comp(tag, L"f") == 0)
		{
			s32 len = isendtag - iscolon - colonsize;
			memcpy(v0, ch + 2 + iscolon + colonsize, sizeof(wchar_t)*len);

			wtoa(v0, v1, len);
			m_FontIdx = atoi(v1);
		}
				
	
		istag = (isendtag == 0) ? 0 : isendtag + istag;
	}

	return istag;
}

wchar_t* Text::GetLine(sz*& txt, size_t size, u32 line, size_t* len)
{

	ASSERT(len != nullptr);

	wchar_t* wbuf;
	size_t strLen = 0;

	size_t s1 = UTF8StrToUnicodeStr(txt, size, nullptr);
	s1 += 1;
	wbuf = new wchar_t[s1];
	memset(wbuf, 0, sizeof(wchar_t)*s1);

	strLen = UTF8StrToUnicodeStr(txt, size, wbuf);

	u32 nLine = 0;
	bl  firstLine = false, nextLine = false;
	wchar_t* first = nullptr, *next = nullptr;

	firstLine = (line == 0 ? true : false);
	first = (line == 0 ? &wbuf[0] : nullptr);

	size_t i;
	for (i = 0; i < strLen; ++i)
	{
		if (firstLine == false && wbuf[i] == L'\r' && wbuf[i + 1] == L'\n')
			++nLine;

		if (nextLine == false && firstLine == true && wbuf[i] == L'\r' && wbuf[i + 1] == L'\n')
		{
			nextLine = true;
			next = &wbuf[i];
			break;
		}

		if (firstLine == false && line == nLine)
		{
			firstLine = true;
			first = &wbuf[i + 2];
		}
	}

	if (first != nullptr && *first == 0xFEFF) first = first + 1;	// utf 삽입글자는 지나친다.
	if (next == nullptr) next = &wbuf[i];							// 파일맨끝이다.

	// 파일맨끝이거나 \r\n만 있는 라인이다.
	strLen = next - first;
	if (strLen == 0 || first == nullptr)
	{
		strLen = 1;
		wbuf[0] = L' ';
		first = wbuf;
	}



	wchar_t* tmpbuf;
	tmpbuf = new wchar_t[strLen];
	memset(tmpbuf, 0, strLen*sizeof(wchar_t));
	memcpy(tmpbuf, first, strLen*sizeof(wchar_t));

	SAFE_DELETE_NULL(&wbuf);
	wbuf = new wchar_t[strLen + 1];
	memset(wbuf, 0, strLen*sizeof(wchar_t));
	memcpy(wbuf, tmpbuf, strLen*sizeof(wchar_t));
	wbuf[strLen] = 0;

	SAFE_DELETE_NULL(&tmpbuf);
	*len = strLen;
	return wbuf;
}


Text* Text::Create(u8* buf, size_t size, u32 line)
{
	ASSERT(buf != nullptr);

	wchar_t* wbuf;
	Text*   text;

	
	line = (line <= 0 ? 0 : --line);
	wbuf = GetLine((sz*&)buf, size, line, &size);
	text = new Text(wbuf, size);
	
	SAFE_DELETE_NULL(&wbuf);

	return text;
}

Text* Text::Create(sz* fileName, u32 line)
{
	u8*		buf;
	wchar_t* wbuf;
	size_t	len;
	Text*   text;
	buf = GetData(fileName, &len);
	ASSERT(buf != nullptr);

	LOG("wchar: %d\n", sizeof(wchar_t));

	line = (line <= 0 ? 0 : --line);
	wbuf = GetLine((sz*&)buf, len, line, &len);
	text = new Text(wbuf, len);

	SAFE_DELETE_NULL(&buf);
	SAFE_DELETE_NULL(&wbuf);

	return text;
}

NS_JMW_END
