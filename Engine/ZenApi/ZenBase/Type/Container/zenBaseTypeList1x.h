#pragma once
#ifndef __zenApi_Base_Type_List1x_h__
#define __zenApi_Base_Type_List1x_h__

namespace zen { namespace zenType 
{

//=================================================================================================
//! @class		zList1xNode
//-------------------------------------------------------------------------------------------------
//! @brief		Item for a zList1x.
//!	@n			Intrusive list item can only belong to one list at a time since they store the next/previous pointer in themselves.
//!	@n			They allow for better performance/memory management by not needing to use a 'new' for each element added to a list.
//=================================================================================================
class zList1xNode
{
ZENClassDeclareNoParent(zList1xNode)
public:
						zList1xNode	():mpLstNext(NULL){};
						~zList1xNode	(){};
ZENInline  zList1xNode*	LstNext			();

protected:
		zList1xNode*	mpLstNext;	//!< Next item in the list
		friend class	zList1x;		
};

//=================================================================================================
//! @class		zList1xItem
//-------------------------------------------------------------------------------------------------
//! @brief		Item for a zList1x. 
//! @n			Non intrusive class can use this for adding themselves to a list.
//! @n			Ownership of the object can be given to it, making sure the object will be deleted alongside the ListItem.
//=================================================================================================
class zList1xItem : public zList1xNode
{
ZENClassDeclare(zList1xItem, zList1xNode)
public:
						zList1xItem	( void* _pObject, bool _bAutoDelete=false );
	virtual				~zList1xItem	( void );

protected:
	void*				mpObject;
	bool				mbAutoDel;
};
 
//=================================================================================================
//! @class		zList1x
//-------------------------------------------------------------------------------------------------
//! @brief		Simple single linked list.
//! @details	For usage example, look at  @link SampleList.cpp  @endlink
//!				- Can be @b Intrusive (adding objects inheriting from zList1xNode) 
//!				- Or @b Non-Intrusive (adding zList1xItem containing pointer to object)
//=================================================================================================
class zList1x
{
ZENClassDeclareNoParent(zList1x)
public:
							zList1x	( 											);
							~zList1x	( 											);
	
	void					Clear       ( bool _bDelete=false						);
	zList1xNode*			GetHead     ( void										)const;
	const zList1xNode*		GetInvalid	( void										)const;
	bool					IsEmpty		( void										)const;
	zList1xNode*			PopHead		( void										);
	bool					Remove		( zList1xNode* _pRemove					);
	void					AddHead     ( zList1xNode* _pAdd						);
template<class _Type_> void	AddSort		( zList1xNode* _pAdd,  _Type_* _pReference	);
	void					InsertAfter ( zList1xNode* ZENRestrict _pAdd, zList1xNode* ZENRestrict _pItem	); 

protected:
	zList1xNode			moFirst;	//!< Store reference to first(moRoot.mpLstNext) of the list.
};

} } //namespace zen, Type

#endif	
