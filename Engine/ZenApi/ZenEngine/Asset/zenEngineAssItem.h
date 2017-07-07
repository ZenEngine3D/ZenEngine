#pragma once

#if ZEN_ENGINETOOL

namespace zeAss { class Asset; }

namespace zen { namespace zenAss 
{
	class zAssetItemRef : public zEngineRef<zeAss::Asset>
	{
	zenClassDeclare(zAssetItemRef, zEngineRef)
	public:
										zAssetItemRef();
										zAssetItemRef(const zAssetItemRef& _Copy);
										zAssetItemRef(zeAss::Asset* _pAsset);
		const zAssetID&					GetID()const;
		zU64							GetIDUInt()const;
		const zString&					GetName()const;
		const zString&					GetDescription()const;
		zUInt							GetValueCount()const;
		zenConst::eAssetType			GetType()const;
		class zPackageRef				GetPackage();
		class PropertyValueRef			GetValue(zUInt _uValIndex);
		
		void							InitDefault();

		void							SetPackage(zPackageRef& _rPackage);
		void							SetName(const char* _zName);

		void							Delete();
		zAssetItemRef&					operator=(const zAssetItemRef& _Copy);

		zenSig::zSignal1<zenAss::PropertyValueRef>& GetSignalUpdatedProperty();
		//zenSig::zSignal1<zenAss::PropertyValueRef>& GetSignalUpdatedProperty();
	};

	zAssetItemRef						AssetCreate			(zenConst::eAssetType _eAssetType, zPackageRef& _rPackage);
	const char*							AssetTypeToString	(zenConst::eAssetType _ePropertyType);
	zenConst::eAssetType				AssetNameToType		(zHash32 _hAssetName);
	const zAssetItemRef&				AssetGet			(const zAssetID& _uAssetID); //! @todo Asset: Move to an asset manager at api level?

	typedef zArraySparse<zenAss::zAssetItemRef>::Key64 zArrayAsset;
}}// namespace zen { namespace zenAss

#endif