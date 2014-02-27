#pragma once
#ifndef __zenBase_Type_List2x_h__
#define __zenBase_Type_List2x_h__

namespace zen { namespace zenType {

class zenList2xNode
{
ZENClassDeclareNoParent(zenList2xNode)
public:
							zenList2xNode	();
							~zenList2xNode	();
	void					LstRemove		();
	zenList2xNode*			LstNext			();
	zenList2xNode*			LstPrev			();
	void					LstReset		();
protected:
	zenList2xNode*			mpLstNext;	//!< Next item in the list
	zenList2xNode*			mpLstPrev;	//!< Previous item in the list
	friend class			zenList2x;		
};

//=================================================================================================
//! @class		zenList2xItem
//-------------------------------------------------------------------------------------------------
//! @brief		Item for a zenList2x. 
//! @n			Non intrusive class can use this for adding themselves to a list.
//! @n			Ownership of the object can be given to it, making sure the object will be deleted alongside the ListItem.
//=================================================================================================
class zenList2xItem : public zenList2xNode
{
ZENClassDeclare(zenList2xItem, zenList2xNode)
public:
							zenList2xItem	( void* apObject, bool abAutoDelete=false );
	virtual					~zenList2xItem	( void );

protected:
	void*					mpObject;
	bool					mbAutoDel;
};
 
//=================================================================================================
//! @class		zenList2x
//-------------------------------------------------------------------------------------------------
//! @brief		Simple double linked list.
//! @details	For usage example, look at  @link SampleList.cpp  @endlink
//!				- Can be @b Intrusive (adding objects inheriting from zenList2xNode) 
//!				- Or @b Non-Intrusive (adding zenList2xItem containing pointer to object)
//=================================================================================================
class zenList2x
{
ZENClassDeclareNoParent(zenList2x)
public:
							zenList2x	( 											);
							~zenList2x	( 											);
    
	void					Clear       ( bool _bDelete=false						);
	zenList2xNode*			GetHead     ( 											)const;
	zenList2xNode*			GetTail     ( 											)const;
	zenList2xNode*			PopHead		( void										);
	zenList2xNode*			PopTail		( void										);
	const zenList2xNode*	GetInvalid	(											)const;
	bool					IsEmpty		(											)const;
	void					AddHead     ( zenList2xNode* _pAdd						);
	void					AddTail     ( zenList2xNode* _pAdd						);
	
template<class _Type_> void	AddSort		( zenList2xNode* _pAdd,  _Type_* _pReference	);
	void					InsertBefore( zenList2xNode* AWRestrict _pAdd, zenList2xNode* AWRestrict _pItem		);
	void					InsertAfter ( zenList2xNode* AWRestrict _pAdd, zenList2xNode* AWRestrict _pItem		); 

protected:
	zenList2xNode			moRoot;		//!< Store reference to first(moRoot.mpLstNext) and last item(moRoot.mpLstPrev) of the list.
};

#include "zenTypeList2x.inl"

} } //namespace zen, Type



#endif	
