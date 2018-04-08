#pragma once

#define IS_SPACE_CHAR(c) ((c)==0x20 || ((c)>=0x9 && (c)<=0xD)

#define MEM_REALLOC_SIZE		32

// utf8 bom endianness
#define UTF8_BOM				0xBFBBEF
#define UTF8_BOM_SIZE			3

// utf16 bom endianness
#define UTF16_BOM				0xFEFF
#define UTF16_BOM_SIZE			2

namespace Str
{
	int _LengthA(LPCSTR pStr);
	int _LengthW(LPCWSTR pStr);
	void _CopyA(LPSTR pDest, LPCSTR pSrc, int iSize);
	void _CopyW(LPWSTR pDest, LPCWSTR pSrc, int iSize);
	void _CatA(LPSTR pDest, LPCSTR pSrc, size_t iSize);
	void _CatW(LPWSTR pDest, LPCWSTR pSrc, size_t iSize);
	int _CompareA(LPCSTR pStr1, LPCSTR pStr2, int iSize1, int iSize2);
	int _CompareW(LPCWSTR pStr1, LPCWSTR pStr2, int iSize1, int iSize2);
	int _sprintfA(LPSTR pBuf, int iBufSize, LPCSTR pStrFormat, ...);
	int _sprintfW(LPWSTR pBuf, int iBufSize, LPCWSTR pStrFormat, ...);
	int _vsprintfA(LPSTR pBuf, int iBufSize, LPCSTR pStrFormat, va_list arglist);
	int _vsprintfW(LPWSTR pBuf, int iBufSize, LPCWSTR pStrFormat, va_list arglist);
	int _ExtractA(LPSTR pStr, int delim, LPSTR output);
	int _ExtractW(LPWSTR pStr, int delim, LPWSTR output);
	int _getdelimitedinput(LPSTR *output, FILE *fp, int delim);
	int _getdelimitedinput(LPWSTR *output, FILE *fp, int delim);
}
