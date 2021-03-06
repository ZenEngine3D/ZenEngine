#include "zbBase.h"

namespace zbFile
{

FileInfo_PC::FileInfo_PC()
: mhFileHandle(INVALID_HANDLE_VALUE)
{

}

void FileInfo_PC::Close(zI64& iFilePos, zI64& iFileSize, bool& bValidHandle)
{
	if( mhFileHandle != INVALID_HANDLE_VALUE )
	{
		CloseHandle(mhFileHandle);

		mhFileHandle	= INVALID_HANDLE_VALUE;
		iFilePos		= 0;
		iFileSize		= 0;
		bValidHandle	= false;
	}
}

void FileInfo_PC::Seek(zI64& iNewFilePos, const zI64 iFilePos)
{
	if( mhFileHandle != INVALID_HANDLE_VALUE )
	{
		LONG iPos = (LONG)iFilePos;
		DWORD dwFilePointer = SetFilePointer( mhFileHandle, iPos, nullptr, FILE_BEGIN );
		if( dwFilePointer != INVALID_SET_FILE_POINTER )
		{
			//Ares_ErrorSystem::SetError( eAres_Error_SeekOutsideOfFile );
		}
		else
		{
			iNewFilePos = iFilePos;
		}
	}
}

void FileInfo_PC::Skip(zI64& iFilePos, const zI32 iNumOfBytes)
{
	if( mhFileHandle != INVALID_HANDLE_VALUE )
	{
		DWORD dwFilePointer = SetFilePointer( mhFileHandle, iNumOfBytes, nullptr, FILE_CURRENT );
		if( dwFilePointer != INVALID_SET_FILE_POINTER )
		{
			//Ares_ErrorSystem::SetError( eAres_Error_SeekOutsideOfFile );
		}
		else
		{
			iFilePos += iNumOfBytes;
		}
	}
}

void FileInfo_PC::Read(zI64& iFilePos, void* pxDest, const zU32 uNumBytes)
{
	if( mhFileHandle != INVALID_HANDLE_VALUE )
	{
		DWORD dwBytesRead = 0;
		BOOL bResult = ReadFile( mhFileHandle, pxDest, uNumBytes, &dwBytesRead, nullptr );
		if( !( bResult && ( dwBytesRead == uNumBytes ) ) )
		{
			//Ares_ErrorSystem::SetError( eAres_Error_Read );
		}
		else
		{
			iFilePos += uNumBytes;
		}
	}
}

void FileInfo_PC::Write(zI64& iFilePos, zI64& iFileSize, const void* pxSource, const zU32 uNumBytes)
{
	if( mhFileHandle != INVALID_HANDLE_VALUE )
	{
		DWORD dwBytesWriten = 0;
		BOOL bResult = WriteFile( mhFileHandle, pxSource, uNumBytes, &dwBytesWriten, nullptr );

		if( !( bResult && ( dwBytesWriten == uNumBytes ) ) )
		{
			//Ares_ErrorSystem::SetError( eAres_Error_Write );
		}
		else
		{
			iFilePos += uNumBytes;
			if(iFilePos > iFileSize)
			{
				iFileSize = iFilePos;
			}
		}
	}
}

void FileInfo_PC::Flush()
{
	if( mhFileHandle != INVALID_HANDLE_VALUE )
	{
		FlushFileBuffers( mhFileHandle );
	}
}

}