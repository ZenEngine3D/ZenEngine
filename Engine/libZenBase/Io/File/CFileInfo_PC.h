#pragma once
#ifndef __LibZenBase_File_FileInfo_PC_h__
#define __LibZenBase_File_FileInfo_PC_h__

namespace CFil
{
	class FileInfo_PC
	{
		ZENClassDeclareNoParent(FileInfo_PC);
	public:
							FileInfo_PC();

		void				Close(zS64& iFilePos, zS64& iFileSize, bool& bValidHandle);
		void				Seek(zS64& iNewFilePos, const zS64 iFilePos);
		void				Skip(zS64& iFilePos, const zS32 iNumOfBytes);
		void				Read(zS64& iFilePos, void* pxDest, const zU32 uNumBytes);
		void				Write(zS64& iFilePos, zS64& iFileSize, const void* pxSource, const zU32 uNumBytes);
		void				Flush();
	protected:
		

		AWForceInline void	Copy(const FileInfo_PC &_Copy)	{mhFileHandle = _Copy.mhFileHandle;}
		HANDLE				mhFileHandle;
	};

}  

#endif
