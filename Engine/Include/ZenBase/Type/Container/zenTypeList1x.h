#pragma once
#ifndef __zenBase_Type_List1x_h__
#define __zenBase_Type_List1x_h__

namespace zen { namespace zenType {

//=================================================================================================
//! @class		zenList1xNode
//-------------------------------------------------------------------------------------------------
//! @brief		Item for a zenList1x.
//!	@n			Intrusive list item can only belong to one list at a time since they store the next/previous pointer in themselves.
//!	@n			They allow for better performance/memory management by not needing to use a 'new' for each element added to a list.
//=================================================================================================
class zenList1xNode
{
AWClassDeclareNoParent(zenList1xNode)
public:
						zenList1xNode	():mpLstNext(NULL){};
						~zenList1xNode	(){};
inline  zenList1xNode*	LstNext			();

protected:
		zenList1xNode*	mpLstNext;	//!< Next item in the list
		friend class	zenList1x;		
};

//=================================================================================================
//! @class		zenList1xItem
//-------------------------------------------------------------------------------------------------
//! @brief		Item for a zenList1x. 
//! @n			Non intrusive class can use this for adding themselves to a list.
//! @n			Ownership of the object can be given to it, making sure the object will be deleted alongside the ListItem.
//=================================================================================================
class zenList1xItem : public zenList1xNode
{
AWClassDeclare(zenList1xItem, zenList1xNode)
public:
						zenList1xItem	( void* _pObject, bool _bAutoDelete=false );
	virtual				~zenList1xItem	( void );

protected:
	void*				mpObject;
	bool				mbAutoDel;
};
 
//=================================================================================================
//! @class		zenList1x
//-------------------------------------------------------------------------------------------------
//! @brief		Simple single linked list.
//! @details	For usage example, look at  @link SampleList.cpp  @endlink
//!				- Can be @b Intrusive (adding objects inheriting from zenList1xNode) 
//!				- Or @b Non-Intrusive (adding zenList1xItem containing pointer to object)
//=================================================================================================
class zenList1x
{
AWClassDeclareNoParent(zenList1x)
public:
							zenList1x	( 											);
							~zenList1x	( 											);
	
	void					Clear       ( bool _bDelete=false						);
	zenList1xNode*			GetHead     ( void										)const;
	const zenList1xNode*		GetInvalid	( void										)const;
	bool					IsEmpty		( void										)const;
	zenList1xNode*			PopHead		( void										);
	bool					Remove		( zenList1xNode* _pRemove					);
	void					AddHead     ( zenList1xNode* _pAdd						);
template<class _Type_> void	AddSort		( zenList1xNode* _pAdd,  _Type_* _pReference	);
	void					InsertAfter ( zenList1xNode* AWRestrict _pAdd, zenList1xNode* AWRestrict _pItem	); 

protected:
	zenList1xNode			moFirst;	//!< Store reference to first(moRoot.mpLstNext) of the list.
};

#include "zenTypeList1x.inl"

} } //namespace zen, Type

#endif	
