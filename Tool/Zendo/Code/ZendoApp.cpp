#include "ZendoPCH.h"
#include "ZendoApp.h"

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. ZendoApp and not wxApp)
IMPLEMENT_APP_CONSOLE(ZendoApp)

class EngineInstance : public awsys::awEngineInstance
{
ZENClassDeclare(EngineInstance, awsys::awEngineInstance);
public:
	virtual bool Init()
	{
		return Super::Init();
	}
	virtual void Destroy()
	{
		Super::Destroy();
	}
	virtual void Update()
	{
		Super::Update();
	}
	virtual bool IsDone()
	{
		return false;
	}
protected:
};

ZendoApp::ZendoApp()
{
}

bool ZendoApp::OnInit()
{
	// call the base class initialization method, currently it only parses a
	// few common command-line options but it could be do more in the future
	if ( !wxApp::OnInit() )
		return false;

	wxInitAllImageHandlers();
	static EngineInstance Engine;
	
	//Connect( wxID_ANY, wxEVT_IDLE, wxIdleEventHandler(ZendoApp::onIdle) );
	if( Engine.Init() )
	{
		awsys::SetEngine(&Engine);
		/*
		AAss::Package testPackage;				
		testPackage.SetFilename( "TestDir/monpackage.xml" );
		AGfx::Texture2D* pAssTexture = (AGfx::Texture2D*)AAss::AssetItem::CreateItem(AAss::AssetItem::keType_Texture2D);
		testPackage.AssetAdd(pAssTexture);
		testPackage.Save();
		testPackage.SetFilename("TestDir2/monpackage.xml");
		*/
		mpFrame = zenNewDefault WndMain("Zendo Editor");
		zeMgr::Asset.Load();
		InitResources();
		if( mpFrame->Initialize() )
		{
			mpFrame->Show(true);												// And show it (the frames, unlike simple controls, are not shown when created initially)			
			return true;
		}		
	}
	return false;
}

int ZendoApp::OnExit()
{
	awsys::GetEngineInstance()->Destroy();
	return 0;
}

bool ZendoApp::InitResources()
{
	// Init Asset type icon
	zenUInt auIconSize[]={16,32,128};
	for( zenUInt idxSize(0); idxSize<ZENArrayCount(auIconSize); ++idxSize )
	{
		wxString zFolder	= wxString::Format("Zendo/Icons/ResourceType%i/", auIconSize[idxSize]);	//! @todo move root path outside assetmanager control
		wxString zFilename	= zFolder + "Unknown.png";
		wxImage imgIconUnknown(zFilename);
		if( imgIconUnknown.IsOk() )
		{
			wxBitmap bmpIconUnknown(imgIconUnknown);
			maIcon[keIco_Asset16+idxSize] = zenNewDefault wxImageList(auIconSize[idxSize], auIconSize[idxSize], true);
			for(zenUInt idxType(0); idxType<zeAss::AssetItem::keType__Count; ++idxType)
			{
				zFilename = zFolder + wxString(zeAss::AssetItem::GetTypeDesc(zeAss::AssetItem::enumType(idxType))) + wxT(".png");				
				wxImage imgIcon(zFilename);
				if( imgIcon.IsOk() )
				{
					imgIcon.Rescale(auIconSize[idxSize], auIconSize[idxSize]);
					wxBitmap bmpIcon(imgIcon);
					maIcon[keIco_Asset16+idxSize]->Add(bmpIcon, wxColour(0xFFFFFFFF));
				}		
				else
				{
					maIcon[keIco_Asset16+idxSize]->Add(bmpIconUnknown, wxColour(0xFFFFFFFF));
				}

			}
		}
	}
	
	mdAssetIcon.Init(512);
	mdAssetIcon.SetDefaultValue(NULL);
	return true;
}

const wxIcon* ZendoApp::GetIcon(const zeAss::AssetItem& _Item)
{
	wxIcon* pIcon = mdAssetIcon[_Item.GetID()];
	zenUInt size(128);
	if( !pIcon )
	{	
		pIcon = zenNewDefault wxIcon();
		*pIcon = wxArtProvider::GetIcon(wxART_INFORMATION,wxART_LIST, wxSize(size,size));
		mdAssetIcon.Set(_Item.GetID(), pIcon);		
	}
	return pIcon;
}
