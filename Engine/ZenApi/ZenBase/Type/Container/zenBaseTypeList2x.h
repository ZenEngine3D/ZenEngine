#pragma once
#ifndef __zenApi_Base_Type_List2x_h__
#define __zenApi_Base_Type_List2x_h__

//!< @todo Cleanup Remove this and only uses zList

namespace zen { namespace zenType 
{

class zList2xNode
{
ZENClassDeclareNoParent(zList2xNode)
public:
							zList2xNode	();
							~zList2xNode	();
	void					LstRemove		();
	zList2xNode*			LstNext			();
	zList2xNode*			LstPrev			();
	void					LstReset		();
protected:
	zList2xNode*			mpLstNext;	//!< Next item in the list
	zList2xNode*			mpLstPrev;	//!< Previous item in the list
	friend class			zList2x;		
};

//=================================================================================================
//! @class		zList2xItem
//-------------------------------------------------------------------------------------------------
//! @brief		Item for a zList2x. 
//! @n			Non intrusive class can use this for adding themselves to a list.
//! @n			Ownership of the object can be given to it, making sure the object will be deleted alongside the ListItem.
//=================================================================================================
template<class TItem, bool TAutoDel=false>
class zList2xItem : public zList2xNode
{
ZENClassDeclare(zList2xItem, zList2xNode)
public:
							zList2xItem		( TItem* _pObject );
	virtual					~zList2xItem	( );
	TItem*					Get				( )const;
protected:
	TItem*					mpObject;
};
 
//=================================================================================================
//! @class		zList2x
//-------------------------------------------------------------------------------------------------
//! @brief		Simple double linked list.
//! @details	For usage example, look at  @link SampleList.cpp  @endlink
//!				- Can be @b Intrusive (adding objects inheriting from zList2xNode) 
//!				- Or @b Non-Intrusive (adding zList2xItem containing pointer to object)
//=================================================================================================
class zList2x
{
ZENClassDeclareNoParent(zList2x)
public:
						zList2x		( 											);
						~zList2x	( 											);
    
	void				Clear       ( bool _bDelete=false						);
	zList2xNode*		GetHead     ( 											)const;
	zList2xNode*		GetTail     ( 											)const;
	zList2xNode*		PopHead		( 											);
	zList2xNode*		PopTail		( 											);
	const zList2xNode*	GetInvalid	(											)const;
	bool				IsEmpty		(											)const;
	void				AddHead     ( zList2xNode* _pAdd						);
	void				AddTail     ( zList2xNode* _pAdd						);
	
	template<class _Type_> 
	void				AddSort		( zList2xNode* _pAdd,  _Type_* _pReference	);

	void				InsertBefore( zList2xNode* ZENRestrict _pItem,	zList2xNode* ZENRestrict _pAdd);
	void				InsertAfter	( zList2xNode* ZENRestrict _pItem,	zList2xNode* ZENRestrict _pAdd);

protected:
	zList2xNode			moRoot;		//!< Store reference to first(moRoot.mpLstNext) and last item(moRoot.mpLstPrev) of the list.
};

} } //namespace zen, Type

#endif