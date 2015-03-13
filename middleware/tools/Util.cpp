#include "jmw.h"
#include "Util.h"
#include <fstream>


size_t filesize(const char* filename)
{
	std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
	return in.tellg();
}




static size_t UnicodeToUTF8(wchar_t uc, char* UTF8)
{
	size_t tRequiredSize = 0;

	if (uc <= 0x7f)
	{
		if (NULL != UTF8)
		{
			UTF8[0] = (char)uc;
			UTF8[1] = (char) '\0';
		}
		tRequiredSize = 1;
	}
	else if (uc <= 0x7ff)
	{
		if (NULL != UTF8)
		{
			UTF8[0] = (char)(0xc0 + uc / (0x01 << 6));
			UTF8[1] = (char)(0x80 + uc % (0x01 << 6));
			UTF8[2] = (char) '\0';
		}
		tRequiredSize = 2;
	}
	else if (uc <= 0xffff)
	{
		if (NULL != UTF8)
		{
			UTF8[0] = (char)(0xe0 + uc / (0x01 << 12));
			UTF8[1] = (char)(0x80 + uc / (0x01 << 6) % (0x01 << 6));
			UTF8[2] = (char)(0x80 + uc % (0x01 << 6));
			UTF8[3] = (char) '\0';
		}
		tRequiredSize = 3;
	}

	return tRequiredSize;
}


static size_t UTF8ToUnicode(char* UTF8, wchar_t& uc)
{
	size_t tRequiredSize = 0;

	uc = 0x0000;

	// ASCII byte ascii 는 127까지 값으로 표현
	if (0 == (UTF8[0] & 0x80))
	{
		uc = UTF8[0];
		tRequiredSize = 1;
	}
	else // Start byte for 2byte
		if (0xC0 == (UTF8[0] & 0xE0) &&
			0x80 == (UTF8[1] & 0xC0))
		{
		uc += (UTF8[0] & 0x1F) << 6;
		uc += (UTF8[1] & 0x3F) << 0;
		tRequiredSize = 2;
		}
		else // Start byte for 3byte
			if (0xE0 == (UTF8[0] & 0xE0) &&
				0x80 == (UTF8[1] & 0xC0) &&
				0x80 == (UTF8[2] & 0xC0))
			{
		uc += (UTF8[0] & 0x1F) << 12;
		uc += (UTF8[1] & 0x3F) << 6;
		uc += (UTF8[2] & 0x3F) << 0;
		tRequiredSize = 3;
			}
			else
			{
				// Invalid case
				assert(false);
			}

	return tRequiredSize;
}


size_t UnicodeStrToUTF8Str(wchar_t *szUni, char *szUTF8)
{
	size_t tRequiredSize = 0;

	int i = 0;
	for (i = 0; szUni[i]; i++)
	{
		size_t tSize = 0;
		if (NULL != szUTF8)
			tSize = UnicodeToUTF8(szUni[i], szUTF8 + tRequiredSize);
		else
			tSize = UnicodeToUTF8(szUni[i], NULL);
		tRequiredSize += tSize;
	}
	return tRequiredSize;
}

size_t UTF8StrToUnicodeStr(char* szUTF8, size_t tUTF8Len, wchar_t* szUni)
{
	size_t tReadPos = 0;

	size_t i = 0;
	for (i = 0; tReadPos<tUTF8Len; i++)
	{
		wchar_t tTempUnicodeChar = 0;
		size_t tSize = UTF8ToUnicode(szUTF8 + tReadPos, tTempUnicodeChar);
		if (NULL != szUni)
			szUni[i] = tTempUnicodeChar;
		tReadPos += tSize;
	}
	return i;
}

s32 UTF8Len(const wchar_t* src)
{
	s32 idx = 0;
	do
	{
		if (*(src + idx++) == 0)
		{
			--idx;
			break;
		}
	} while (idx < MAX_PATH_LEN);

	return idx;
}


s32 UTF8Comp(const wchar_t* src, const wchar_t* dst)
{
	s32 srcLen = 0;
	s32 dstLen = 0;
	s32 comp = 0;
	s32 idx = 0;

	srcLen = UTF8Len(src);
	dstLen = UTF8Len(dst);

	if (srcLen != dstLen)
	{
		comp = srcLen - dstLen;
	}
	else
	{
		do
		{
			if (*(src + idx) != *(dst+idx))
			{
				comp = *(src + idx) - *(dst + idx);
				break;
			}

			++idx;
		} while (idx < srcLen);
	}


	return comp;
}


s32 NextP2(s32 a)
{
	int rval = 1;    // rval<<=1 Is A Prettier Way Of Writing rval*=2;     
	while(rval<a) rval<<=1;    
	return rval;
}


u32 GetXdpi(void)
{
	static u32 x0 = 0;

	if (x0 == 0)
	{
#if defined(__ANDROID__)
		x0 = 96;
#elif defined(WIN32) || defined(__MINGW__)
		HDC hdc = GetDC(glfwGetWin32Window(window));

		x0 = GetDeviceCaps(hdc, LOGPIXELSX);
#endif
	}
	return x0;
}


u32 GetYdpi(void)
{
	static u32 y0 = 0;

	if (y0 == 0)
	{
#if defined(__ANDROID__)
		y0 = 96;
#elif defined(WIN32) || defined(__MINGW__)
		HDC hdc = GetDC(glfwGetWin32Window(window));

		y0 = GetDeviceCaps(hdc, LOGPIXELSY);
#endif
	}
	return y0;
}



