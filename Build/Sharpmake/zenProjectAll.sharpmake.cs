using Sharpmake;
using System;
[module: Sharpmake.Include("zenShared.sharpmake.cs")]

namespace ZenSharpmake
{
	//==============================================================================================
	// Project : Library - Base 
	//==============================================================================================
	[Sharpmake.Generate]
	public class ZenLibBase : ZenProjectLibrary
	{
		public ZenLibBase()
		{
			Name = "ZenBaseLib";
			SourceRootPath = RootDir +  @"\Engine\ZenBase";
			AdditionalSourceRootPaths.Add(RootDir + @"\Engine\ZenApi\ZenBase");
			PchFiles.Add("zbBase.h");
			AddTarget(CreateTargetAll());
		}
		
		[Configure()]
		public new void Configure(Configuration conf, ZenTarget target)
		{
			base.Configure(conf, target);
			conf.Defines.Add("ZEN_ENGINELIB=1"); 
			conf.IncludePaths.Add(RootDir + @"\Engine");
			conf.IncludePaths.Add(RootDir + @"\Engine\ZenApi");
			conf.IncludePaths.Add(RootDir + @"\Engine\ZenApi\ZenBase");
			conf.IncludePaths.Add(RootDir + @"\Engine\ZenApi");
			conf.IncludePrivatePaths.Add(RootDir + @"\Engine\ZenBase");
			conf.IncludePrivatePaths.Add(RootDir + @"\Engine\ZenBase\ZenBase");
		
			conf.AddPublicDependency<ZenLibThirdPartyImgui>(target);
			conf.AddPublicDependency<ZenLibThirdPartyEASTL>(target);
		}
	}

	//==============================================================================================
	// Project : Library - Core 
	//==============================================================================================
	[Sharpmake.Generate]
	public class ZenLibCore: ZenProjectLibrary
	{
		public ZenLibCore()
		{
			Name = "ZenCoreLib";
			SourceRootPath = RootDir + @"\Engine\ZenCore";
			AdditionalSourceRootPaths.Add(RootDir + @"\Engine\ZenApi\ZenCore");
			PchFiles.Add("zcCore.h");
			AddTarget(CreateTargetAll());
		}

		[Configure()]
		public new void Configure(Configuration conf, ZenTarget target)
		{
			base.Configure(conf, target);
			conf.Defines.Add("ZEN_ENGINELIB=1");
			conf.AddPublicDependency<ZenLibBase>(target);
									
			conf.IncludePaths.Add(RootDir + @"\Engine\ZenApi\ZenCore");
			conf.IncludePrivatePaths.Add(RootDir + @"\Engine\ZenCore");
			conf.IncludePrivatePaths.Add(RootDir + @"\Engine\ZenCore\ZenCore");			
		}
	}
	//==============================================================================================
	// Project : Library - Engine
	//==============================================================================================
	[Sharpmake.Generate]
	public class ZenLibEngine : ZenProjectLibrary
	{
		public ZenLibEngine()
		{
			Name = "ZenEngineLib";
			SourceRootPath = RootDir + @"\Engine\ZenEngine";
			AdditionalSourceRootPaths.Add(RootDir + @"\Engine\ZenApi\ZenEngine");
			PchFiles.Add("zeEngine.h");
			AddTarget(CreateTargetAll());
		}

		[Configure()]
		public new void Configure(Configuration conf, ZenTarget target)
		{
			base.Configure(conf, target);
			conf.Defines.Add("ZEN_ENGINELIB=1");
			conf.AddPublicDependency<ZenLibCore>(target);
			
			conf.IncludePaths.Add(RootDir + @"\Engine\ZenApi\ZenEngine");
			conf.IncludePrivatePaths.Add(RootDir + @"\Engine\ZenEngine");
			conf.IncludePrivatePaths.Add(RootDir + @"\Engine\ZenEngine\ZenEngine");
		}
	}
	//==============================================================================================
	// Project : Library - External
	//==============================================================================================
	[Sharpmake.Generate]
	public class ZenLibExternal: ZenProjectLibrary
	{
		public ZenLibExternal()
		{
			Name = "ZenExternalLib";
			SourceRootPath = RootDir + @"\Engine\ZenExternal";
			//AdditionalSourceRootPaths.Add(RootDir + @"\Engine\ZenApi\ZenExternal");
			PchFiles.Add("zxExternal.h");
			AddTarget(CreateTargetAll());
		}

