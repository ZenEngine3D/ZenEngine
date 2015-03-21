#pragma once
#ifndef __zCore_Exp_Ser_Type_h__
#define __zCore_Exp_Ser_Type_h__

namespace zcExp
{			
	//=================================================================================================
	//! @class		Used to keep array of serializable items
	//=================================================================================================
	template<class _Type_>
	class StaticArraySerial : public zArrayStatic<_Type_>, public ISerialize
	{
	ZENClassDeclare(StaticArraySerial, ISerialize)
	public:
		StaticArraySerial()										: zArrayStatic<_Type_>(){}
		StaticArraySerial(const zArrayStatic<_Type_>& aCopy)	: zArrayStatic<_Type_>( aCopy ){}
		StaticArraySerial(zU32 auCount)						: zArrayStatic<_Type_>(auCount){}
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
