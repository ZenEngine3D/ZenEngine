#pragma once
#ifndef __LibExport_Serializer_Type_h__
#define __LibExport_Serializer_Type_h__

namespace EExp
{			
	//=================================================================================================
	//! @class		Used to keep array of serializable items
	//=================================================================================================
	template<class _Type_>
	class StaticArraySerial : public zenArrayStatic<_Type_>, public ISerialize
	{
	ZENClassDeclare(StaticArraySerial, ISerialize)
	public:
		StaticArraySerial()										: zenArrayStatic<_Type_>(){}
		StaticArraySerial(const zenArrayStatic<_Type_>& aCopy)	: zenArrayStatic<_Type_>( aCopy ){}
		StaticArraySerial(zenU32 auCount)						: zenArrayStatic<_Type_>(auCount){}
		virtual bool Serialize( Serializer_Base& aSerializer )
		{		
			bool bSuccess = aSerializer.Serialize(muCount);	
			if( bSuccess )
			{
				if( aSerializer.GetStatus() == Serializer_Base::keStatus_Importing )
					SetCountNoConstructor(muCount);

				_Type_* pCurrent	= mpData;
				_Type_* pLast		= &mpData[muCount];
				while( pCurrent < pLast && bSuccess )
					bSuccess = aSerializer.Serialize( *pCurrent++ );
			}
			return bSuccess;
		}
	};
}

#endif
