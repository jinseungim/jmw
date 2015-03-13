#include "jmw.h"
#include "SevenZDec.h"


#include "7z/7z.h"
#include "7z/7zAlloc.h"
#include "7z/7zBuf.h"
#include "7z/7zCrc.h"
#include "7z/7zFile.h"
#include "7z/7zVersion.h"

#include <errno.h>




NS_JMW_BEGIN


static ISzAlloc g_Alloc = { SzAlloc, SzFree };

static int Buf_EnsureSize(CBuf *dest, size_t size)
{
	if (dest->size >= size)
		return 1;
	Buf_Free(dest, &g_Alloc);
	return Buf_Create(dest, size, &g_Alloc);
}





#ifndef _WIN32

static Byte kUtf8Limits[5] = { 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

static Bool Utf16_To_Utf8(Byte *dest, size_t *destLen, const UInt16 *src, size_t srcLen)
{
	size_t destPos = 0, srcPos = 0;
	for (;;)
	{
		unsigned numAdds;
		UInt32 value;
		if (srcPos == srcLen)
		{
			*destLen = destPos;
			return True;
		}
		value = src[srcPos++];
		if (value < 0x80)
		{
			if (dest)
				dest[destPos] = (char)value;
			destPos++;
			continue;
		}
		if (value >= 0xD800 && value < 0xE000)
		{
			UInt32 c2;
			if (value >= 0xDC00 || srcPos == srcLen)
				break;
			c2 = src[srcPos++];
			if (c2 < 0xDC00 || c2 >= 0xE000)
				break;
			value = (((value - 0xD800) << 10) | (c2 - 0xDC00)) + 0x10000;
		}
		for (numAdds = 1; numAdds < 5; numAdds++)
			if (value < (((UInt32)1) << (numAdds * 5 + 6)))
				break;
		if (dest)
			dest[destPos] = (char)(kUtf8Limits[numAdds - 1] + (value >> (6 * numAdds)));
		destPos++;
		do
		{
			numAdds--;
			if (dest)
				dest[destPos] = (char)(0x80 + ((value >> (6 * numAdds)) & 0x3F));
			destPos++;
		} while (numAdds != 0);
	}
	*destLen = destPos;
	return False;
}

static SRes Utf16_To_Utf8Buf(CBuf *dest, const UInt16 *src, size_t srcLen)
{
	size_t destLen = 0;
	Bool res;
	Utf16_To_Utf8(NULL, &destLen, src, srcLen);
	destLen += 1;
	if (!Buf_EnsureSize(dest, destLen))
		return SZ_ERROR_MEM;
	res = Utf16_To_Utf8(dest->data, &destLen, src, srcLen);
	dest->data[destLen] = 0;
	return res ? SZ_OK : SZ_ERROR_FAIL;
}

#endif






static SRes Utf16_To_Char(CBuf *buf, const UInt16 *s
#ifdef _WIN32
	, UINT codePage
#endif
	)
{
	unsigned len = 0;
	for (len = 0; s[len] != 0; len++);

#ifdef _WIN32
	{
		unsigned size = len * 3 + 100;
		if (!Buf_EnsureSize(buf, size))
			return SZ_ERROR_MEM;
		{
			buf->data[0] = 0;
			if (len != 0)
			{
				char defaultChar = '_';
				BOOL defUsed;
				unsigned numChars = 0;
				numChars = WideCharToMultiByte(codePage, 0, (WCHAR*)s, len, (char *)buf->data, size, &defaultChar, &defUsed);
				if (numChars == 0 || numChars >= size)
					return SZ_ERROR_FAIL;
				buf->data[numChars] = 0;
			}
			return SZ_OK;
		}
	}
#else
	return Utf16_To_Utf8Buf(buf, s, len);
#endif
}

#ifdef _WIN32
#ifndef USE_WINDOWS_FILE
static UINT g_FileCodePage = CP_ACP;
#endif
#define MY_FILE_CODE_PAGE_PARAM ,g_FileCodePage
#else
#define MY_FILE_CODE_PAGE_PARAM
#endif

static WRes MyCreateDir(const UInt16 *name)
{
#ifdef USE_WINDOWS_FILE

	return CreateDirectoryW((WCHAR*)name, NULL) ? 0 : GetLastError();

#else

	CBuf buf;
	WRes res;
	Buf_Init(&buf);
	RINOK(Utf16_To_Char(&buf, name MY_FILE_CODE_PAGE_PARAM));

	res =
#ifdef _WIN32
		_mkdir((const char *)buf.data)
#else
		mkdir((const char *)buf.data, 0777)
#endif
		== 0 ? 0 : errno;
	Buf_Free(&buf, &g_Alloc);
	return res;

#endif
}


static WRes OutFile_OpenUtf16(CSzFile *p, const UInt16 *name)
{
#ifdef USE_WINDOWS_FILE
	return OutFile_OpenW(p, (WCHAR*)name);
#else
	CBuf buf;
	WRes res;
	Buf_Init(&buf);
	RINOK(Utf16_To_Char(&buf, name MY_FILE_CODE_PAGE_PARAM));
	res = OutFile_Open(p, (const char *)buf.data);
	Buf_Free(&buf, &g_Alloc);
	return res;
#endif
}



static void UInt64ToStr(UInt64 value, char *s)
{
	char temp[32];
	int pos = 0;
	do
	{
		temp[pos++] = (char)('0' + (unsigned)(value % 10));
		value /= 10;
	} while (value != 0);
	do
		*s++ = temp[--pos];
	while (pos);
	*s = '\0';
}

static char *UIntToStr(char *s, unsigned value, int numDigits)
{
	char temp[16];
	int pos = 0;
	do
		temp[pos++] = (char)('0' + (value % 10));
	while (value /= 10);
	for (numDigits -= pos; numDigits > 0; numDigits--)
		*s++ = '0';
	do
		*s++ = temp[--pos];
	while (pos);
	*s = '\0';
	return s;
}

static void UIntToStr_2(char *s, unsigned value)
{
	s[0] = (char)('0' + (value / 10));
	s[1] = (char)('0' + (value % 10));
}

#define PERIOD_4 (4 * 365 + 1)
#define PERIOD_100 (PERIOD_4 * 25 - 1)
#define PERIOD_400 (PERIOD_100 * 4 + 1)

static void ConvertFileTimeToString(const CNtfsFileTime *nt, char *s)
{
	unsigned year, mon, hour, min, sec;
	Byte ms[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	unsigned t;
	UInt32 v;
	UInt64 v64 = nt->Low | ((UInt64)nt->High << 32);
	v64 /= 10000000;
	sec = (unsigned)(v64 % 60); v64 /= 60;
	min = (unsigned)(v64 % 60); v64 /= 60;
	hour = (unsigned)(v64 % 24); v64 /= 24;

	v = (UInt32)v64;

	year = (unsigned)(1601 + v / PERIOD_400 * 400);
	v %= PERIOD_400;

	t = v / PERIOD_100; if (t == 4) t = 3; year += t * 100; v -= t * PERIOD_100;
	t = v / PERIOD_4;   if (t == 25) t = 24; year += t * 4;   v -= t * PERIOD_4;
	t = v / 365;        if (t == 4) t = 3; year += t;       v -= t * 365;

	if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
		ms[1] = 29;
	for (mon = 0;; mon++)
	{
		unsigned s = ms[mon];
		if (v < s)
			break;
		v -= s;
	}
	s = UIntToStr(s, year, 4); *s++ = '-';
	UIntToStr_2(s, mon + 1); s[2] = '-'; s += 3;
	UIntToStr_2(s, (unsigned)v + 1); s[2] = ' '; s += 3;
	UIntToStr_2(s, hour); s[2] = ':'; s += 3;
	UIntToStr_2(s, min); s[2] = ':'; s += 3;
	UIntToStr_2(s, sec); s[2] = 0;
}



#ifdef USE_WINDOWS_FILE
static void GetAttribString(UInt32 wa, Bool isDir, char *s)
{
	s[0] = (char)(((wa & FILE_ATTRIBUTE_DIRECTORY) != 0 || isDir) ? 'D' : '.');
	s[1] = (char)(((wa & FILE_ATTRIBUTE_READONLY) != 0) ? 'R' : '.');
	s[2] = (char)(((wa & FILE_ATTRIBUTE_HIDDEN) != 0) ? 'H' : '.');
	s[3] = (char)(((wa & FILE_ATTRIBUTE_SYSTEM) != 0) ? 'S' : '.');
	s[4] = (char)(((wa & FILE_ATTRIBUTE_ARCHIVE) != 0) ? 'A' : '.');
	s[5] = '\0';
}
#else
static void GetAttribString(UInt32, Bool, char *s)
{
	s[0] = '\0';
}
#endif







SevenZDec::SevenZDec(const string src, const string dst, const string command)
	:m_Src(src), m_Dst(dst), m_Cmd(command)
{
}


SevenZDec::~SevenZDec()
{
}


void SevenZDec::Enter(void* param)
{
}

void* SevenZDec::Run(void* param)
{

	CFileInStream archiveStream;
	CLookToRead lookStream;
	CSzArEx db;
	SRes res;
	ISzAlloc allocImp;
	ISzAlloc allocTempImp;
	UInt16 *temp = NULL;
	size_t tempSize = 0;


	allocImp.Alloc = SzAlloc;
	allocImp.Free = SzFree;

	allocTempImp.Alloc = SzAllocTemp;
	allocTempImp.Free = SzFreeTemp;


	if (InFile_Open(&archiveStream.file, m_Src.c_str()))
	{
		LOG("can not find file");
		return 0;
	}

	FileInStream_CreateVTable(&archiveStream);
	LookToRead_CreateVTable(&lookStream, False);

	lookStream.realStream = &archiveStream.s;
	LookToRead_Init(&lookStream);

	CrcGenerateTable();

	SzArEx_Init(&db);
	res = SzArEx_Open(&db, &lookStream.s, &allocImp, &allocTempImp);
	if (res == SZ_OK)
	{
		UInt32 i;
		UInt32 blockIndex = 0xFFFFFFFF; /* it can have any value before first call (if outBuffer = 0) */
		Byte *outBuffer = 0; /* it must be 0 before first call for each new archive. */
		size_t outBufferSize = 0;  /* it can have any value before first call (if outBuffer = 0) */


		const char *command = m_Cmd.c_str();
		int listCommand = 0, testCommand = 0, extractCommand = 0, fullPaths = 0;
		if (strcmp(command, "l") == 0) listCommand = 1;
		else if (strcmp(command, "t") == 0) testCommand = 1;
		else if (strcmp(command, "e") == 0) extractCommand = 1;
		else if (strcmp(command, "x") == 0) { extractCommand = 1; fullPaths = 1; }


		for (i = 0; i < db.NumFiles; i++)
		{
			size_t offset = 0;
			size_t outSizeProcessed = 0;

			// const CSzFileItem *f = db.Files + i;
			size_t len;
			int isDir = SzArEx_IsDir(&db, i);
			if (listCommand == 0 && isDir && !fullPaths)
				continue;
			len = SzArEx_GetFileNameUtf16(&db, i, NULL);
			// len = SzArEx_GetFullNameLen(&db, i);

			if (m_Dst.size() > 0)
				len += m_Dst.size() + 1;

			if (len > tempSize)
			{
				SzFree(NULL, temp);
				tempSize = len;
				temp = (UInt16 *)SzAlloc(NULL, tempSize * sizeof(temp[0]));
				if (!temp)
				{
					res = SZ_ERROR_MEM;
					break;
				}
			}

			SzArEx_GetFileNameUtf16(&db, i, temp);


			//LOG("temp name: %S", temp);

			if (listCommand)
			{
				char attr[8], s[32], t[32];
				UInt64 fileSize;

				GetAttribString(SzBitWithVals_Check(&db.Attribs, i) ? db.Attribs.Vals[i] : 0, isDir, attr);

				fileSize = SzArEx_GetFileSize(&db, i);
				UInt64ToStr(fileSize, s);
				if (SzBitWithVals_Check(&db.MTime, i))
					ConvertFileTimeToString(&db.MTime.Vals[i], t);
				else
				{
					size_t j;
					for (j = 0; j < 19; j++)
						t[j] = ' ';
					t[j] = '\0';
				}

				LOG("%s %s %s  ", t, attr, s);

				if (res != SZ_OK)
					break;
				if (isDir)
					LOG("/");
				LOG("\n");
				continue;
			}


			if (isDir)
				LOG("/");
			else
			{
				res = SzArEx_Extract(&db, &lookStream.s, i,
					&blockIndex, &outBuffer, &outBufferSize,
					&offset, &outSizeProcessed,
					&allocImp, &allocTempImp);
				if (res != SZ_OK)
					break;
			}


			if (!testCommand)
			{
				CSzFile outFile;
				size_t processedSize;
				size_t j;

				
				if (m_Dst.size()>0)
				{

					UInt16 *swapTemp = NULL;
					size_t size0 = (len - m_Dst.size()-1) * sizeof(swapTemp[0]);
					swapTemp = (UInt16 *)SzAlloc(NULL, size0);
					if (swapTemp)
					{
						memcpy(swapTemp, temp, size0);
					}
					else
					{
						res = SZ_ERROR_MEM;
						break;
					}
					
					size_t i = 0;
					for (i = 0; i < m_Dst.size(); i++)
					{
						temp[i] = m_Dst.c_str()[i];
					}
					temp[i++] = '/';
					for (size_t z = 0; z < size0>>1; z++)
					{
						temp[i+z] = swapTemp[z];
					}
					SzFree(NULL, swapTemp);


				}

				UInt16 *name = (UInt16 *)temp;
				const UInt16 *destPath = (const UInt16 *)name;
				for (j = 0; name[j] != 0; j++)
					if (name[j] == '/')
					{
						if (fullPaths)
						{
							name[j] = 0;
							MyCreateDir(name);
							name[j] = CHAR_PATH_SEPARATOR;
						}
						else
							destPath = name + j + 1;
					}

				if (isDir)
				{
					MyCreateDir(destPath);
					LOG("\n");
					continue;
				}
				else if (OutFile_OpenUtf16(&outFile, destPath))
				{
					LOG("can not open output file");
					res = SZ_ERROR_FAIL;
					break;
				}
				processedSize = outSizeProcessed;
				if (File_Write(&outFile, outBuffer + offset, &processedSize) != 0 || processedSize != outSizeProcessed)
				{
					LOG("can not write output file");
					res = SZ_ERROR_FAIL;
					break;
				}
				if (File_Close(&outFile))
				{
					LOG("can not close output file");
					res = SZ_ERROR_FAIL;
					break;
				}
#ifdef USE_WINDOWS_FILE
				if (SzBitWithVals_Check(&db.Attribs, i))
					SetFileAttributesW((WCHAR*)destPath, db.Attribs.Vals[i]);
#endif
			}
			LOG("\n");
		}
		IAlloc_Free(&allocImp, outBuffer);

	}


	SzArEx_Free(&db, &allocImp);
	SzFree(NULL, temp);

	File_Close(&archiveStream.file);
	if (res == SZ_OK)
	{
		LOG("\nEverything is Ok\n");
	}
	if (res == SZ_ERROR_UNSUPPORTED)
		LOG("decoder doesn't support this archive");
	else if (res == SZ_ERROR_MEM)
		LOG("can not allocate memory");
	else if (res == SZ_ERROR_CRC)
		LOG("CRC error");
	else
		LOG("\nERROR #%d\n", res);

	return 0;
}

void SevenZDec::Exit(void* param)
{

}
NS_JMW_END