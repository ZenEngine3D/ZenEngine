#include "ZendoPCH.h"
#include "ZendoApp.h"

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. ZendoApp and not wxApp)
wxIMPLEMENT_APP_CONSOLE(ZendoApp);

class EngineInstance : public zenSys::zEngineInstance
{
zenClassDeclare(EngineInstance, zenSys::zEngineInstance);
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
	virtual const char* GetAppName() const
	{
		return "Zendo";
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
		/*
		AAss::Package testPackage;				
		testPackage.SetFilename( "TestDir/monpackage.xml" );
		AGfx::Texture2D* pAssTexture = (AGfx::Texture2D*)AAss::Asset::CreateItem(AAss::Asset::keType_Texture2D);
		testPackage.AssetAdd(pAssTexture);
		testPackage.Save();
		testPackage.SetFilename("TestDir2/monpackage.xml");
		*/
		InitResources();
		mpFrame = zenNewDefault WndMain("Zendo Editor");
		
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
	zenSys::GetEngineInstance()->Destroy();
	return 0;
}

bool ZendoApp::InitResources()
{
	// Init Asset type icon
	zU32 auIconSize[]={16,32,128};
	for( zUInt idxSize(0); idxSize<zenArrayCount(auIconSize); ++idxSize )
	{
		wxString zFolder	= wxString::Format("Zendo/Icons/ResourceType%i/", auIconSize[idxSize]);	//! @todo Clean: move root path outside assetmanager control
		wxString zFilename	= zFolder + "Unknown.png";
		wxImage imgIconUnknown(zFilename);
		if( imgIconUnknown.IsOk() )
		{
			wxBitmap bmpIconUnknown(imgIconUnknown);
			maIcon[keIco_Asset16+idxSize] = zenNewDefault wxImageList(auIconSize[idxSize], auIconSize[idxSize], true);
			for(zUInt idxType(0); idxType<zenConst::keAssType__Count; ++idxType)
			{
				zFilename = zFolder + wxString(zenAss::AssetTypeToString(zenConst::eAssetType(idxType))) + wxT(".png");				
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
	mdAssetIcon.SetDefaultValue(nullptr);
	return true;
}

const wxIcon* ZendoApp::GetIcon(const zenAss::zAssetItemRef& _rAsset)
{
	wxIcon* pIcon = mdAssetIcon[_rAsset.GetIDUInt()];
	zUInt size(128);
	if( !pIcon )
	{	
		pIcon = zenNewDefault wxIcon();
		*pIcon = wxArtProvider::GetIcon(wxART_INFORMATION,wxART_LIST, wxSize(size,size));
		mdAssetIcon.Set(_rAsset.GetIDUInt(), pIcon);		
	}
	return pIcon;
}
