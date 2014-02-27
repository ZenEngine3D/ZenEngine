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
				, meSrcBlend(awconst::keBlend_SrcColor)
				, meDestBlend(awconst::keBlend_InvSrcColor)
				, meBlendOp(awconst::keBlendOp_Add)
				, meSrcBlendAlpha(awconst::keBlend_SrcColor)
				, meDestBlendAlpha(awconst::keBlend_InvSrcColor)
				, meBlendOpAlpha(awconst::keBlendOp_Add)
			{
			}
			bool				mbBlendEnable;
			zenU8				muRenderTargetWriteMask;
			awconst::eBlend		meSrcBlend;
			awconst::eBlend		meDestBlend;
			awconst::eBlendOp	meBlendOp;
			awconst::eBlend		meSrcBlendAlpha;
			awconst::eBlend		meDestBlendAlpha;
			awconst::eBlendOp	meBlendOpAlpha;
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
				: meStencilFailOp(awconst::keStencilOp_Keep)
				, meStencilDepthFailOp(awconst::keStencilOp_Keep)
				, meStencilPassOp(awconst::keStencilOp_Keep)
				, meStencilFunc(awconst::keComparisonFunc_Always)
			{
			}
			awconst::eStencilOp			meStencilFailOp;
			awconst::eStencilOp			meStencilDepthFailOp;
			awconst::eStencilOp			meStencilPassOp;
			awconst::eComparisonFunc	meStencilFunc;
		};

		awDepthStencilDesc(void)
			: mbDepthEnable(false)
			, mbDepthWrite(false)
			, mbStencilEnable(false)
			, muStencilReadMask(0xFF)
			, muStencilWriteMask(0xFF)
			, meDepthFunc(awconst::keComparisonFunc_Always)
		{
		}

		bool						mbDepthEnable;
		bool						mbDepthWrite;
		bool						mbStencilEnable;
		zenU8						muStencilReadMask;
		zenU8						muStencilWriteMask;
		awconst::eComparisonFunc	meDepthFunc;
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
		zenResID():mhName(""), meType(awconst::keResType__Invalid), mePlatform(awconst::keResPlatform__Invalid), meSource(awconst::keResSource_Runtime){}
		zenResID(NameHash _hName, awconst::eResPlatform _ePlatform, awconst::eResType _eType, awconst::eResSource _eSource):mhName(_hName), meType(_eType), mePlatform(_ePlatform), meSource(_eSource){}								

		inline const NameHash&			Name()const							{ return mhName; }
		inline awconst::eResType		Type()const							{ return static_cast<awconst::eResType>(meType); }
		inline awconst::eResPlatform	Platform()const						{ return static_cast<awconst::eResPlatform>(mePlatform); } 
		inline awconst::eResSource		Source()const						{ return static_cast<awconst::eResSource>(meSource); }
		inline zenU64					HashID()const						{ return muHashID; }
		inline void						SetName(NameHash _hName)			{ mhName = _hName; }
		inline void						SetType(awconst::eResType _eResType)			{ meType = _eResType; }
		inline void						SetPlatform(awconst::eResPlatform _ePlatform)	{ mePlatform = _ePlatform; }
		inline void						SetSource(awconst::eResSource _eSource)			{ meSource = _eSource; }
		inline bool						IsRuntime()const					{ return meSource==awconst::keResSource_Runtime; }
		inline bool						IsExport()const						{ return meSource==awconst::keResSource_Offline; }
		inline bool						IsLoaded()const						{ return meSource==awconst::keResSource_Loaded; }
		inline bool						IsValid()const						{ return meType < awconst::keResType__Count; }
		inline bool						operator==(const zenResID& _Compare)const{return muHashID == _Compare.muHashID;};
		inline bool						operator!=(const zenResID& _Compare)const{return muHashID != _Compare.muHashID;};
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