using Sharpmake;
using System;
using System.Collections.Generic; //List

namespace ZenSharpmake
{
	[Fragment, Flags]
	public enum GraphicsAPI
	{
		DirectX11 = 1 << 0,
		DirectX12 = 1 << 1
	}

	[Fragment, Flags]
	public enum EngineMode
	{
		Game = 1 << 0,
		Tool = 1 << 1
	}

	//==============================================================================================
	// Target : ZenTarget
	//==============================================================================================
	public class ZenTarget : ITarget
	{
		//------------------------------------------------------------------------------------------
		// Per Project platform settings that can be configured
		//------------------------------------------------------------------------------------------
		public Platform Platform;		
		public GraphicsAPI GraphicsAPI;
		public EngineMode EngineMode;

		//------------------------------------------------------------------------------------------
		// Default build settings that's the same for all target
		//------------------------------------------------------------------------------------------
		public Optimization Optimization = Optimization.Debug | Optimization.Release | Optimization.Retail;
		public DevEnv DevEnv = DevEnv.vs2015;
		public OutputType OutputType = OutputType.Lib;
		public DotNetFramework Framework = DotNetFramework.v3_5;
		public Blob Blob = Blob.NoBlob;
		public BuildSystem BuildSystem = BuildSystem.MSBuild;

		public override string Name { get { return Optimization.ToString(); } } //Used for 'Solution Configuration' Combobox entry naming

		public ZenTarget() { }
		public ZenTarget(Platform platform, GraphicsAPI graphicsAPI, EngineMode engineMode )
		{
			Platform = platform;
			GraphicsAPI = graphicsAPI;
			EngineMode = engineMode;
		}

		public string GetNamePlatform()
		{
			switch (Platform)
			{
				case Platform.win32: return "PC32"; 
				case Platform.win64: return "PC64";
				default: throw new NotImplementedException();
			}
		}
		public string GetNameGraphics()
		{
			switch (GraphicsAPI)
			{
				case GraphicsAPI.DirectX11: return "DX11";
				case GraphicsAPI.DirectX12: return "DX12";
				default: throw new NotImplementedException();
			}
		}
		public string GetNameEngineMode()
		{
			switch (EngineMode)
			{
				case EngineMode.Game: return "Game";
				case EngineMode.Tool: return "Tool";
				default: throw new NotImplementedException();
			}
		}
		public string GetNameBuild()
		{
			return GetNamePlatform() + "_" + GetNameGraphics() + "_" + GetNameEngineMode();
		}
	}

	//==============================================================================================
	// Solution : ZenAll
	//==============================================================================================
	[Sharpmake.Generate]
	public class ZenSolution : Sharpmake.Solution
	{
		public const string RootDir = @"[solution.SharpmakeCsPath]\..\..";

		public ZenSolution()
		: base(typeof(ZenTarget))
		{
			IsFileNameToLower = false;
		}		

		[Configure()]
		public void Configure(Configuration conf, ZenTarget target)
		{
			conf.SolutionFileName = "[solution.Name]_[target.DevEnv]";
			conf.SolutionPath = RootDir +  @"\Build\{Projects}";
			conf.PlatformName = target.GetNameBuild();
		}
	}

	//==============================================================================================
	// Zen Project
	//==============================================================================================
	public class ZenProject : Project
	{
		public const string RootDir = @"[project.SharpmakeCsPath]\..\..";
		private struct PlatformFileExt
		{
			public PlatformFileExt(string InFileExt, Platform InPlatformMask, GraphicsAPI graphicMask=0)
			{
				FileExt			= InFileExt;
				PlatformMask	= InPlatformMask;				
				GraphicMask		= graphicMask;
			}
			public string		FileExt;
			public Platform		PlatformMask;
			public GraphicsAPI	GraphicMask;
		};

		public Strings PchFiles;
		

		public ZenProject()
		: base(typeof(ZenTarget))
		{
			PchFiles = new Strings();
			IsFileNameToLower = false;
			IsTargetFileNameToLower = false;
		}

		// Allow child class to remove unsupported mask before adding the new configuration
		protected void AddTarget(ZenTarget InTarget)
		{
			AddTargets(InTarget);
		}
		
		protected ZenTarget CreateTargetAll()
		{
			return new ZenTarget(Platform.win32 | Platform.win64, GraphicsAPI.DirectX11 | GraphicsAPI.DirectX12, EngineMode.Game | EngineMode.Tool);
		}
		
