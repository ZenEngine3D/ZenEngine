#pragma once
#ifndef __LibZenBase_File_FileManager_h__
#define __LibZenBase_File_FileManager_h__

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
		void							SplitFolder(zenArrayDynamic<zenArrayStatic<wchar_t>>& _aFolder)const;
	protected:	
		inline void						FindOffsets();
		mutable zenArrayDynamic<wchar_t>	mzFilename;			//!< Contains filename string, +2characters to handle no offset for some filename component (ext, parentdir, ...)
		zenU16								muOffsetName;		//!< Offset to filename,extension
		zenU16								muOffsetParentPath;	//!< Offset to parent folder, filename, extension
		zenU16								muOffsetExt;		//!< Offset to extension
	};

	class File
	{
		ZENClassDeclareNoParent(File);
	public:
		Filename mzFilename;
		Filename mzFilenameStage;
	};
}

#include ZENHeaderPlatform( CFileInfo )

namespace CFil
{
	class FileInfo : public ZENClassPlatform(FileInfo)
	{
	ZENClassDeclare(FileInfo, ZENClassPlatform(FileInfo));
	public:
		struct FileTime
		{
			zenU16		Year;
			zenU16		Month	: 4;
			zenU16		Day		: 9;
			zenU16		Hour	: 5;
			zenU16		Second	: 6;
		};
								FileInfo();
		const Filename&			GetFilename()const	{return mFilename;}
		inline bool				IsDir()const		{return (muFlags&keFileFlag_Dir)!=0; }
		inline bool				IsFile()const		{return (muFlags&keFileFlag_File)!=0; }
		inline zenS64			GetFileSize()const	{return miFileSize;}
		inline zenS64			GetFilePos()const	{return miFilePos;}
		bool					IsEOF()const;

		void					Close();
		void					Seek(const zenS64 uFilePos);
		void					Skip(const zenS32 iNumOfBytes);
		void					Read(void* pxDest, const zenU32 uNumBytes);
		void					Write(const void* pxSource, const zenU32 uNumBytes);
		void					Flush();

		const FileInfo&	operator=(const FileInfo& _Copy);
	protected:
		Filename				mFilename;
		FileTime				mTimeCreated;
		FileTime				mTimeChanged;
		FileTime				mTimeAccess;
		zenS64					miFileSize;
		zenS64					miFilePos;
		zenUInt					muFlags;
		bool					mbValidHandle;
		
		friend class			ManagerFile;
		friend class			ZENClassPlatform(ManagerFile);
	};

	class ManagerFile_Base : public zbType::Manager
	{
	ZENClassDeclare(ManagerFile_Base, zbType::Manager);
	};
}  

#include ZENHeaderPlatform( CFileManager )

namespace CFil
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

		void		Seek(const zenS64 uFilePos)					{m_hFileHandle.Seek(uFilePos);}
		void		Skip(const zenS32 iNumOfBytes)				{m_hFileHandle.Skip(iNumOfBytes);}
		zenS64		GetFileSize(void)							{return m_hFileHandle.GetFileSize();}
		zenS64		GetFilePosition(void)						{return m_hFileHandle.GetFilePos();}

		void		Flush(void)									{m_hFileHandle.Flush();}
		bool		IsEOF(void)									{return m_hFileHandle.IsEOF();}

		void		Read(void* pxDest, const zenU32 uNumBytes)	{m_hFileHandle.Read(pxDest, uNumBytes);}
		AWStream&	operator>>(bool& bIn)						{Read(&bIn, sizeof(bool )); return *this;}
		AWStream&	operator>>(zenU8& uIn)						{Read(&uIn, sizeof(zenU8 )); return *this;}
		AWStream&	operator>>(zenS8& iIn)						{Read(&iIn, sizeof(zenS8 )); return *this;}
		AWStream&	operator>>(zenU16&uIn)						{Read(&uIn, sizeof(zenU16)); return *this;}
		AWStream&	operator>>(zenS16&iIn)						{Read(&iIn, sizeof(zenS16)); return *this;}
		AWStream&	operator>>(zenU32&uIn)						{Read(&uIn, sizeof(zenU32)); return *this;}
		AWStream&	operator>>(zenS32&iIn)						{Read(&iIn, sizeof(zenS32)); return *this;}
		AWStream&	operator>>(zenU64&uIn)						{Read(&uIn, sizeof(zenU64)); return *this;}
		AWStream&	operator>>(zenS64&iIn )						{Read(&iIn, sizeof(zenS64)); return *this;}
		AWStream&	operator>>(zenF32&fIn )						{Read(&fIn, sizeof(zenF32)); return *this;}
		AWStream&	operator>>(zenF64&fIn )						{Read(&fIn, sizeof(zenF64)); return *this;}

		void		Write(const void* pxSource, const zenU32 uNumBytes) { m_hFileHandle.Write(pxSource, uNumBytes);}
		AWStream&	operator<<(const bool& bOut)				{Write(&bOut,sizeof(bool )); return *this;}
		AWStream&	operator<<(const zenU8& uOut)				{Write(&uOut,sizeof(zenU8 )); return *this;}
		AWStream&	operator<<(const zenS8& iOut)				{Write(&iOut,sizeof(zenS8 )); return *this;}
		AWStream&	operator<<(const zenU16&uOut)				{Write(&uOut,sizeof(zenU16)); return *this;}
		AWStream&	operator<<(const zenS16&iOut)				{Write(&iOut,sizeof(zenS16)); return *this;}
		AWStream&	operator<<(const zenU32&uOut)				{Write(&uOut,sizeof(zenU32)); return *this;}
		AWStream&	operator<<(const zenS32&iOut)				{Write(&iOut,sizeof(zenS32)); return *this;}
		AWStream&	operator<<(const zenU64&uOut)				{Write(&uOut,sizeof(zenU64)); return *this;}
		AWStream&	operator<<(const zenS64&iOut)				{Write(&iOut,sizeof(zenS64)); return *this;}
		AWStream&	operator<<(const zenF32&fOut)				{Write(&fOut,sizeof(zenF32)); return *this;}
		AWStream&	operator<<(const zenF64&fOut)				{Write(&fOut,sizeof(zenF64)); return *this;}

	private:
		FileInfo	m_hFileHandle;

					AWStream( AWStream& xOther );
					AWStream& operator= ( const AWStream& xOther );
	};
}

#endif
