#pragma once
#ifndef __zBase_File_FileManager_PC_h__
#define __zBase_File_FileManager_PC_h__

namespace zbFile
{
	class ManagerFile_PC : public ManagerFile_Base
	{
	zenClassDeclare(ManagerFile_PC, ManagerFile_Base);
	public:		
	//-------------------------------------------------------------------------
	// Common Interface to all ManagerFile
	//-------------------------------------------------------------------------
		bool						Search			( zArrayDynamic<FileInfo>& _ResultOut, zUInt _uFileSearchFlag, const wchar_t* _zDirName, const wchar_t* _zFilePatern, bool bRecursive);

		bool						OpenFile		( const Filename& _Filename, FileInfo* _pOutFile );
		bool						OpenFileForWrite( const Filename& _Filename, FileInfo* _pOutFile );

		bool						Delete			( const Filename& _Filename, bool _bRecursive=false );
		bool						Rename			( const Filename& _OldFilename, const Filename& _NewFilename );
		bool						CreateDir		( const Filename& _Filename, bool _bLastItemIsFile=false );

	protected:
		void						LoadDirectory	( zArrayDynamic<FileInfo>& _ResultOut, zUInt _uWantedFlag, zArrayDynamic<wchar_t>& _zDirName, const wchar_t* _zFilePatern, const zUInt _uFilePaternLen, bool bRecursive);
		virtual bool				Load();
		virtual bool				Unload();
	};

}  

#endif
