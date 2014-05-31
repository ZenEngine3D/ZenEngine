#pragma once
#ifndef __LibZenBase_File_FileManager_h__
#define __LibZenBase_File_FileManager_h__

namespace zbFile
{
	//! @todo Clean: Use flag class
	enum enumFileFlag
	{
		keFileFlag_Dir			= 1<<0,
		keFileFlag_File			= 1<<1,			
		keFileFlag_ReadOnly		= 1<<2,
		keFileFlag_Writeable	= 1<<3,
		keFileFlag_Hidden		= 1<<4,
		keFileFlag__AnyDir		= keFileFlag_Dir|keFileFlag_Hidden,
		keFileFlag__AnyFile		= keFileFlag_File|keFileFlag_Hidden,
		keFileFlag__Any			= keFileFlag__AnyDir|keFileFlag__AnyFile,
	};

	class Filename
	{
	ZENClassDeclareNoParent(Filename);
	public:
										Filename();
										Filename(const wchar_t* _zFilename);
		void							Set(const wchar_t* _zFilename);
		void							Set(const wchar_t* _zPath, const wchar_t* _zFilename);
		const Filename&					operator=(const wchar_t* _zFilename);
		const Filename&					operator=(const Filename& _Copy);
		void							operator+=(const wchar_t* _zFilename);
		void							Clear();
		const wchar_t*					GetName()const;
		const wchar_t*					GetNameNoExt()const;
		const wchar_t*					GetNameFull()const;
		const wchar_t*					GetPathParent()const;
		const wchar_t*					GetPathFull()const;
		const wchar_t*					GetExt()const;
		void							SplitFolder(zArrayDynamic<zArrayStatic<wchar_t>>& _aFolder)const;
	protected:	
		ZENInline void					FindOffsets();
		mutable zArrayDynamic<wchar_t>	mzFilename;			//!< Contains filename string, +2 chars to handle no offset for some filename component (ext, parentdir, ...)
		zU16							muOffsetName;		//!< Offset to filename,extension
		zU16							muOffsetParentPath;	//!< Offset to parent folder, filename, extension
		zU16							muOffsetExt;		//!< Offset to extension
	};

}

#include ZENHeaderPlatform( zbFileInfo )

namespace zbFile
{
	class FileInfo : public ZENClassPlatform(FileInfo)
	{
	ZENClassDeclare(FileInfo, ZENClassPlatform(FileInfo));
	public:
		struct FileTime
		{
			zU16		Year;
			zU16		Month	: 4;
			zU16		Day		: 9;
			zU16		Hour	: 5;
			zU16		Second	: 6;
		};
								FileInfo();
		const Filename&			GetFilename()const	{return mFilename;}
		ZENInline bool			IsDir()const		{return (muFlags&keFileFlag_Dir)!=0; }
		ZENInline bool			IsFile()const		{return (muFlags&keFileFlag_File)!=0; }
		ZENInline zI64			GetFileSize()const	{return miFileSize;}
		ZENInline zI64			GetFilePos()const	{return miFilePos;}
		bool					IsEOF()const;

		void					Close();
		void					Seek(const zI64 uFilePos);
		void					Skip(const zI32 iNumOfBytes);
		void					Read(void* pxDest, const zU32 uNumBytes);
		void					Write(const void* pxSource, const zU32 uNumBytes);
		void					Flush();

		const FileInfo&	operator=(const FileInfo& _Copy);
	protected:
		Filename				mFilename;
		FileTime				mTimeCreated;
		FileTime				mTimeChanged;
		FileTime				mTimeAccess;
		zI64					miFileSize;
		zI64					miFilePos;
		zUInt					muFlags;
		bool					mbValidHandle;
		
		friend class			ManagerFile;
		friend class			ZENClassPlatform(ManagerFile);
	};

	class ManagerFile_Base : public zbType::Manager
	{
	ZENClassDeclare(ManagerFile_Base, zbType::Manager);
	public:
		ZENInline const zWString&	GetRoot()const {return mzRootPath;}
		ZENInline const zWString&	GetRootPackage()const {return mzRootPackage;}
	
	protected:
		void						InitPath(const zWString& _zRootPath);
		zWString					mzRootPath;
		zWString					mzRootPackage;
	};
}  

#include ZENHeaderPlatform( zbFileManager )

namespace zbFile
{
	class ManagerFile : public ZENClassPlatform(ManagerFile)
	{
	ZENClassDeclare(ManagerFile, ZENClassPlatform(ManagerFile));
	public:

		bool IsExistFile( const Filename& _filename )
		{
			FileInfo fileInfo;
			return GetFileInfo(_filename, fileInfo) && fileInfo.IsFile();
		}

