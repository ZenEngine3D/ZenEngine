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

		void				Close(awS64& iFilePos, awS64& iFileSize, bool& bValidHandle);
		void				Seek(awS64& iNewFilePos, const awS64 iFilePos);
		void				Skip(awS64& iFilePos, const awS32 iNumOfBytes);
		void				Read(awS64& iFilePos, void* pxDest, const awU32 uNumBytes);
		void				Write(awS64& iFilePos, awS64& iFileSize, const void* pxSource, const awU32 uNumBytes);
		void				Flush();
	protected:
		

		AWForceInline void	Copy(const FileInfo_PC &_Copy)	{mhFileHandle = _Copy.mhFileHandle;}
		HANDLE				mhFileHandle;
	};

}  

#endif
