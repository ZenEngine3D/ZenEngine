#pragma once
#ifndef __LibCore_File_FileInfo_PC_h__
#define __LibCore_File_FileInfo_PC_h__

namespace CFil
{
	class FileInfo_PC
	{
		AWClassDeclareNoParent(FileInfo_PC);
	public:
							FileInfo_PC();

		void				Close(zenS64& iFilePos, zenS64& iFileSize, bool& bValidHandle);
		void				Seek(zenS64& iNewFilePos, const zenS64 iFilePos);
		void				Skip(zenS64& iFilePos, const zenS32 iNumOfBytes);
		void				Read(zenS64& iFilePos, void* pxDest, const zenU32 uNumBytes);
		void				Write(zenS64& iFilePos, zenS64& iFileSize, const void* pxSource, const zenU32 uNumBytes);
		void				Flush();
	protected:
		

		AWForceInline void	Copy(const FileInfo_PC &_Copy)	{mhFileHandle = _Copy.mhFileHandle;}
		HANDLE				mhFileHandle;
	};

}  

#endif
