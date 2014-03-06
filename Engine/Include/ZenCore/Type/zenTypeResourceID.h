#pragma once
#ifndef __zenCore_Type_ResourceID_h__
#define __zenCore_Type_ResourceID_h__

namespace zen { namespace zenType {

	//! @todo Move this to state files
	class awBlendDesc
	{
	ZENClassDeclareNoParent(awBlendDesc)
	public:
		struct awRTBlendDesc
		{
			awRTBlendDesc(void)
				: mbBlendEnable(false)
				, muRenderTargetWriteMask(0xFF)
				, meSrcBlend(zenConst::keBlend_SrcColor)
				, meDestBlend(zenConst::keBlend_InvSrcColor)
				, meBlendOp(zenConst::keBlendOp_Add)
				, meSrcBlendAlpha(zenConst::keBlend_SrcColor)
				, meDestBlendAlpha(zenConst::keBlend_InvSrcColor)
				, meBlendOpAlpha(zenConst::keBlendOp_Add)
			{
			}
			bool				mbBlendEnable;
			zenU8				muRenderTargetWriteMask;
			zenConst::eBlend		meSrcBlend;
			zenConst::eBlend		meDestBlend;
			zenConst::eBlendOp	meBlendOp;
			zenConst::eBlend		meSrcBlendAlpha;
			zenConst::eBlend		meDestBlendAlpha;
			zenConst::eBlendOp	meBlendOpAlpha;
		};

		AWForceInline awBlendDesc(void)
			: mbAlphaToCoverageEnable(false)
			, mbIndependentBlendEnable(false)
		{
		}

		bool			mbAlphaToCoverageEnable;
		bool			mbIndependentBlendEnable;
		awRTBlendDesc	mxRenderTarget[ 8 ];
	};

	class awDepthStencilDesc
	{
	ZENClassDeclareNoParent(awDepthStencilDesc)
	public:
		struct DepthStencilOp
		{
			DepthStencilOp(void)
				: meStencilFailOp(zenConst::keStencilOp_Keep)
				, meStencilDepthFailOp(zenConst::keStencilOp_Keep)
				, meStencilPassOp(zenConst::keStencilOp_Keep)
				, meStencilFunc(zenConst::keComparisonFunc_Always)
			{
			}
			zenConst::eStencilOp			meStencilFailOp;
			zenConst::eStencilOp			meStencilDepthFailOp;
			zenConst::eStencilOp			meStencilPassOp;
			zenConst::eComparisonFunc	meStencilFunc;
		};

		awDepthStencilDesc(void)
			: mbDepthEnable(false)
			, mbDepthWrite(false)
			, mbStencilEnable(false)
			, muStencilReadMask(0xFF)
			, muStencilWriteMask(0xFF)
			, meDepthFunc(zenConst::keComparisonFunc_Always)
		{
		}

		bool						mbDepthEnable;
		bool						mbDepthWrite;
		bool						mbStencilEnable;
		zenU8						muStencilReadMask;
		zenU8						muStencilWriteMask;
		zenConst::eComparisonFunc	meDepthFunc;
		DepthStencilOp				mxFrontFace;
		DepthStencilOp				mxBackFace;
	};

	class zenResID
	{
	ZENClassDeclareNoParent(zenResID)
	public:
		typedef zenHash32	NameHash;

		enum ePlatformType
		{
			kePlatformType_OS,
			kePlatformType_GFX,
			kePlatformType__Count,
		};
		zenResID():mhName(""), meType(zenConst::keResType__Invalid), mePlatform(zenConst::keResPlatform__Invalid), meSource(zenConst::keResSource_Runtime){}
		zenResID(NameHash _hName, zenConst::eResPlatform _ePlatform, zenConst::eResType _eType, zenConst::eResSource _eSource):mhName(_hName), meType(_eType), mePlatform(_ePlatform), meSource(_eSource){}								

		ZENInline const NameHash&			Name()const							{ return mhName; }
		ZENInline zenConst::eResType		Type()const							{ return static_cast<zenConst::eResType>(meType); }
		ZENInline zenConst::eResPlatform	Platform()const						{ return static_cast<zenConst::eResPlatform>(mePlatform); } 
		ZENInline zenConst::eResSource		Source()const						{ return static_cast<zenConst::eResSource>(meSource); }
		ZENInline zenU64					HashID()const						{ return muHashID; }
		ZENInline void						SetName(NameHash _hName)			{ mhName = _hName; }
		ZENInline void						SetType(zenConst::eResType _eResType)			{ meType = _eResType; }
		ZENInline void						SetPlatform(zenConst::eResPlatform _ePlatform)	{ mePlatform = _ePlatform; }
		ZENInline void						SetSource(zenConst::eResSource _eSource)			{ meSource = _eSource; }
		ZENInline bool						IsRuntime()const					{ return meSource==zenConst::keResSource_Runtime; }
		ZENInline bool						IsExport()const						{ return meSource==zenConst::keResSource_Offline; }
		ZENInline bool						IsLoaded()const						{ return meSource==zenConst::keResSource_Loaded; }
		ZENInline bool						IsValid()const						{ return meType < zenConst::keResType__Count; }
		ZENInline bool						operator==(const zenResID& _Compare)const{return muHashID == _Compare.muHashID;};
		ZENInline bool						operator!=(const zenResID& _Compare)const{return muHashID != _Compare.muHashID;};
		const zenResID&					operator=(const zenResID& _Copy){muHashID = _Copy.muHashID; return *this;}

	protected:
		union
		{
			struct 
			{			
				NameHash	mhName;		//!< Hashed name of the resource
				zenU16		meType;		//!< Resource type (eType)
				zenU8		mePlatform;	//!< Platform this should be used on
				zenU8		meSource;	//!< Source of the resource (eSource)			
			};
			zenU64 muHashID;
		};
	};
	ZENSupportMemcopy(zenResID);
	ZENSupportMemcopy(awBlendDesc);

}}  //namespace zen, Type 

#endif