#pragma once
#ifndef __LibZenBase_File_FileManager_PC_h__
#define __LibZenBase_File_FileManager_PC_h__

namespace zbFile
{
	class ManagerFile_PC : public ManagerFile_Base
	{
	ZENClassDeclare(ManagerFile_PC, ManagerFile_Base);
	public:		
	//-------------------------------------------------------------------------
	// Common Interface to all ManagerFile
	//-------------------------------------------------------------------------
		bool						Search			( zUInt _uFileSearchFlag, const wchar_t* _zDirName, const wchar_t* _zFilePatern, bool bRecursive);
		bool						SearchNext		( const FileInfo*& _pOutFile );
		bool						CreateDir		( const Filename& _Filename );

		bool						OpenFile		( const Filename& _Filename, FileInfo* _pOutFile );
		bool						OpenFileForWrite( const Filename& _Filename, FileInfo* _pOutFile );
	protected:
		void						LoadDirectory	( zUInt _uWantedFlag, zArrayDynamic<wchar_t>& _zDirName, const wchar_t* _zFilePatern, const zUInt _uFilePaternLen, bool bRecursive);
		virtual bool				Load();
		virtual bool				Unload();
		zArrayDynamic<FileInfo>		maSearchResult;
		zUInt						muSearchIndex;	
	};

}  

#endif