		[Configure()]
		public new void Configure(Configuration conf, ZenTarget target)
		{
			base.Configure(conf, target);
			conf.Defines.Add("ZEN_ENGINELIB=1");
			conf.IncludePaths.Add(RootDir + @"\Engine\ZenExternal");
			conf.AddPublicDependency<ZenLibEngine>(target);
			
			if( target.EngineMode == EngineMode.Tool )
				conf.AddPublicDependency<ZenLibThirdPartyPugiXml>(target);			
		}
	}
	//==============================================================================================
	// Project : Library - External - Imgui
	//==============================================================================================
	[Sharpmake.Generate]
	public class ZenLibThirdPartyImgui : ZenProjectLibrary
	{
		public ZenLibThirdPartyImgui()
		{
			Name = "ThirdPartyImguiLib";
			SourceRootPath = RootDir + @"\Engine\ThirdParty\imgui";
			AddTarget(CreateTargetAll());
		}

		[Configure()]
		public new void Configure(Configuration conf, ZenTarget target)
		{
			base.Configure(conf, target);
			conf.Defines.Add("ZEN_THIRDPARTY_IMGUI=1");
			conf.ExportDefines.Add("ZEN_THIRDPARTY_IMGUI=1");
			conf.SourceFilesBuildExcludeRegex.Add(@"\.*\\examples\\");
			conf.SourceFilesBuildExcludeRegex.Add(@"\.*\\extra_fonts\\");
		}
	}
	
	//==============================================================================================
	// Project : Library - External - EA STL
	//==============================================================================================
	[Sharpmake.Generate]
	public class ZenLibThirdPartyEASTL : ZenProjectLibrary
	{
		public ZenLibThirdPartyEASTL()
		{
			Name = "ThirdPartyEASTLLib";
			SourceRootPath = RootDir + @"\Engine\ThirdParty\EASTL\Source";
			AdditionalSourceRootPaths.Add(RootDir + @"\Engine\ThirdParty\EASTL\Include");
			AddTarget(CreateTargetAll());
		}

		[Configure()]
		public new void Configure(Configuration conf, ZenTarget target)
		{
			base.Configure(conf, target);
			conf.IncludePaths.Add(RootDir + @"\Engine\ThirdParty\EASTL\Include");
			conf.IncludePaths.Add(RootDir + @"\Engine\ThirdParty\EASTL\test\packages\EABase\include\Common");
			conf.Defines.Add("ZEN_THIRDPARTY_EASTL=1");
			conf.Defines.Add("EA_COMPILER_NO_NOEXCEPT=0");
			conf.ExportDefines.Add("ZEN_THIRDPARTY_EASTL=1");
			conf.ExportDefines.Add("EA_COMPILER_NO_NOEXCEPT=0");
		}
	}
	
	//==============================================================================================
	// Project : Library - External - Pugi XML
	//==============================================================================================
	[Sharpmake.Generate]
	public class ZenLibThirdPartyPugiXml : ZenProjectLibrary
	{
		public ZenLibThirdPartyPugiXml()
		{
			Name = "ThirdPartyPugiXmlLib";
			SourceRootPath = RootDir + @"\Engine\ThirdParty\pugixml\src";
			AddTarget(CreateTargetAll());
		}
		
		[Configure()]
		public new void Configure(Configuration conf, ZenTarget target)
		{
			base.Configure(conf, target);
			conf.Defines.Add("ZEN_THIRDPARTY_PUGIXML=1");
			conf.ExportDefines.Add("ZEN_THIRDPARTY_PUGIXML=1");
		}
	}

	//==============================================================================================
	// Project : Exe - Samples
	//==============================================================================================
	public class ZenExeSample : ZenProjectExeGame
	{
		public ZenExeSample(string InProjectName, string InSourcesPath)
		{
			Name = InProjectName;
			SourceRootPath = RootDir + @"\Engine\Samples\" + InSourcesPath;
			PchFiles.Add("zenEngine.h");
			AddTarget(CreateTargetAll());
		}

