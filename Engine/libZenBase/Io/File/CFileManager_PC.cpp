#include "libZenBase.h"

namespace CFil
{

bool ManagerFile_PC::Load()
{
	bool bValid(false);
	wchar_t zWorkingDir[512];
	GetCurrentDirectory( ZENArrayCount(zWorkingDir), zWorkingDir );
	size_t uPosEnd = wcslen(zWorkingDir);
	while( !bValid && zWorkingDir[0] != L'' )
	{
		// Look for 'Content' directory in working path		
		_snwprintf_s( &zWorkingDir[uPosEnd], ZENArrayCount(zWorkingDir)-uPosEnd, _TRUNCATE, L"/Content");
		WIN32_FIND_DATA	FileInfo;
		HANDLE hSearch	= FindFirstFile(zWorkingDir, &FileInfo);		
		if( hSearch != INVALID_HANDLE_VALUE )
		{
			FindClose(hSearch);
			bValid = (FileInfo.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) != 0;
			if( bValid )
			{
				mzRoot.SetCount( static_cast<zUInt>(wcslen(zWorkingDir) + 1) );
				_snwprintf_s( mzRoot.First(), mzRoot.Count(), _TRUNCATE, zWorkingDir);
				SetCurrentDirectory( mzRoot.First() );
			}
		}

		// If none found, look in parent directory
		--uPosEnd;
		while( !bValid && (uPosEnd > 0) && (zWorkingDir[uPosEnd]!= L'\\' && zWorkingDir[uPosEnd]!= L'/') )
			--uPosEnd;
	}
	

#if 0
 	const FileInfo*	pFile(NULL);
 	Search( keFileFlag_File, L"", L"*.xml", true );
 	while( SearchNext(pFile) )
 	{
 		wprintf(L"NameFull  : %s\n", pFile->GetFilename().GetNameFull() );
		wprintf(L"Name      : %s\n", pFile->GetFilename().GetName() );
		wprintf(L"NameNoExt : %s\n", pFile->GetFilename().GetNameNoExt() );
		wprintf(L"PathFull  : %s\n", pFile->GetFilename().GetPathFull() );
		wprintf(L"PathParent: %s\n\n", pFile->GetFilename().GetPathParent() );		
 	}

	Filename filenameTest[4];
	filenameTest[0].Set(L"Packages/SubDir/",L"testfile.xml");
	
	filenameTest[1].Set(L"Packages",L"testfile.xml");
	
	filenameTest[2].Set(L"Packages/");
	filenameTest[2] += L"testFile.xml";

	filenameTest[3].Set(L"Packages");
	filenameTest[3] += L"testFile.xml";
	for(zUInt idx(0); idx<ZENArrayCount(filenameTest); ++idx)
	{
		wprintf(L"FILENAME %i\n", idx );
		wprintf(L"NameFull  : %s\n", filenameTest[idx].GetNameFull() );
		wprintf(L"Name      : %s\n", filenameTest[idx].GetName() );
		wprintf(L"NameNoExt : %s\n", filenameTest[idx].GetNameNoExt() );
		wprintf(L"PathFull  : %s\n", filenameTest[idx].GetPathFull() );
		wprintf(L"PathParent: %s\n\n", filenameTest[idx].GetPathParent() );		
	}

	zArrayDynamic<zArrayStatic<wchar_t>> aFolderName;
	filenameTest[0].SplitFolder(aFolderName);
	wprintf(L"SPLIT Filename: " );
	for(zUInt idx(0); idx<aFolderName.Count(); ++idx)
		wprintf(L"%s ", aFolderName[idx].First() );

	CreateDir( L"Packages" );
	CreateDir( L"Packages/Test" );
#endif

	return bValid;
}

bool ManagerFile_PC::Unload()
{
	return true;
}

bool ManagerFile_PC::SearchNext( const FileInfo*& _pOutFile )
{	
	if( muSearchIndex < maSearchResult.Count() )
	{
		_pOutFile = &maSearchResult[muSearchIndex++];
		return true;
	}
	_pOutFile = NULL;		
	return false;
}

bool ManagerFile_PC::Search(zUInt _uWantedFlag, const wchar_t* _zDirName, const wchar_t* _zFilePatern, bool bRecursive)
{
	ZENAssert(_zDirName );
	ZENAssert(_zFilePatern );
	ZENAssertMsg(_zDirName[wcslen(_zDirName)-1] != L'\\' && _zDirName[wcslen(_zDirName)-1] != L'/', "Directory name shouldn't end with '\\' or '/'");
	ZENAssertMsg(_zFilePatern[0] != L'\\' && _zFilePatern[0] != L'/', "Filename patern shouldn't start with '\\' or '/'");
		
	muSearchIndex = 0;
	maSearchResult.Clear();
	maSearchResult.Reserve(32);
	
	_zDirName	= (_zDirName[0] == L'') ? L"." : _zDirName;
	zUInt len	= static_cast<zUInt>(wcslen(_zDirName));
	zArrayDynamic<wchar_t> zPath;
	zPath.Reserve(1024);
	zPath.Copy(_zDirName, len + 1);
	zPath.SetCount(static_cast<zUInt>(zPath.Count() + 1));		
	zPath[zPath.Count()-2] = L'/';
	zPath[zPath.Count()-1] = L'';
	
	LoadDirectory(_uWantedFlag, zPath, _zFilePatern, static_cast<zUInt>(wcslen(_zFilePatern)), bRecursive);
	return maSearchResult.Count() > 0;
}

void ManagerFile_PC::LoadDirectory( zUInt _uWantedFlag, zArrayDynamic<wchar_t>& _zDirName, const wchar_t* _zFilePatern, const zUInt _uFilePaternLen, bool bRecursive)
{
	WIN32_FIND_DATA	sysFileInfo;
	zUInt uPathSizeInitial(_zDirName.Count());	

	//-------------------------------------------------------------------------
	// Look inside each directory when resursice is enabled
	//-------------------------------------------------------------------------
	if( bRecursive )
	{
		// Append search pattern	
		_zDirName.SetCount( uPathSizeInitial + 1 );	
		_zDirName[uPathSizeInitial-1]	= L'*';	
		_zDirName[uPathSizeInitial]		= L'';
		HANDLE hSearch					= FindFirstFile(_zDirName.First(), &sysFileInfo);
		// Parse all children directories
		if(hSearch != INVALID_HANDLE_VALUE)
		{
			do
			{
				if( (sysFileInfo.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) != 0 && 
					!(sysFileInfo.cFileName[0] == L'.' && sysFileInfo.cFileName[1] == L'') &&
					!(sysFileInfo.cFileName[0] == L'.' && sysFileInfo.cFileName[1] == L'.' && sysFileInfo.cFileName[2] == L'') )
				{				
					zUInt len = static_cast<zUInt>(wcslen(sysFileInfo.cFileName));
					_zDirName.SetCount( static_cast<zUInt>(uPathSizeInitial + len + 1));
					wcsncpy_s( &_zDirName[uPathSizeInitial-1], len+1, sysFileInfo.cFileName, _TRUNCATE );
					_zDirName[uPathSizeInitial+len-1]	= L'/';
					_zDirName[uPathSizeInitial+len]		= L'';
					LoadDirectory(_uWantedFlag, _zDirName, _zFilePatern, _uFilePaternLen, bRecursive);
				}

			}
			while( FindNextFile(hSearch, &sysFileInfo) );
			FindClose(hSearch);
		}
	}

	//-------------------------------------------------------------------------
	// Look for all files/dir matching patern in current directory
	//-------------------------------------------------------------------------
	_zDirName.SetCount( uPathSizeInitial + _uFilePaternLen );	//Append search pattern	
	wcsncpy_s( &_zDirName[uPathSizeInitial-1], _uFilePaternLen+1, _zFilePatern, _TRUNCATE );	
	HANDLE hSearch = FindFirstFile(_zDirName.First(), &sysFileInfo);	
	_zDirName[uPathSizeInitial-1] = L'';
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
			bValid &=	(_uWantedFlag&keFileFlag_Hidden)==0		|| (uFileFlag&keFileFlag_Hidden)!=0;

			if( bValid )
			{
				SYSTEMTIME	Time;
				zUInt		len(static_cast<zUInt>(wcslen(sysFileInfo.cFileName)));
				zUInt		idx(maSearchResult.Count());
				
				maSearchResult.SetCount(idx+1);
				FileInfo& fileInfo	= maSearchResult[idx];
				fileInfo.mFilename.Set(_zDirName.First(), sysFileInfo.cFileName);
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
//! @param	_zFilename	- Directory filename (will try to create entire path hierarchy)
//! @return				- True if sucessfull or path already existed
//=================================================================================================
bool ManagerFile_PC::CreateDir( const Filename& _Filename )
{
	bool bOk(true);
	Filename nameDir;
	zArrayDynamic<zArrayStatic<wchar_t>> aFolderNames;
	_Filename.SplitFolder(aFolderNames);
	for(zUInt idx(0), count(aFolderNames.Count()); idx<count && bOk; ++idx)
	{
		nameDir += aFolderNames[idx].First();
		if( !CreateDirectory( nameDir.GetNameFull(), NULL ) )
			bOk = (GetLastError() == ERROR_ALREADY_EXISTS);
	}
	return bOk;
}

bool ManagerFile_PC::OpenFile( const Filename& _Filename, FileInfo* _pOutFile )
{
	HANDLE hFile = CreateFile( _Filename.GetNameFull(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL );

	if( hFile == INVALID_HANDLE_VALUE )
	{
//		Ares_ErrorSystem::SetError( eAres_Error_UnableToOpenFileForRead );
	}
	else
	{
		DWORD uFileSize = ::GetFileSize( hFile, NULL );
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
	HANDLE hFile = CreateFile( _Filename.GetNameFull(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL );

	if( hFile == INVALID_HANDLE_VALUE )
	{
		//Ares_ErrorSystem::SetError( eAres_Error_UnableToOpenFileForReadWrite );
	}
	else
	{
		DWORD uFileSize = ::GetFileSize( hFile, NULL );
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

}
