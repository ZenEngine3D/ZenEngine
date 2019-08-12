#pragma once

namespace zcExp
{			
	//=================================================================================================
	//! @class		Used to keep array of serializable items
	//=================================================================================================
	template<class _Type_>
	class StaticArraySerial : public zArrayDyn<_Type_>, public ISerialize
	{
	zenClassDeclare(StaticArraySerial, ISerialize)
	public:
		StaticArraySerial()									: zArrayDyn<_Type_>(){}
		StaticArraySerial(const zArrayDyn<_Type_>& aCopy)	: zArrayDyn<_Type_>( aCopy ){}
		StaticArraySerial(zU32 auCount)						: zArrayDyn<_Type_>(auCount){}
		virtual bool Serialize( Serializer_Base& aSerializer )
		{		
			bool bSuccess = aSerializer.Serialize(muCount);	
			if( bSuccess )
			{
				if( aSerializer.GetStatus() == Serializer_Base::keStatus_Importing )
					resize(muCount);

				_Type_* pCurrent	= mpData;
				_Type_* pLast		= &mpData[muCount];
				while( pCurrent < pLast && bSuccess )
					bSuccess = aSerializer.Serialize( *pCurrent++ );
			}
			return bSuccess;
		}
	};
}