 		[Configure()]
 		public new void Configure(Configuration conf, ZenTarget target)
 		{
 			base.Configure(conf, target);
			conf.AddPublicDependency<ZenLibExternal>(target);
			//DisabledWarning.Add("4238"); // nonstandard extension used: class rvalue used as lvalue (Using & on constructor of class for array init)

			conf.Options.Add(new Sharpmake.Options.Vc.Compiler.DisableSpecificWarnings("4189")); //local variable is initialized but not referenced
			
		}
	}
	
	[Sharpmake.Generate] public class ZenExeSampleBaseType		: ZenExeSample{ public ZenExeSampleBaseType() 		: base("SampleBaseType", @"Core\Type" ) {} }
	[Sharpmake.Generate] public class ZenExeSampleBaseMem 		: ZenExeSample{ public ZenExeSampleBaseMem() 		: base("SampleBaseMem", @"Core\Mem" ) {} }
	[Sharpmake.Generate] public class ZenExeSampleBaseLog 		: ZenExeSample{ public ZenExeSampleBaseLog() 		: base("SampleBaseLog", @"Core\Log" ) {} }
	[Sharpmake.Generate] public class ZenExeSampleBaseMath 		: ZenExeSample{ public ZenExeSampleBaseMath() 		: base("SampleBaseMath", @"Core\Math" ) {} }
	[Sharpmake.Generate] public class ZenExeSampleBaseSignal	: ZenExeSample{ public ZenExeSampleBaseSignal() 	: base("SampleBaseSignal", @"Core\Signal" ) {} }
	[Sharpmake.Generate] public class ZenExeSampleBaseSystem	: ZenExeSample{ public ZenExeSampleBaseSystem()		: base("SampleBaseSystem", @"Core\System" ) {} }
	
	[Sharpmake.Generate] public class ZenExeSampleGfxSimple		: ZenExeSample {public ZenExeSampleGfxSimple() 		: base("SampleGfxSimple", @"Gfx\Renderer" ) {} }
	[Sharpmake.Generate] public class ZenExeSampleGfxDebugUI 	: ZenExeSample {public ZenExeSampleGfxDebugUI() 	: base("SampleGfxDebugUI", @"Gfx\DebugUI" ) {} }
	[Sharpmake.Generate] public class ZenExeSampleGfxDirextX12 	: ZenExeSample {public ZenExeSampleGfxDirextX12() 	: base("SampleGfxDirextX12", @"Gfx\DirectX12" ) {} }


	//==============================================================================================
	// Solution : ZenAll
	//==============================================================================================
	[Sharpmake.Generate]
	public class SolutionZenAll : ZenSolution
	{
		public SolutionZenAll()
		{
			Name = "ZenAll";			
			AddTargets(new ZenTarget(Platform.win64, GraphicsAPI.DirectX11 | GraphicsAPI.DirectX12, EngineMode.Game | EngineMode.Tool));
		}

		[Configure()]
		public new void Configure(Configuration conf, ZenTarget target)
		{
			base.Configure(conf, target);
					
			//--------------------------------------------------------------------------------------
			// Samples
			//--------------------------------------------------------------------------------------
			if (target.EngineMode == EngineMode.Game)
			{
				conf.AddProject<ZenExeSampleBaseType>(target);
				conf.AddProject<ZenExeSampleBaseMem>(target);
				conf.AddProject<ZenExeSampleBaseLog>(target);
				conf.AddProject<ZenExeSampleBaseMath>(target);
				conf.AddProject<ZenExeSampleBaseSignal>(target);
				conf.AddProject<ZenExeSampleBaseSystem>(target);
				conf.AddProject<ZenExeSampleGfxSimple>(target);
				conf.AddProject<ZenExeSampleGfxDebugUI>(target);
				conf.AddProject<ZenExeSampleGfxDirextX12>(target);
			}
		}
	}	

	internal static class main
	{
		[Sharpmake.Main]
		public static void SharpmakeMain(Sharpmake.Arguments arguments)
		{
			arguments.Generate<SolutionZenAll>();	
		}
	}

}

