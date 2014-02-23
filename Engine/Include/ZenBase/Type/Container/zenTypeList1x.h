#pragma once
#ifndef __LibCore_Api_Type_List1x_h__
#define __LibCore_Api_Type_List1x_h__

namespace zen { namespace awtype {

//=================================================================================================
//! @class		awList1xNode
//-------------------------------------------------------------------------------------------------
//! @brief		Item for a awList1x.
//!	@n			Intrusive list item can only belong to one list at a time since they store the next/previous pointer in themselves.
//!	@n			They allow for better performance/memory management by not needing to use a 'new' for each element added to a list.
//=================================================================================================
class awList1xNode
{
AWClassDeclareNoParent(awList1xNode)
public:
						awList1xNode	():mpLstNext(NULL){};
						~awList1xNode	(){};
inline  awList1xNode*	LstNext			();

protected:
		awList1xNode*	mpLstNext;	//!< Next item in the list
		friend class	awList1x;		
};

//=================================================================================================
//! @class		awList1xItem
//-------------------------------------------------------------------------------------------------
//! @brief		Item for a awList1x. 
//! @n			Non intrusive class can use this for adding themselves to a list.
//! @n			Ownership of the object can be given to it, making sure the object will be deleted alongside the ListItem.
//=================================================================================================
class awList1xItem : public awList1xNode
{
AWClassDeclare(awList1xItem, awList1xNode)
public:
						awList1xItem	( void* _pObject, bool _bAutoDelete=false );
	virtual				~awList1xItem	( void );

protected:
	void*				mpObject;
	bool				mbAutoDel;
};
 
//=================================================================================================
//! @class		awList1x
//-------------------------------------------------------------------------------------------------
//! @brief		Simple single linked list.
//! @details	For usage example, look at  @link SampleList.cpp  @endlink
//!				- Can be @b Intrusive (adding objects inheriting from awList1xNode) 
//!				- Or @b Non-Intrusive (adding awList1xItem containing pointer to object)
//=================================================================================================
class awList1x
{
AWClassDeclareNoParent(awList1x)
public:
							awList1x	( 											);
							~awList1x	( 											);
	
	void					Clear       ( bool _bDelete=false						);
	awList1xNode*			GetHead     ( void										)const;
	const awList1xNode*		GetInvalid	( void										)const;
	bool					IsEmpty		( void										)const;
	awList1xNode*			PopHead		( void										);
	bool					Remove		( awList1xNode* _pRemove					);
	void					AddHead     ( awList1xNode* _pAdd						);
template<class _Type_> void	AddSort		( awList1xNode* _pAdd,  _Type_* _pReference	);
	void					InsertAfter ( awList1xNode* AWRestrict _pAdd, awList1xNode* AWRestrict _pItem	); 

protected:
	awList1xNode			moFirst;	//!< Store reference to first(moRoot.mpLstNext) of the list.
};

#include "zenTypeList1x.inl"

} } //namespace zen, Type

#endif	
