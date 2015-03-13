#pragma once

typedef enum
{
	Normal,
	Darken,
	Multiply,
	Lighten,
	Screen,
	LinearDodge,
	Subtract,
	Opaque,

}BlendingMode;

size_t	filesize(const char* filename);
size_t	UnicodeStrToUTF8Str(wchar_t *szUni, char *szUTF8);
size_t	UTF8StrToUnicodeStr(char* szUTF8, size_t tUTF8Len, wchar_t* szUni);
s32     UTF8Len(const wchar_t* src);
s32     UTF8Comp(const wchar_t* src, const wchar_t* dst);
s32		NextP2(s32 a);
u32		GetXdpi(void);
u32		GetYdpi(void);

// 전경
// 배경
unsigned AlphaBlending(unsigned src, unsigned dst, BlendingMode mode = Normal);


u8*		 GetData(const string, size_t*, s32 mode=0);
u8*		 GetDataStream(const string, size_t*);


unsigned strtoi16(const char* str);
void     wtoa(const wchar_t* src, char* dst, s32 len);