		bool IsExistDir( const Filename& _filename )
		{
			FileInfo fileInfo;
			return GetFileInfo(_filename, fileInfo) && fileInfo.IsDir();
		}

		bool GetFileInfo( const Filename& _filename, FileInfo& _outFileInfo )
		{
			const FileInfo* pInfo;
			Search( keFileFlag__Any, L"", _filename.GetNameFull(), false);
			if( SearchNext( pInfo ) )
			{
				_outFileInfo = *pInfo;
				return true;
			}
			return false;
		}
	};
}

namespace zbMgr { extern zbFile::ManagerFile File; }

namespace zbFile
{
	class AWStream
	{
	public:
					AWStream( const Filename& szFileName, bool bOpenForWrite = false )
					{
						if( bOpenForWrite ) zbMgr::File.OpenFileForWrite( szFileName, &m_hFileHandle );
						else zbMgr::File.OpenFile( szFileName, &m_hFileHandle );
					}

					~AWStream( void )							{m_hFileHandle.Close();}

		void		Seek(const zI64 uFilePos)					{m_hFileHandle.Seek(uFilePos);}
		void		Skip(const zI32 iNumOfBytes)				{m_hFileHandle.Skip(iNumOfBytes);}
		zI64		GetFileSize(void)							{return m_hFileHandle.GetFileSize();}
		zI64		GetFilePosition(void)						{return m_hFileHandle.GetFilePos();}

		void		Flush(void)									{m_hFileHandle.Flush();}
		bool		IsEOF(void)									{return m_hFileHandle.IsEOF();}

		void		Read(void* pxDest, const zU32 uNumBytes)	{m_hFileHandle.Read(pxDest, uNumBytes);}
		AWStream&	operator>>(bool& bIn)						{Read(&bIn, sizeof(bool )); return *this;}
		AWStream&	operator>>(zU8& uIn)						{Read(&uIn, sizeof(zU8 )); return *this;}
		AWStream&	operator>>(zI8& iIn)						{Read(&iIn, sizeof(zI8 )); return *this;}
		AWStream&	operator>>(zU16&uIn)						{Read(&uIn, sizeof(zU16)); return *this;}
		AWStream&	operator>>(zI16&iIn)						{Read(&iIn, sizeof(zI16)); return *this;}
		AWStream&	operator>>(zU32&uIn)						{Read(&uIn, sizeof(zU32)); return *this;}
		AWStream&	operator>>(zI32&iIn)						{Read(&iIn, sizeof(zI32)); return *this;}
		AWStream&	operator>>(zU64&uIn)						{Read(&uIn, sizeof(zU64)); return *this;}
		AWStream&	operator>>(zI64&iIn )						{Read(&iIn, sizeof(zI64)); return *this;}
		AWStream&	operator>>(float&fIn )						{Read(&fIn, sizeof(float)); return *this;}
		AWStream&	operator>>(double&fIn )						{Read(&fIn, sizeof(double)); return *this;}

		void		Write(const void* pxSource, const zU32 uNumBytes) { m_hFileHandle.Write(pxSource, uNumBytes);}
		AWStream&	operator<<(const bool& bOut)				{Write(&bOut,sizeof(bool )); return *this;}
		AWStream&	operator<<(const zU8& uOut)					{Write(&uOut,sizeof(zU8 )); return *this;}
		AWStream&	operator<<(const zI8& iOut)					{Write(&iOut,sizeof(zI8 )); return *this;}
		AWStream&	operator<<(const zU16&uOut)					{Write(&uOut,sizeof(zU16)); return *this;}
		AWStream&	operator<<(const zI16&iOut)					{Write(&iOut,sizeof(zI16)); return *this;}
		AWStream&	operator<<(const zU32&uOut)					{Write(&uOut,sizeof(zU32)); return *this;}
		AWStream&	operator<<(const zI32&iOut)					{Write(&iOut,sizeof(zI32)); return *this;}
		AWStream&	operator<<(const zU64&uOut)					{Write(&uOut,sizeof(zU64)); return *this;}
		AWStream&	operator<<(const zI64&iOut)					{Write(&iOut,sizeof(zI64)); return *this;}
		AWStream&	operator<<(const float&fOut)				{Write(&fOut,sizeof(float)); return *this;}
		AWStream&	operator<<(const double&fOut)				{Write(&fOut,sizeof(double)); return *this;}

	private:
		FileInfo	m_hFileHandle;

					AWStream( AWStream& xOther );
					AWStream& operator= ( const AWStream& xOther );
	};
}

#endif
