#include "zbBase.h"

namespace zbFile
{

bool ManagerFile_PC::Load()
{
	bool bValid(false);
	wchar_t zWorkingDir[512];
	GetCurrentDirectory( zenArrayCount(zWorkingDir), zWorkingDir );
	size_t uPosEnd = wcslen(zWorkingDir);
	while( !bValid && zWorkingDir[0] != 0 )
	{
		// Look for 'Content' directory in working path		
		_snwprintf_s( &zWorkingDir[uPosEnd], zenArrayCount(zWorkingDir)-uPosEnd, _TRUNCATE, L"/Content");
		WIN32_FIND_DATA	FileInfo;
		HANDLE hSearch	= FindFirstFile(zWorkingDir, &FileInfo);		
		if( hSearch != INVALID_HANDLE_VALUE )
		{
			FindClose(hSearch);
			bValid = (FileInfo.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) != 0;
			if( bValid )
			{
				//mzRoot.resize( static_cast<zUInt>(wcslen(zWorkingDir) + 1) );
				//_snwprintf_s( mzRoot.front(), mzRoot.size(), _TRUNCATE, zWorkingDir);
				InitPath(zWorkingDir);
				SetCurrentDirectory( mzRootPath.c_str() );				
			}
		}

		// If none found, look in parent directory
		--uPosEnd;
		while( !bValid && (uPosEnd > 0) && (zWorkingDir[uPosEnd]!= L'\\' && zWorkingDir[uPosEnd]!= L'/') )
			--uPosEnd;
	}
	//! @todo Safety Check if content folder hasn't been found. Currently crash engine
	return bValid;
}

bool ManagerFile_PC::Unload()
{
	return true;
}

bool ManagerFile_PC::Search(zArrayDyn<FileInfo>& _ResultOut, zUInt _uWantedFlag, const wchar_t* _zDirName, const wchar_t* _zFilePatern, bool bRecursive)
{
	zenAssert(_zDirName );
	zenAssert(_zFilePatern );
			
	_ResultOut.clear();
	_ResultOut.Reserve(32);
	
	_zDirName	= (_zDirName[0] == 0) ? L"." : _zDirName;
	zUInt len	= static_cast<zUInt>(wcslen(_zDirName));
	zArrayDyn<wchar_t> zPath;
	zPath.Reserve(1024);
	zPath.Copy(_zDirName, len + 1);
	if( zPath[-2] != L'/' && zPath[-2] != L'\\'  )
	{
		zPath.resize(static_cast<zUInt>(zPath.size() + 1));
		zPath[-2] = L'/';
		zPath[-1] = 0;
	}
	LoadDirectory(_ResultOut, _uWantedFlag, zPath, _zFilePatern, static_cast<zUInt>(wcslen(_zFilePatern)), bRecursive);
	return _ResultOut.size() > 0;
}

void ManagerFile_PC::LoadDirectory(zArrayDyn<FileInfo>& _ResultOut, zUInt _uWantedFlag, zArrayDyn<wchar_t>& _zDirName, const wchar_t* _zFilePatern, const zUInt _uFilePaternLen, bool bRecursive)
{
	WIN32_FIND_DATA	sysFileInfo;
	zUInt uPathSizeInitial(_zDirName.size());	

	//-------------------------------------------------------------------------
	// Look inside each directory when recursive is enabled
	//-------------------------------------------------------------------------
	if( bRecursive )
	{
		// Append search pattern	
		_zDirName.resize( uPathSizeInitial + 1 );	
		_zDirName[uPathSizeInitial-1]	= L'*';	
		_zDirName[uPathSizeInitial]		= 0;
		HANDLE hSearch					= FindFirstFile(_zDirName.Data(), &sysFileInfo);
		// Parse all children directories
		if(hSearch != INVALID_HANDLE_VALUE)
		{
			do
			{
				if( (sysFileInfo.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) != 0 && 
					!(sysFileInfo.cFileName[0] == L'.' && sysFileInfo.cFileName[1] == 0) &&
					!(sysFileInfo.cFileName[0] == L'.' && sysFileInfo.cFileName[1] == L'.' && sysFileInfo.cFileName[2] == 0) )
				{				
					zUInt len = static_cast<zUInt>(wcslen(sysFileInfo.cFileName));
					_zDirName.resize( static_cast<zUInt>(uPathSizeInitial + len + 1));
					wcsncpy_s( &_zDirName[uPathSizeInitial-1], len+1, sysFileInfo.cFileName, _TRUNCATE );
					_zDirName[uPathSizeInitial+len-1]	= L'/';
					_zDirName[uPathSizeInitial+len]		= 0;
					LoadDirectory(_ResultOut, _uWantedFlag, _zDirName, _zFilePatern, _uFilePaternLen, bRecursive);
				}

			}
			while( FindNextFile(hSearch, &sysFileInfo) );
			FindClose(hSearch);
		}
	}

	//-------------------------------------------------------------------------
	// Look for all files/dir matching pattern in current directory
	//-------------------------------------------------------------------------
	_zDirName.resize( uPathSizeInitial + _uFilePaternLen );	//Append search pattern	
	wcsncpy_s( &_zDirName[uPathSizeInitial-1], _uFilePaternLen+1, _zFilePatern, _TRUNCATE );	
	HANDLE hSearch = FindFirstFile(_zDirName.Data(), &sysFileInfo);	
	_zDirName[uPathSizeInitial-1] = 0;
	if(hSearch != INVALID_HANDLE_VALUE)
	{		
		do
		{
			bool bValid;
			zUInt uFileFlag(0);			
			uFileFlag	|= (sysFileInfo.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)!=0	? keFileFlag_Dir		: keFileFlag_File;
			uFileFlag	|= (sysFileInfo.dwFileAttributes&FILE_ATTRIBUTE_READONLY)!=0	? keFileFlag_ReadOnly	: keFileFlag_Writeable;
			uFileFlag	|= (sysFileInfo.dwFileAttributes&FILE_ATTRIBUTE_HIDDEN)!=0		? keFileFlag_Hidden		: 0;
			
			bValid	=	(_uWantedFlag&uFileFlag&keFileFlag_Dir)!=0 || (_uWantedFlag&uFileFlag&keFileFlag_File)!=0;
			bValid &=	(_uWantedFlag&keFileFlag_ReadOnly)==0	|| (uFileFlag&keFileFlag_ReadOnly)!=0;
			bValid &=	(_uWantedFlag&keFileFlag_Writeable)==0	|| (uFileFlag&keFileFlag_Writeable)!=0;
			bValid &=	(uFileFlag&keFileFlag_Hidden)==0		|| (_uWantedFlag&keFileFlag_Hidden)!=0;
			bValid &=	!(sysFileInfo.cFileName[0] == L'.' && sysFileInfo.cFileName[1] == 0);
			bValid &=	!(sysFileInfo.cFileName[0] == L'.' && sysFileInfo.cFileName[1] == L'.' || sysFileInfo.cFileName[2] == 0);

			if( bValid )
			{
				SYSTEMTIME	Time;
				zUInt		idx(_ResultOut.size());
				
				_ResultOut.resize(idx+1);
				FileInfo& fileInfo	= _ResultOut[idx];
				fileInfo.mFilename.Set(_zDirName.Data(), sysFileInfo.cFileName);
				if( FileTimeToSystemTime( &sysFileInfo.ftCreationTime, &Time ) )
				{
					fileInfo.mTimeCreated.Year		= Time.wYear;	
					fileInfo.mTimeCreated.Month		= Time.wMonth;
					fileInfo.mTimeCreated.Day		= Time.wDay;
					fileInfo.mTimeCreated.Hour		= Time.wHour;
					fileInfo.mTimeCreated.Second	= Time.wSecond;
				};
				if( FileTimeToSystemTime( &sysFileInfo.ftLastWriteTime, &Time ) )
				{					
					fileInfo.mTimeChanged.Year		= Time.wYear;	
					fileInfo.mTimeChanged.Month		= Time.wMonth;
					fileInfo.mTimeChanged.Day		= Time.wDay;
					fileInfo.mTimeChanged.Hour		= Time.wHour;
					fileInfo.mTimeChanged.Second	= Time.wSecond;
				};
				fileInfo.muFlags = uFileFlag;
			}

		}
		while( FindNextFile(hSearch, &sysFileInfo) );
		FindClose(hSearch);
	}
}

//=================================================================================================
//! @brief		Create a directory path 
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param	_zFilename		- Directory filename (will try to create entire path hierarchy)
//! @param _bLastItemIsFile	- If last item should be ignore because it's the name of a file
//! @return					- True if successful or path already existed
//=================================================================================================
bool ManagerFile_PC::CreateDir( const Filename& _Filename, bool _bLastItemIsFile )
{
	bool bOk(true);
	Filename nameDir;
	zArrayDyn<zArrayDyn<wchar_t>> aFolderNames;
	_Filename.SplitFolder(aFolderNames);
	zUInt count(aFolderNames.size());
	count -= _bLastItemIsFile ? 1 : 0;
	for(zUInt idx(0); idx<count && bOk; ++idx)
	{
		nameDir += aFolderNames[idx].Data();
		if( !CreateDirectory( nameDir.GetNameFull(), nullptr ) )
			bOk = (GetLastError() == ERROR_ALREADY_EXISTS);
	}
	return bOk;
}

bool ManagerFile_PC::OpenFile( const Filename& _Filename, FileInfo* _pOutFile )
{
	HANDLE hFile = CreateFile( _Filename.GetNameFull(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, nullptr );

	if( hFile == INVALID_HANDLE_VALUE )
	{
//		Ares_ErrorSystem::SetError( eAres_Error_UnableToOpenFileForRead );
	}
	else
	{
		DWORD uFileSize = ::GetFileSize( hFile, nullptr );
		if( uFileSize == INVALID_FILE_SIZE )
		{
			CloseHandle( hFile );
//			Ares_ErrorSystem::SetError( Ares_Error_CalculatingFileSize );
		}
		else
		{
			_pOutFile->mhFileHandle = hFile;
			_pOutFile->miFilePos = 0;
			_pOutFile->miFileSize = uFileSize;
			_pOutFile->mbValidHandle = true;
			return true;
		}
	}

	return false;
}

bool ManagerFile_PC::OpenFileForWrite( const Filename& _Filename, FileInfo* _pOutFile )
{
	HANDLE hFile = CreateFile( _Filename.GetNameFull(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, nullptr );

	if( hFile == INVALID_HANDLE_VALUE )
	{
		//Ares_ErrorSystem::SetError( eAres_Error_UnableToOpenFileForReadWrite );
	}
	else
	{
		DWORD uFileSize = ::GetFileSize( hFile, nullptr );
		if( uFileSize == INVALID_FILE_SIZE )
		{
			CloseHandle( hFile );
//			Ares_ErrorSystem::SetError( Ares_Error_CalculatingFileSize );
		}
		else
		{
			_pOutFile->mhFileHandle = hFile;
			_pOutFile->miFilePos = 0;
			_pOutFile->miFileSize = uFileSize;
			_pOutFile->mbValidHandle = true;
			return true;
		}
	}
	return false;
}

bool ManagerFile_PC::Delete( const Filename& _Filename, bool _bRecursive )
{	
	if( _Filename.IsValid() )
	{
		if (DeleteFile(_Filename.GetNameFull()) == FALSE)
		{
			zArrayDyn<FileInfo> fileAndDirList;
			Search(fileAndDirList, keFileFlag__Any, _Filename.GetNameFull(), L"*", true);
			for (zUInt idx(0), count(fileAndDirList.size()); idx < count; ++idx)
			{
				if (fileAndDirList[idx].IsFile())
					DeleteFile(fileAndDirList[idx].GetFilename().GetNameFull());
				else
					RemoveDirectory(fileAndDirList[idx].GetFilename().GetNameFull());
			}
			return RemoveDirectory(_Filename.GetNameFull()) == TRUE;
		}
	}
	return true;
}

bool ManagerFile_PC::Rename( const Filename& _OldFilename, const Filename& _NewFilename )
{
	return MoveFile( _OldFilename.GetNameFull(), _NewFilename.GetNameFull() ) == TRUE;
}

}
