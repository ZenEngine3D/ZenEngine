#pragma once
#ifndef __LibCore_Api_Type_List2x_h__
#define __LibCore_Api_Type_List2x_h__

namespace zen { namespace awtype {

class awList2xNode
{
AWClassDeclareNoParent(awList2xNode)
public:
							awList2xNode	();
							~awList2xNode	();
	void					LstRemove		();
	awList2xNode*			LstNext			();
	awList2xNode*			LstPrev			();
	void					LstReset		();
protected:
	awList2xNode*			mpLstNext;	//!< Next item in the list
	awList2xNode*			mpLstPrev;	//!< Previous item in the list
	friend class			awList2x;		
};

//=================================================================================================
//! @class		awList2xItem
//-------------------------------------------------------------------------------------------------
//! @brief		Item for a awList2x. 
//! @n			Non intrusive class can use this for adding themselves to a list.
//! @n			Ownership of the object can be given to it, making sure the object will be deleted alongside the ListItem.
//=================================================================================================
class awList2xItem : public awList2xNode
{
AWClassDeclare(awList2xItem, awList2xNode)
public:
							awList2xItem	( void* apObject, bool abAutoDelete=false );
	virtual					~awList2xItem	( void );

protected:
	void*					mpObject;
	bool					mbAutoDel;
};
 
//=================================================================================================
//! @class		awList2x
//-------------------------------------------------------------------------------------------------
//! @brief		Simple double linked list.
//! @details	For usage example, look at  @link SampleList.cpp  @endlink
//!				- Can be @b Intrusive (adding objects inheriting from awList2xNode) 
//!				- Or @b Non-Intrusive (adding awList2xItem containing pointer to object)
//=================================================================================================
class awList2x
{
AWClassDeclareNoParent(awList2x)
public:
							awList2x	( 											);
							~awList2x	( 											);
    
	void					Clear       ( bool _bDelete=false						);
	awList2xNode*			GetHead     ( 											)const;
	awList2xNode*			GetTail     ( 											)const;
	awList2xNode*			PopHead		( void										);
	awList2xNode*			PopTail		( void										);
	const awList2xNode*		GetInvalid	(											)const;
	bool					IsEmpty		(											)const;
	void					AddHead     ( awList2xNode* _pAdd						);
	void					AddTail     ( awList2xNode* _pAdd						);
	
template<class _Type_> void	AddSort		( awList2xNode* _pAdd,  _Type_* _pReference	);
	void					InsertBefore( awList2xNode* AWRestrict _pAdd, awList2xNode* AWRestrict _pItem		);
	void					InsertAfter ( awList2xNode* AWRestrict _pAdd, awList2xNode* AWRestrict _pItem		); 

protected:
	awList2xNode			moRoot;		//!< Store reference to first(moRoot.mpLstNext) and last item(moRoot.mpLstPrev) of the list.
};

#include "zenTypeList2x.inl"

} } //namespace zen, Type



#endif	