		[Configure()]
		public void Configure(Configuration conf, ZenTarget target)
		{
			conf.ProjectFileName	= "[project.Name]_[target.DevEnv]";
			conf.ProjectPath		= RootDir + @"\Build\{Projects}\[project.Name]";
			conf.Name				= "[project.Name]_" + target.GetNameBuild() + "[target.Name]";			

			//--------------------------------------------------------------------------------------
			// Generates PreCompiled Header file
			//--------------------------------------------------------------------------------------
			//NOTE : At the moment only pick first file, and use its name for generating pch, should
			// be revisited to all use PrecompileHeader with multiple includes support
			if (PchFiles.Count > 0)
			{
				Resolver resolver = new Resolver();
				resolver.SetParameter("conf", conf);
				resolver.SetParameter("target", target);
				resolver.SetParameter("project", this);

				conf.PrecompHeader = "PrecompiledHeader.h";				
				conf.PrecompSource = "PrecompiledHeader.cpp";
				string PchOutputDir = resolver.Resolve(conf.ProjectPath);
				string OutputSource = PchOutputDir + "\\" + conf.PrecompSource;
				string OutputHeader = PchOutputDir + "\\" + conf.PrecompHeader;
				
				Util.ResolvePath(SharpmakeCsPath, ref PchOutputDir);
				System.IO.Directory.CreateDirectory(PchOutputDir);

				//-------------------
				//System.IO.File.WriteAllText(OutputSource, "//Generated PCH\n#include \"" + conf.PrecompHeader + "\"");				

				//System.IO.File.WriteAllText(OutputHeader, "//Generated PCH\n#pragma once\n\n");
				//foreach (string HeaderFile in PchFiles)
				//{					
				//	System.IO.File.AppendAllText(OutputHeader, "#include \"" + HeaderFile + "\"\n");
				//}
				//-------------------
				conf.PrecompHeader = PchFiles.SortedValues[0];// REMOVE ME
				System.IO.File.WriteAllText(OutputSource, "//Generated PCH\n#include \"" + PchFiles.SortedValues[0] + "\"");				
				//-------------------
				//See about moving this out of config?
				Strings NewPchFiles = new Strings(SourceFiles);
				NewPchFiles.Add(OutputSource);
				SourceFiles = NewPchFiles;
			}

			conf.IncludePrivatePaths.Add(SourceRootPath);

			//--------------------------------------------------------------------------------------
			// Remove compilation of files with extension not supported by current platform 
			// (use filename_[PLATFORM].ext naming convention)
			//--------------------------------------------------------------------------------------
			PlatformFileExt[] ConsoleFileExtensions = new PlatformFileExt[] {
				new PlatformFileExt("PC", Platform.win32|Platform.win64),
				new PlatformFileExt("DX11", 0, GraphicsAPI.DirectX11 ),
				new PlatformFileExt("DX12", 0, GraphicsAPI.DirectX12)
			};
			
			foreach (var Entry in ConsoleFileExtensions)
			{				
				if( Entry.PlatformMask.HasFlag(target.Platform) == false &&
					Entry.GraphicMask.HasFlag(target.GraphicsAPI) == false )
				{
					conf.SourceFilesBuildExcludeRegex.Add(".*_" + Entry.FileExt + @"\..*");
				}
			}

			//--------------------------------------------------------------------------------------
			// Add build config specific defines
			//--------------------------------------------------------------------------------------
			if ((Platform.win32 | Platform.win64).HasFlag(target.Platform))
			{				
				conf.Defines.Add("_WINDOWS");
				conf.Defines.Add("ZEN_PLATFORM=PC");
				conf.Defines.Add("ZEN_PLATFORM_PC=1");
				conf.Defines.Add("_HAS_EXCEPTIONS=0");
			}
			if ( target.GraphicsAPI == GraphicsAPI.DirectX11 )
			{
				conf.Defines.Add("ZEN_RENDERER=DX11");
				conf.Defines.Add("ZEN_RENDERER_DX11=1");
			}
			else if (target.GraphicsAPI == GraphicsAPI.DirectX12)
			{
				conf.Defines.Add("ZEN_RENDERER=DX12");
				conf.Defines.Add("ZEN_RENDERER_DX12=1");
			}

			if( target.Optimization == Optimization.Debug)
				conf.Defines.Add("ZEN_BUILD_DEBUG=1");
			else if (target.Optimization == Optimization.Release)
				conf.Defines.Add("ZEN_BUILD_RELEASE=1");
			else if (target.Optimization == Optimization.Retail)
				conf.Defines.Add("ZEN_BUILD_FINAL=1");

			if (target.EngineMode == EngineMode.Game)
				conf.Defines.Add("ZEN_ENGINEGAME=1");
			else if (target.EngineMode == EngineMode.Tool)
				conf.Defines.Add("ZEN_ENGINETOOL=1");

			//--------------------------------------------------------------------------------------
			// Others options per Platform
			//--------------------------------------------------------------------------------------
			if (target.GraphicsAPI == GraphicsAPI.DirectX12)
			{
				conf.IncludePrivatePaths.Add(RootDir + @"\Engine\ThirdParty\WinPixEventRuntime\Include\WinPixEventRuntime");
			}

			//--------------------------------------------------------------------------------------
			// Compilers options
			//--------------------------------------------------------------------------------------
			//Disable some warnings
			List<string> DisabledWarning = new List<string>();
			DisabledWarning.Add("4201"); // Nameless structure
			DisabledWarning.Add("4100"); // Unreferenced parameter
			DisabledWarning.Add("4127"); // conditional expression is constant
			DisabledWarning.Add("4505"); // unreferenced local function has been removed
			DisabledWarning.Add("4238"); // nonstandard extension used: class rvalue used as lvalue (Using & on constructor of class for array init)

			conf.Options.Add(new Sharpmake.Options.Vc.Compiler.DisableSpecificWarnings(DisabledWarning.ToArray()));
			conf.Options.Add(Sharpmake.Options.Vc.General.TreatWarningsAsErrors.Enable);
			conf.Options.Add(Sharpmake.Options.Vc.Compiler.Exceptions.Disable);
			conf.Options.Add(Sharpmake.Options.Vc.Compiler.MinimalRebuild.Disable); //Incompatible with /MP (multi process build)
			conf.Options.Add(Sharpmake.Options.Vc.Compiler.RTTI.Enable); //Temp, until fix issue with RTTI
			conf.Options.Add(Options.Vc.General.CharacterSet.Unicode);
			conf.Options.Add(Options.Vc.General.WindowsTargetPlatformVersion.v10_0_14393_0);
		}
	}

