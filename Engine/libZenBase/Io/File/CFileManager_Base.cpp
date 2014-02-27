#include "libZenBase.h"

namespace CMgr { CFil::ManagerFile File; }

namespace CFil
{

	
FileInfo::FileInfo()
	: ZENClassPlatform(FileInfo)()
	, miFileSize(0)
	, miFilePos(0)
	, muFlags(0)
	, mbValidHandle(false)
{
}

const FileInfo&	FileInfo::operator=(const FileInfo& _Copy)
{
	Copy(_Copy);
	mFilename		= _Copy.mFilename;
	mTimeCreated	= _Copy.mTimeCreated;
	mTimeChanged	= _Copy.mTimeChanged;
	mTimeAccess		= _Copy.mTimeAccess;
	miFileSize		= _Copy.miFileSize;
	miFilePos		= _Copy.miFilePos;
	muFlags			= _Copy.muFlags;
	mbValidHandle	= _Copy.mbValidHandle;
	return *this;
}

bool FileInfo::IsEOF()const
{
	bool bEOF = true;
	if( mbValidHandle )
	{
		bEOF = ( miFilePos >= miFileSize );
	}
	return bEOF;
}

void FileInfo::Close()
{
	Super::Close(miFilePos, miFileSize, mbValidHandle);
}

void FileInfo::Seek(const zenS64 iFilePos)
{
	Super::Seek(miFilePos, iFilePos);
}

void FileInfo::Skip(const zenS32 iNumOfBytes)
{
	Super::Skip(miFilePos, iNumOfBytes);
}

void FileInfo::Read(void* pxDest, const zenU32 uNumBytes)
{
	Super::Read(miFilePos, pxDest, uNumBytes);
}

void FileInfo::Write(const void* pxSource, const zenU32 uNumBytes)
{
	Super::Write(miFilePos, miFileSize, pxSource, uNumBytes);
}

void FileInfo::Flush()
{
	Super::Flush();
}

Filename::Filename()
: muOffsetName(0)
, muOffsetParentPath(2)
, muOffsetExt(2)
{
	mzFilename.SetCount(4);
	mzFilename[0]	= L'.';
	mzFilename[1]	= L'';
	mzFilename[2]	= L'';
	mzFilename[3]	= L'';
}

Filename::Filename(const wchar_t* _zFilename)
{
	Set(_zFilename);
}

const Filename& Filename::operator=(const wchar_t* _zFilename)
{
	Set(_zFilename);
	return *this;
}

//=================================================================================================
//! @brief		Copy content of another filename
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param	_Copy	- Source filename to copy from
//=================================================================================================
const Filename&	Filename::operator=(const Filename& _Copy)
{
	mzFilename			= _Copy.mzFilename;
	muOffsetName		= _Copy.muOffsetName;
	muOffsetParentPath	= _Copy.muOffsetParentPath;
	muOffsetExt			= _Copy.muOffsetExt;
	return *this;
}

//=================================================================================================
//! @brief		Remove all filename elements
//! @details	
//-------------------------------------------------------------------------------------------------
//! @return
//=================================================================================================
void Filename::Clear()
{
	muOffsetName		= 0;
	muOffsetParentPath	= 2;
	muOffsetExt			= 2;
	mzFilename.SetCount(4);
	mzFilename[0]		= L'.';
	mzFilename[1]		= L'';
	mzFilename[2]		= L'';
	mzFilename[3]		= L'';
}

//=================================================================================================
//! @brief		Find each filename element offset
//! @details	
//-------------------------------------------------------------------------------------------------
//! @return
//=================================================================================================
void Filename::FindOffsets()
{
	zenUInt end			= mzFilename.Count()-1;
	int pos				= end;
	muOffsetName		= end;
	muOffsetParentPath	= end;
	muOffsetExt			= end;

	while(--pos >= 0 && muOffsetParentPath == end)
	{
		if( mzFilename[pos] == L'.' && muOffsetExt==end)
		{
			muOffsetExt = pos+1;
		}
		else if( mzFilename[pos] == L'\\' || mzFilename[pos] == L'/' )
		{
			if( muOffsetName == end )
				muOffsetName = pos+1;
			else
				muOffsetParentPath = pos+1;
		}
		else if( pos==0 && muOffsetName != end )
		{
			muOffsetParentPath = pos;
		}
	}
}

//=================================================================================================
//! @brief		Assign the filename
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param	_zFilename	- Name of filename
//=================================================================================================
void Filename::Set(const wchar_t* _zFilename)
{
	zenUInt len(static_cast<zenUInt>(wcslen(_zFilename)));
	mzFilename.SetCount(len+3);	//add some padding handling of null name offsets
	mzFilename[len+0] = L'';
	mzFilename[len+1] = L'';
	mzFilename[len+2] = L'';
	wcsncpy_s( mzFilename.First(), len+1, _zFilename, _TRUNCATE );
	FindOffsets();
}

//=================================================================================================
//! @brief		Assign the filename, as a concatenation of two path element
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param	_zFilename	- Name of filename
//=================================================================================================
void Filename::Set(const wchar_t* _zPath, const wchar_t* _zFilename)
{
	zenUInt len1(static_cast<zenUInt>(wcslen(_zPath)));
	zenUInt len2(static_cast<zenUInt>(wcslen(_zFilename)));
	bool bNoSeparator	=	(_zPath[len1-1] != L'\\')	&& (_zPath[len1-1] != L'/') && 
							(_zFilename[0] != L'\\')	&& (_zFilename[0] != L'/' );
	len1				+= bNoSeparator ? 1 : 0;

	mzFilename.SetCount(len1+len2+3); //add some padding for possible '/' added and handling of null name offsets
	mzFilename[len1+len2+0] = L'';
	mzFilename[len1+len2+1] = L'';
	mzFilename[len1+len2+2] = L'';
	wcsncpy_s( &mzFilename[0], len1+1, _zPath, _TRUNCATE );		
	wcsncpy_s( &mzFilename[len1], len2+1, _zFilename, _TRUNCATE );
	if( bNoSeparator )
		mzFilename[len1-1] = L'/';

	FindOffsets();
}

//=================================================================================================
//! @brief		Add a new part to the filename
//! @details	
//-------------------------------------------------------------------------------------------------
//! @param	_zFilename	- Filename to add to current name
//=================================================================================================
void Filename::operator+=(const wchar_t* _zFilename)
{
	zenUInt len1(mzFilename.Count()-3);
	zenUInt len2(static_cast<zenUInt>(wcslen(_zFilename)));
	bool bNoSeparator	=	(mzFilename[len1-1] != L'\\')	&& (mzFilename[len1-1] != L'/') && 
							(_zFilename[0] != L'\\')		&& (_zFilename[0] != L'/' );
	len1				+= bNoSeparator ? 1 : 0;
	mzFilename.SetCount(len1+len2+3); //add some padding for possible '/' added and handling of null name offsets
	mzFilename[len1+len2+0] = L'';
	mzFilename[len1+len2+1] = L'';
	mzFilename[len1+len2+2] = L'';
	wcsncpy_s( &mzFilename[len1], len2+1, _zFilename, _TRUNCATE );	
	if( bNoSeparator )
		mzFilename[len1-1] = L'/';
	FindOffsets();
}

//=================================================================================================
//! @brief		Return the name part of the filename with extension
//! @details	
//! @note		Do not save the pointer return for later usage, it will become invalid as soon as
//!				another 'GetXXX' method is called
//-------------------------------------------------------------------------------------------------
//! @return		Name of filename last element
//=================================================================================================
const wchar_t* Filename::GetName()const
{
	mzFilename[muOffsetExt-1] = L'.';
	return &mzFilename[muOffsetName];
}

//=================================================================================================
//! @brief		Return the name part of the filename without extension
//! @details	
//! @note		Do not save the pointer return for later usage, it will become invalid as soon as
//!				another 'GetXXX' method is called
//-------------------------------------------------------------------------------------------------
//! @return		Name of filename last element
//=================================================================================================
const wchar_t* Filename::GetNameNoExt()const
{
	mzFilename[muOffsetExt-1] = L'';
	return &mzFilename[muOffsetName];
}

//=================================================================================================
//! @brief		Return the full filename, with path and extension
//! @details	
//! @note		Do not save the pointer return for later usage, it will become invalid as soon as
//!				another 'GetXXX' method is called
//-------------------------------------------------------------------------------------------------
//! @return		Name of filename last element
//=================================================================================================
const wchar_t* Filename::GetNameFull()const
{
	mzFilename[muOffsetName-1] = L'/';
	mzFilename[muOffsetExt-1] = L'.';
	return &mzFilename[0];
}

//=================================================================================================
//! @brief		Return the name part of the filename without extension
//! @details	
//! @note		Do not save the pointer return for later usage, it will become invalid as soon as
//!				another 'GetXXX' method is called
//-------------------------------------------------------------------------------------------------
//! @return		Name of filename last element
//=================================================================================================
const wchar_t* Filename::GetPathParent()const
{
	mzFilename[muOffsetName-1] = L'';
	mzFilename[muOffsetExt-1] = L'.';
	return &mzFilename[muOffsetParentPath];
}

//=================================================================================================
//! @brief		Return the entire path to element, without element name/extension
//! @details	
//! @note		Do not save the pointer return for later usage, it will become invalid as soon as
//!				another 'GetXXX' method is called
//-------------------------------------------------------------------------------------------------
//! @return		Name of filename last element
//=================================================================================================
const wchar_t* Filename::GetPathFull()const
{
	mzFilename[muOffsetName-1] = L'';
	return &mzFilename[0];
}

//=================================================================================================
//! @brief		Return the extension part of the filename
//! @details	
//! @note		Do not save the pointer return for later usage, it will become invalid as soon as
//!				another 'GetXXX' method is called
//-------------------------------------------------------------------------------------------------
//! @return		Name of filename last element
//=================================================================================================
const wchar_t* Filename::GetExt()const
{
	return &mzFilename[muOffsetExt];
}

//=================================================================================================
//! @brief		Return each path element, and store in array form
//! @details	
//-------------------------------------------------------------------------------------------------
//! @Param		_aFolderOut -	Array each folder string will be stored in
//=================================================================================================
void Filename::SplitFolder(zenArrayDynamic<zenArrayStatic<wchar_t>>& _aFolderOut)const
{
	zenUInt aOffset[64];
	zenUInt uOffetCount(1);
	zenUInt uPos(0), uCount(mzFilename.Count()-3);
	
	//Make sure filename is intact (no \0 inserted)
	GetNameFull(); 

	// Find all path separators	
	while( ++uPos < uCount)
	{
		ZENAssert(uOffetCount < ZENArrayCount(aOffset) )
		if( mzFilename[uPos] == L'\\' || mzFilename[uPos] == L'/')
			aOffset[uOffetCount++] = uPos+1;
	}
	aOffset[0]				= 0;
	aOffset[uOffetCount]	= uPos+1;

	// Copy each path string to array
	uPos = 0;
	_aFolderOut.SetCount( uOffetCount );
	while( uPos < uOffetCount )
	{
		uCount = aOffset[uPos+1] - aOffset[uPos];
		_aFolderOut[uPos].Copy( &mzFilename[aOffset[uPos]], uCount );
		_aFolderOut[uPos][uCount-1] = L'';
		++uPos;
	}
}

}
