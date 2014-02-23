#pragma once
#ifndef __LibExport_Serializer_Type_h__
#define __LibExport_Serializer_Type_h__

namespace EExp
{			
	//=================================================================================================
	//! @class		Used to keep array of serializable items
	//=================================================================================================
	template<class _Type_>
	class StaticArraySerial : public awArrayStatic<_Type_>, public ISerialize
	{
	AWClassDeclare(StaticArraySerial, ISerialize)
	public:
		StaticArraySerial()										: awArrayStatic<_Type_>(){}
		StaticArraySerial(const awArrayStatic<_Type_>& aCopy)	: awArrayStatic<_Type_>( aCopy ){}
		StaticArraySerial(awU32 auCount)						: awArrayStatic<_Type_>(auCount){}
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
