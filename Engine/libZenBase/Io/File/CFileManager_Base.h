#pragma once
#ifndef __LibCore_File_FileManager_h__
#define __LibCore_File_FileManager_h__

namespace CFil
{

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
	AWClassDeclareNoParent(Filename);
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
		void							SplitFolder(awArrayDynamic<awArrayStatic<wchar_t>>& _aFolder)const;
	protected:	
		inline void						FindOffsets();
		mutable awArrayDynamic<wchar_t>	mzFilename;			//!< Contains filename string, +2characters to handle no offset for some filename component (ext, parentdir, ...)
		awU16								muOffsetName;		//!< Offset to filename,extension
		awU16								muOffsetParentPath;	//!< Offset to parent folder, filename, extension
		awU16								muOffsetExt;		//!< Offset to extension
	};

	class File
	{
		AWClassDeclareNoParent(File);
	public:
		Filename mzFilename;
		Filename mzFilenameStage;
	};
}

#include AWHeaderPlatform( CFileInfo )

namespace CFil
{
	class FileInfo : public AWClassPlatform(FileInfo)
	{
	AWClassDeclare(FileInfo, AWClassPlatform(FileInfo));
	public:
		struct FileTime
		{
			awU16		Year;
			awU16		Month	: 4;
			awU16		Day		: 9;
			awU16		Hour	: 5;
			awU16		Second	: 6;
		};
								FileInfo();
		const Filename&			GetFilename()const	{return mFilename;}
		inline bool				IsDir()const		{return (muFlags&keFileFlag_Dir)!=0; }
		inline bool				IsFile()const		{return (muFlags&keFileFlag_File)!=0; }
		inline awS64			GetFileSize()const	{return miFileSize;}
		inline awS64			GetFilePos()const	{return miFilePos;}
		bool					IsEOF()const;

		void					Close();
		void					Seek(const awS64 uFilePos);
		void					Skip(const awS32 iNumOfBytes);
		void					Read(void* pxDest, const awU32 uNumBytes);
		void					Write(const void* pxSource, const awU32 uNumBytes);
		void					Flush();

		const FileInfo&	operator=(const FileInfo& _Copy);
	protected:
		Filename				mFilename;
		FileTime				mTimeCreated;
		FileTime				mTimeChanged;
		FileTime				mTimeAccess;
		awS64					miFileSize;
		awS64					miFilePos;
		awUInt					muFlags;
		bool					mbValidHandle;
		
		friend class			ManagerFile;
		friend class			AWClassPlatform(ManagerFile);
	};

	class ManagerFile_Base : public zbType::Manager
	{
	AWClassDeclare(ManagerFile_Base, zbType::Manager);
	};
}  

#include AWHeaderPlatform( CFileManager )

namespace CFil
{
	class ManagerFile : public AWClassPlatform(ManagerFile)
	{
	AWClassDeclare(ManagerFile, AWClassPlatform(ManagerFile));
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

	protected:
	};

}

namespace CMgr { extern CFil::ManagerFile File; }

namespace CFil
{
	class AWStream
	{
	public:
					AWStream( const Filename& szFileName, bool bOpenForWrite = false )
					{
						if( bOpenForWrite ) CMgr::File.OpenFileForWrite( szFileName, &m_hFileHandle );
						else CMgr::File.OpenFile( szFileName, &m_hFileHandle );
					}

					~AWStream( void )							{m_hFileHandle.Close();}

		void		Seek(const awS64 uFilePos)					{m_hFileHandle.Seek(uFilePos);}
		void		Skip(const awS32 iNumOfBytes)				{m_hFileHandle.Skip(iNumOfBytes);}
		awS64		GetFileSize(void)							{return m_hFileHandle.GetFileSize();}
		awS64		GetFilePosition(void)						{return m_hFileHandle.GetFilePos();}

		void		Flush(void)									{m_hFileHandle.Flush();}
		bool		IsEOF(void)									{return m_hFileHandle.IsEOF();}

		void		Read(void* pxDest, const awU32 uNumBytes)	{m_hFileHandle.Read(pxDest, uNumBytes);}
		AWStream&	operator>>(bool& bIn)						{Read(&bIn, sizeof(bool )); return *this;}
		AWStream&	operator>>(awU8& uIn)						{Read(&uIn, sizeof(awU8 )); return *this;}
		AWStream&	operator>>(awS8& iIn)						{Read(&iIn, sizeof(awS8 )); return *this;}
		AWStream&	operator>>(awU16&uIn)						{Read(&uIn, sizeof(awU16)); return *this;}
		AWStream&	operator>>(awS16&iIn)						{Read(&iIn, sizeof(awS16)); return *this;}
		AWStream&	operator>>(awU32&uIn)						{Read(&uIn, sizeof(awU32)); return *this;}
		AWStream&	operator>>(awS32&iIn)						{Read(&iIn, sizeof(awS32)); return *this;}
		AWStream&	operator>>(awU64&uIn)						{Read(&uIn, sizeof(awU64)); return *this;}
		AWStream&	operator>>(awS64&iIn )						{Read(&iIn, sizeof(awS64)); return *this;}
		AWStream&	operator>>(awF32&fIn )						{Read(&fIn, sizeof(awF32)); return *this;}
		AWStream&	operator>>(awF64&fIn )						{Read(&fIn, sizeof(awF64)); return *this;}

		void		Write(const void* pxSource, const awU32 uNumBytes) { m_hFileHandle.Write(pxSource, uNumBytes);}
		AWStream&	operator<<(const bool& bOut)				{Write(&bOut,sizeof(bool )); return *this;}
		AWStream&	operator<<(const awU8& uOut)				{Write(&uOut,sizeof(awU8 )); return *this;}
		AWStream&	operator<<(const awS8& iOut)				{Write(&iOut,sizeof(awS8 )); return *this;}
		AWStream&	operator<<(const awU16&uOut)				{Write(&uOut,sizeof(awU16)); return *this;}
		AWStream&	operator<<(const awS16&iOut)				{Write(&iOut,sizeof(awS16)); return *this;}
		AWStream&	operator<<(const awU32&uOut)				{Write(&uOut,sizeof(awU32)); return *this;}
		AWStream&	operator<<(const awS32&iOut)				{Write(&iOut,sizeof(awS32)); return *this;}
		AWStream&	operator<<(const awU64&uOut)				{Write(&uOut,sizeof(awU64)); return *this;}
		AWStream&	operator<<(const awS64&iOut)				{Write(&iOut,sizeof(awS64)); return *this;}
		AWStream&	operator<<(const awF32&fOut)				{Write(&fOut,sizeof(awF32)); return *this;}
		AWStream&	operator<<(const awF64&fOut)				{Write(&fOut,sizeof(awF64)); return *this;}

	private:
		FileInfo	m_hFileHandle;

					AWStream( AWStream& xOther );
					AWStream& operator= ( const AWStream& xOther );
	};
}

#endif
