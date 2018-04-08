#include <stdio.h>
#include <Windows.h>
#include <Shlwapi.h>

#include "str.h"
#include "mem.h"

#pragma comment (lib, "shlwapi.lib")

int Str::_LengthA(LPCSTR pStr)
{
	int c=0;

	if( pStr!=NULL )
	{
		while( pStr[c]!=0 ) c++;
	}

	return c;
}

int Str::_LengthW(LPCWSTR pStr)
{
	int c=0;

	if( pStr!=NULL )
	{
		while( pStr[c]!=0 ) c++;
	}

	return c;
}

void Str::_CopyA(LPSTR pDest, LPCSTR pSrc, int iSize)
{
	if( iSize==-1 ) iSize=_LengthA(pSrc);

	Mem::_copy(pDest, pSrc, iSize);
	pDest[iSize]=0;

	return;
}

void Str::_CopyW(LPWSTR pDest, LPCWSTR pSrc, int iSize)
{
	if( iSize==-1 ) iSize=_LengthW(pSrc);

	Mem::_copy(pDest, pSrc, iSize*sizeof(WCHAR));
	pDest[iSize]=0;

	return;
}

void Str::_CatA(LPSTR pDest, LPCSTR pSrc, size_t iSize)
{
	_CopyA(pDest+_LengthA(pDest), pSrc, iSize);

	return;
}

void Str::_CatW(LPWSTR pDest, LPCWSTR pSrc, size_t iSize)
{
	_CopyW(pDest+_LengthW(pDest), pSrc, iSize);

	return;
}

int Str::_CompareA(LPCSTR pStr1, LPCSTR pStr2, int iSize1, int iSize2)
{
	if( pStr1==NULL && pStr2!=NULL ) return -1;
	if( pStr1!=NULL && pStr2==NULL ) return 1;
	if( pStr1==NULL && pStr2==NULL ) return 0;

	if( iSize1==-1 && iSize2==-1 )
	{
		while( (iSize1=*pStr1-*pStr2)==0 && *pStr2!=0 )
		{
			pStr1++;
			pStr2++;
		}
	}
	else
	{
		if( iSize1==-1 ) iSize1=_LengthA(pStr1);
		if( iSize2==-1 ) iSize2=_LengthA(pStr2);

		if( iSize1!=iSize2 || iSize1==0 || iSize2==0 ) iSize1-=iSize2;
		else
		{
			int c=0;

			for( c=0; c<iSize2; ++c )
			{
				if( (iSize1=*pStr1-*pStr2)!=0 ) break;
				pStr1++;
				pStr2++;
			}
		}
	}

	return ( iSize1==0 ? 0 : ( iSize1>0 ? 1 : -1 ) );
}

int Str::_CompareW(LPCWSTR pStr1, LPCWSTR pStr2, int iSize1, int iSize2)
{
	if( pStr1==NULL && pStr2!=NULL ) return -1;
	if( pStr1!=NULL && pStr2==NULL ) return 1;
	if( pStr1==NULL && pStr2==NULL ) return 0;

	if( iSize1==-1 && iSize2==-1 )
	{
		while( (iSize1=*pStr1-*pStr2)==0 && *pStr2!=0 )
		{
			pStr1++;
			pStr2++;
		}
	}
	else
	{
		if( iSize1==-1 ) iSize1=_LengthW(pStr1);
		if( iSize2==-1 ) iSize2=_LengthW(pStr2);

		if( iSize1!=iSize2 || iSize1==0 || iSize2==0 ) iSize1-=iSize2;
		else
		{
			int c=0;

			for( c=0; c<iSize2; ++c )
			{
				if( (iSize1=*pStr1-*pStr2)!=0 ) break;
				pStr1++;
				pStr2++;
			}
		}
	}

	return ( iSize1==0 ? 0 : ( iSize1>0 ? 1 : -1 ) );
}

int Str::_vsprintfA(LPSTR pBuf, int iBufSize, LPCSTR pStrFormat, va_list arglist)
{
	if( iBufSize<=0 ) return -1;

	Mem::_zero(pBuf, iBufSize*sizeof(char));
	int iSize=wvnsprintfA(pBuf, iBufSize, pStrFormat, arglist);
	pBuf[iBufSize-1]=0;

	if( iSize==-1 )
	{
		iSize=_LengthA(pBuf);
	}
	else
	{
		pBuf[iSize]=0;
	}

	return iSize;
}

int Str::_vsprintfW(LPWSTR pBuf, int iBufSize, LPCWSTR pStrFormat, va_list arglist)
{
	if( iBufSize<=0 ) return -1;

	Mem::_zero(pBuf, sizeof(WCHAR)*iBufSize);
	int iSize=wvnsprintfW(pBuf, iBufSize, pStrFormat, arglist);
	pBuf[iBufSize-1]=0;

	if( iSize==-1 )
	{
		iSize=_LengthW(pBuf);
	}
	else
	{
		pBuf[iSize]=0;
	}

	return iSize;
}

int Str::_sprintfA(LPSTR pBuf, int iBufSize, LPCSTR pStrFormat, ...)
{
	va_list list;
	va_start(list, pStrFormat);
	int iSize=_vsprintfA(pBuf, iBufSize, pStrFormat, list);
	va_end(list);

	return iSize;
}

int Str::_sprintfW(LPWSTR pBuf, int iBufSize, LPCWSTR pStrFormat, ...)
{
	va_list list;
	va_start(list, pStrFormat);
	int iSize=_vsprintfW(pBuf, iBufSize, pStrFormat, list);
	va_end(list);

	return iSize;
}

int Str::_ExtractA(LPSTR str, int delim, LPSTR output)
{
	if( output == NULL )
	{
		return -1;
	}

	int i = 0, len = 0;
	while( *str != NULL && *str != (char)delim )
	{
		output[i] = *str;
		*str++;
	}
	output[i] = '\0';
	len = i;

	return len;
}

int Str::_ExtractW(LPWSTR str, int delim, LPWSTR output)
{
	if( output == NULL )
	{
		return -1;
	}

	int i = 0, len = 0;
	while( *str != NULL && *str != (char)delim )
	{
		output[i] = *str;
		*str++;
	}
	output[i] = '\0';
	len = i;

	return len;
}

int Str::_getdelimitedinput(LPSTR *output, FILE *fp, int delim)
{
	if( fp == NULL )
	{
		return -1;
	}

	int i = 0, total = 0, bytes_read = 0, count = 0, init_buff = MEM_REALLOC_SIZE;
	char c;

	(*output) = (char*)Mem::_alloc(sizeof(char) * init_buff);
	if( (*output) == NULL )
	{
		printf("error in malloc\n");
		exit(-1);
	}
	do
	{
		if( count >= (init_buff - 1) )
		{
			init_buff += count + init_buff + MEM_REALLOC_SIZE;
			char *temp = (char*)Mem::_realloc((*output), init_buff);
			if( temp == NULL )
			{
				printf("error in realloc\n");
				exit(-1);
			}
			(*output) = temp;
		}
		
		bytes_read = fread(&c, sizeof(char), 1, fp);

		(*output)[count] = c;
		count = count + bytes_read;
		total = total + bytes_read;
	}while( bytes_read > 0 && c != (char)delim );
	total = total - 1;
	(*output)[count - 1] = 0;

	return bytes_read;
}

int Str::_getdelimitedinput(LPWSTR *output, FILE *fp, int delim)
{
	return 0;
}