unsigned AlphaBlending(unsigned src, unsigned dst, BlendingMode mode)
{
	unsigned result = 0;
	int r=0, g=0, b=0;
	unsigned srcA, dstR, dstG, dstB, srcR, srcG, srcB;
	float alphaFactor;

	srcA = (src & 0xff000000) >> 24;
	srcB = (src & 0xff0000) >> 16;
	srcG = (src & 0x00ff00) >> 8;
	srcR = (src & 0x0000ff);

	dstB = (dst & 0xff0000) >> 16;
	dstG = (dst & 0x00ff00) >> 8;
	dstR = (dst & 0x0000ff);

	alphaFactor = srcA / 255.f;

	if (Normal == mode)
	{
		// 소스 알파를 기준으로 블렌딩
		r = (srcR*alphaFactor) + dstR * (1.0 - alphaFactor);
		g = (srcG*alphaFactor) + dstG * (1.0 - alphaFactor);
		b = (srcB*alphaFactor) + dstB * (1.0 - alphaFactor);

		result = ABGR(r, g, b, 255);
	}
	else if (Darken == mode)
	{
		// 두 이미지 중에서 가장 어두운 부분만 추출
		r = MIN(srcR, dstR);
		g = MIN(srcG, dstG);
		b = MIN(srcB, dstB);
	}
	else if (Multiply == mode)
	{
		// 두 색을 곱합니다. 원본보다 어두워 진다.
		r = ((srcR / 255.f) * (dstR / 255.f)) * 255;
		g = ((srcG / 255.f) * (dstG / 255.f)) * 255;
		b = ((srcB / 255.f) * (dstB / 255.f)) * 255;
		
	}
	else if (Lighten == mode)
	{
		// 두 색 중 밝은 것을 추출
		r = MAX(srcR, dstR);
		g = MAX(srcG, dstG);
		b = MAX(srcB, dstB);
	}
	else if (Screen == mode)
	{
		// 혼합 색상(Src)과 기본 색상(Dest)의 반전색을 곱합니다.
		
		r = srcR * (1.0 - (dstR/255.f)) + dstR;
		g = srcG * (1.0 - (dstG/255.f)) + dstG;
		b = srcB * (1.0 - (dstB/255.f)) + dstB;
		
		r = MIN(r, 255);
		g = MIN(g, 255);
		b = MIN(b, 255);

	}
	else if (LinearDodge == mode)
	{
		// 각 채널의 색상을 더해 명도를 증가 시킵니다.

		r = srcR + dstR;
		g = srcG + dstG;
		b = srcB + dstB;

		r = MIN(r, 255);
		g = MIN(g, 255);
		b = MIN(b, 255);
	}
	else if (Subtract == mode)
	{
		// 혼합 색상(Src)과 기본 색상(Dest)을 뺀다

		r = abs((int)srcR - (int)dstR);
		g = abs((int)srcG - (int)dstG);
		b = abs((int)srcB - (int)dstB);

	}
	else if (Opaque == mode)
	{
		// 반투명
		r = (srcR >> 1) + (dstR >> 1);
		g = (srcG >> 1) + (dstG >> 1);
		b = (srcB >> 1) + (dstB >> 1);
	}
	


	if (Normal != mode)
	{
		// 투명값이 있다면 그값을 적용한다.
		r = (r*alphaFactor) + dstR * (1.0 - alphaFactor);
		g = (g*alphaFactor) + dstG * (1.0 - alphaFactor);
		b = (b*alphaFactor) + dstB * (1.0 - alphaFactor);

		result = ABGR(r, g, b, 255);
	}


	
	return result;
}




u8* GetData(const string fileName, size_t* size, s32 mode)
{
	u8* pdata = nullptr;
	ASSERT(size != nullptr);
	*size = 0;

#if defined(__ANDROID__)
	if(mode==0)
		pdata = GetRes(fileName.c_str(), size);
	else
		pdata = GetDataStream(fileName, size);
#elif defined(WIN32) || defined(__MINGW__)
	pdata = GetDataStream(fileName, size);

#endif

	return pdata;
}



u8* GetDataStream(const string fileName, size_t* size)
{
	u8* pdata = nullptr;
	ASSERT(size != nullptr);
	*size = 0;

	ifstream fin(fileName, ios::in | ios::binary);

	if (fin.is_open())
	{
		size_t size0 = 0;
		size0 = filesize(fileName.c_str());
		pdata = new u8[size0];

		size_t nread = 0;
		do{
			fin.read((char*)pdata + nread, size0 - nread);
			nread = fin.gcount();

			if (nread == 0) break;
		} while (1);
		fin.close();
		*size = size0;
	}
	return pdata;
}

unsigned strtoi16(const char* str)
{
	char s[3] = { 0, };
	unsigned ret = 0;
	int len = strlen(str);
	len = len > 8 ? 8 : len;
	len = len % 2 == 0 ? len : 0;

	for (int i = 0; i<len; i += 2)
	{
		ret <<= 8;

		s[0] = str[i];
		s[1] = str[i + 1];

		int a = strtoul(s, nullptr, 16);

		ret |= a;
	}

	return ret;
}

void wtoa(const wchar_t* src, char* dst, s32 len)
{
	for (int i = 0; i < len; ++i)
	{
		dst[i] = src[i] & 0xff;
	}
}