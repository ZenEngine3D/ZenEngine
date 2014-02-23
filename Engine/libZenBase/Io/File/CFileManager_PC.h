#pragma once
#ifndef __LibCore_File_FileManager_PC_h__
#define __LibCore_File_FileManager_PC_h__

namespace CFil
{
	class ManagerFile_PC : public ManagerFile_Base
	{
	AWClassDeclare(ManagerFile_PC, ManagerFile_Base);
	public:		
	//-------------------------------------------------------------------------
	// Common Interface to all ManagerFile
	//-------------------------------------------------------------------------
		bool						Search			( zenUInt _uFileSearchFlag, const wchar_t* _zDirName, const wchar_t* _zFilePatern, bool bRecursive);
		bool						SearchNext		( const FileInfo*& _pOutFile );
		bool						CreateDir		( const Filename& _Filename );

		bool						OpenFile		( const Filename& _Filename, FileInfo* _pOutFile );
		bool						OpenFileForWrite( const Filename& _Filename, FileInfo* _pOutFile );
	protected:
		void						LoadDirectory	( zenUInt _uWantedFlag, zenArrayDynamic<wchar_t>& _zDirName, const wchar_t* _zFilePatern, const zenUInt _uFilePaternLen, bool bRecursive);
		virtual bool				Load();
		virtual bool				Unload();
		zenArrayDynamic<wchar_t>		mzRoot;			//! @todo Use proper string class
		zenArrayDynamic<FileInfo>	maSearchResult;
		zenUInt						muSearchIndex;	
	};

}  

#endif