	public class ZenProjectLibrary : ZenProject
	{
 		[Configure()]
 		public new void Configure(Configuration conf, ZenTarget target)
 		{
 			base.Configure(conf, target);
 			conf.Output = Project.Configuration.OutputType.Lib;
			conf.TargetPath = RootDir + @"\Build\{projects}\[project.Name]\Output\" + target.Platform.ToString();
			conf.TargetFileName = "[project.Name]_" + target.GetNameBuild() + "_[target.Name]";
		}		
	}

	public class ZenProjectExe : ZenProject
	{
		[Configure()]
		public new void Configure(Configuration conf, ZenTarget target)
		{
			base.Configure(conf, target);
			conf.Output = Project.Configuration.OutputType.Exe;
			conf.TargetPath = RootDir + @"\Build\{Bin}\" + target.Platform.ToString();
			conf.TargetFileName = "[project.Name]_" + target.GetNameGraphics() + "_[target.Name]";

			//------------------------------------------------------
			if (target.GraphicsAPI == GraphicsAPI.DirectX11)
			//------------------------------------------------------
			{
				conf.LibraryFiles.Add("d3d11");
				conf.LibraryFiles.Add("d3dcompiler");
				conf.LibraryFiles.Add("dxguid");				
			}
			//------------------------------------------------------
			else if (target.GraphicsAPI == GraphicsAPI.DirectX12)
			//------------------------------------------------------
			{
				conf.LibraryFiles.Add("d3d12");
				conf.LibraryFiles.Add("d3dcompiler");
				conf.LibraryFiles.Add("dxgi");
				conf.LibraryFiles.Add("dxguid");				
			}
		}
	}

	public class ZenProjectExeGame : ZenProjectExe
	{
		protected new void AddTarget(ZenTarget InTarget)
		{
			InTarget.EngineMode &= EngineMode.Game;
			base.AddTarget(InTarget);
		}

		[Configure()]
		public new void Configure(Configuration conf, ZenTarget target)
		{			
			base.Configure(conf, target);			
		}
	}

	public class ZenProjectExeTool : ZenProjectExe
	{
		protected new void AddTarget(ZenTarget InTarget)
		{
			InTarget.EngineMode &= EngineMode.Tool;
			base.AddTarget(InTarget);
		}
		[Configure()]
		public new void Configure(Configuration conf, ZenTarget target)
		{
			base.Configure(conf, target);			
		}

	}

}
