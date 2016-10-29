#pragma once
#ifndef __zBase_File_FileInfo_PC_h__
#define __zBase_File_FileInfo_PC_h__

namespace zbFile
{
	class FileInfo_PC
	{
		zenClassDeclareNoParent(FileInfo_PC);
	public:
							FileInfo_PC();

		void				Close(zI64& iFilePos, zI64& iFileSize, bool& bValidHandle);
		void				Seek(zI64& iNewFilePos, const zI64 iFilePos);
		void				Skip(zI64& iFilePos, const zI32 iNumOfBytes);
		void				Read(zI64& iFilePos, void* pxDest, const zU32 uNumBytes);
		void				Write(zI64& iFilePos, zI64& iFileSize, const void* pxSource, const zU32 uNumBytes);
		void				Flush();
	protected:
		zenInlineForce void	Copy(const FileInfo_PC &_Copy)	{mhFileHandle = _Copy.mhFileHandle;}
		HANDLE				mhFileHandle;
	};
}  

#endif
