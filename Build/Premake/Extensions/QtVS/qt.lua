--
-- define the qt extension
--
premake.extensions.qt = {
	--
	-- The available modules' definitions.
	-- Use the keys with qtmodule. For instance `qtmodule { "core", "gui" }` to use QtCore and QtGui
	--
	modules = {
		core = {
			name = "Core",
			include = "QtCore"
		},
		gui = {
			name = "Gui",
			include = "QtGui"
		},
		opengl = {
			name = "OpenGL",
			include = "QtOpenGL",
			links = { "OpenGL32" }
		},
		qml = {
			name = "Qml",
			include = "QtQml"
		},
		widgets = {
			name = "Widgets",
			include = "QtWidgets"
		}
	},
	
	--
	-- these are private, do not touch
	--
	enabled = false,
	defaultPath = os.getenv("QTDIR")
}



--
-- register our custom API
--

--
-- Set the path where Qt is installed
--
premake.api.register {
	name = "qtpath",
	scope = "config",
	kind = "path"
}

--
-- Set the prefix of the libraries ("Qt4" or "Qt5" usually)
--
premake.api.register {
	name = "qtprefix",
	scope = "config",
	kind = "string"
}

--
-- Set a suffix for the libraries ("d" usually when using Debug Qt libs)
--
premake.api.register {
	name = "qtsuffix",
	scope = "config",
	kind = "string"
}

--
-- Specify the modules to use (will handle include paths, links, etc.)
-- See premake.extensions.qt.modules for a list of available modules.
--
premake.api.register {
	name = "qtmodules",
	scope = "config",
	kind = "string-list"
}

--
-- Private use only : used by the addon to know if qt has already been enabled or not
--
premake.api.register {
	name = "qtenabled",
	scope = "project",
	kind = "string"
}

--
-- Enable Qt for a project. This must be called before calling any of the qt* api
-- functions previously registered.
--
function premake.extensions.qt.enable()

 	local qt = premake.extensions.qt
 	
 	-- enable Qt for the current config
 	qtenabled "true"

	-- setup our overrides if not already done
	if qt.enabled == false then
		qt.enabled = true
		premake.override(premake.oven, "bakeFiles", qt.customBakeFiles)
		premake.override(premake.oven, "bakeConfig", qt.customBakeConfig)
		premake.override(premake.fileconfig, "addconfig",  qt.customAddFileConfig)
	end

end

--
-- A small function which will get the generated directory for a given config.
-- If objdir was specified, it will be used. Else, it's the project's location +
-- obj + configuration + platform
--
-- @param
--		cfg The input configuration
--
function premake.extensions.qt.getGeneratedDir(cfg)

	if cfg.objdir ~= nil then
		return cfg.objdir
	end

	local dir = path.join(cfg.project.location, "obj")
	dir = path.join(dir, cfg.buildcfg)
	dir = path.join(dir, cfg.platform)

	return path.getabsolute(dir)

end

--
-- Override the project.bakeFile method to be able to add the Qt generated files
-- to the project.
--
-- @param
--		base The original getFiles method.
-- @param
--		prj The current project.
-- @return
--		The table of files.
--
function premake.extensions.qt.customBakeConfig(base, prj, buildcfg, platform)

	local qt = premake.extensions.qt
	local modules = qt.modules

	-- bake
	local config = base(prj, buildcfg, platform)

	-- check if we need to add Qt support
	if config.qtenabled == "true" then

		-- get the installation path
		local qtpath = config.qtpath or qt.defaultPath
		if qtpath == nil then
			error("Qt path is not set. Either use 'qtpath' in your project configuration or set the QTDIR environment variable.")
		end

		-- add the includes and libraries directories
		table.insert(config.includedirs, qtpath .. "/include")
		table.insert(config.libdirs, qtpath .. "/lib")

		-- add the modules
		for _, modulename in ipairs(config.qtmodules) do

			if modules[modulename] ~= nil then

				local module	= modules[modulename]
				local prefix	= config.qtprefix or ""
				local suffix	= config.qtsuffix or ""
				local libname	= prefix .. module.name .. suffix .. ".lib"
				local dllname	= prefix .. module.name .. suffix .. ".dll"

				-- configure the module
				table.insert(config.includedirs, qtpath .. "/include/" .. module.include)
				table.insert(config.links, libname)

				-- add additional links
				if module.links ~= nil then
					for __, additionallink in ipairs(module.links) do
						table.insert(config.links, additionallink)
					end
				end
			end
		end

	end

	-- return the modified config
	return config

end

--
-- Override the project.bakeFile method to be able to add the Qt generated files
-- to the project.
--
-- @param
--		base The original getFiles method.
-- @param
--		prj The current project.
-- @return
--		The table of files.
--
function premake.extensions.qt.customBakeFiles(base, prj)

	local qt		= premake.extensions.qt
	local project	= premake.project

	-- parse the configurations for the project
	for cfg in project.eachconfig(prj) do

		-- ignore this config if Qt is not enabled
		if cfg.qtenabled == "true" then

			local mocs	= {}
			local qrc	= {}

			-- add the objdir as an include path
			table.insert(cfg.includedirs, cfg.objdir)

			-- check each file in this configuration
			table.foreachi(cfg.files, function(filename)

				if qt.isQRC(filename) then
					table.insert(qrc, filename)
				elseif qt.needMOC(filename) then
					table.insert(mocs, filename)
				end

			end)

			-- the moc files
			table.foreachi(mocs, function(filename)
				table.insert(cfg.files, qt.getGeneratedDir(cfg) .. "/moc_" .. path.getbasename(filename) .. ".cpp")
			end)

			-- the qrc files
			table.foreachi(qrc, function(filename)
				table.insert(cfg.files, qt.getGeneratedDir(cfg) .. "/qrc_" .. path.getbasename(filename) .. ".cpp")
			end)
		end
	end

	return base(prj)

end

--
-- Override the base premake.fileconfig.addconfig method in order to add our
-- custom build rules for special Qt files.
--
-- @param base
--		The base method that we must call.
-- @param fcfg
--		The file configuration object.
-- @param cfg
--		The current configuration that we're adding to the file configuration.
--
function premake.extensions.qt.customAddFileConfig(base, fcfg, cfg)

	-- call the base method to add the file config
	base(fcfg, cfg)

	-- do nothing else if Qt is not enabled
	if cfg.qtenabled ~= "true" then
		return
	end

	-- get the current config
	local config = premake.fileconfig.getconfig(fcfg, cfg)

	-- now filter the files, and depending on their type, add our
	-- custom build rules

	local qt = premake.extensions.qt

	-- ui files
	if qt.isUI(config.abspath) then
		qt.addUICustomBuildRule(config)

	-- resource files
	elseif qt.isQRC(config.abspath) then
		qt.addQRCCustomBuildRule(config)

	-- moc files
	elseif qt.needMOC(config.abspath) then
		qt.addMOCCustomBuildRule(config)

	end

end

--
-- Checks if a file is a ui file.
--
-- @param filename
--		The file name to check.
-- @return
--		true if the file needs to be run through the uic tool, false if not.
--
function premake.extensions.qt.isUI(filename)
	return path.hasextension(filename, { ".ui" })
end

--
-- Adds the custom build for ui files.
--
-- @param config
--	  The configuration for a single file and project configuration.
--
function premake.extensions.qt.addUICustomBuildRule(config)

	local qt = premake.extensions.qt

	-- get the output file
	local output = qt.getGeneratedDir(config) .. "/ui_" .. config.basename .. ".h"

	-- add the custom build rule
	config.buildmessage		= "Uic'ing " .. config.name
	config.buildcommands	= { config.config.qtpath .. "/bin/uic.exe -o \"" .. output .. "\" \"" .. config.abspath .. "\"" }
	config.buildoutputs		= { output }

end

--
-- Checks if a file is a qrc file.
--
-- @param filename
--		The file name to check.
-- @return
--		true if the file needs to be run through the rcc tool, false if not.
--
function premake.extensions.qt.isQRC(filename)
	return path.hasextension(filename, { ".qrc" })
end

--
-- Adds the custom build for ui files.
--
-- @param config
--	  The configuration for a single file and project configuration.
--
function premake.extensions.qt.addQRCCustomBuildRule(config)

	local qt = premake.extensions.qt

	-- get the output file
	local output = qt.getGeneratedDir(config) .. "/qrc_" .. config.basename .. ".cpp"

	-- add the custom build rule
	config.buildmessage		= "Rcc'ing " .. config.name
	config.buildcommands	= { config.config.qtpath .. "/bin/rcc.exe -name \"" .. config.basename .. "\" -no-compress \"" .. config.abspath .. "\" -o \"" .. output .. "\"" }
	config.buildoutputs		= { output }

end

--
-- Checks if a file needs moc'ing.
--
-- @param filename
--		The file name to check.
-- @return
--		true if the header needs to be run through the moc tool, false if not.
--
function premake.extensions.qt.needMOC(filename)

	local needmoc = false

	-- only handle headers
	if path.iscppheader(filename) then

		-- open the file
		local file = io.open(filename)

		-- scan it to find 'Q_OBJECT'
		for line in file:lines() do
			if line:find("Q_OBJECT") then
				needmoc = true
				break
			end
		end

		io.close(file)
	end

	return needmoc
end

--
-- Adds the custom build for a moc'able file.
--
-- @param config
--	  The configuration for a single file and project configuration.
--
function premake.extensions.qt.addMOCCustomBuildRule(config)

	local qt = premake.extensions.qt

	-- create the output file name
	local output = qt.getGeneratedDir(config) .. "/moc_" .. config.basename .. ".cpp"

	-- create the moc command
	local command = config.config.qtpath .. "/bin/moc.exe \"" .. config.abspath .. "\" -o \"" .. output .. "\""

	-- if we have a precompiled header, add it, and don't forget to add the original header:
	-- the moc command will override it if we specify a -f include
	if config.config.pchheader then
		command = command .. " \"-f" .. config.config.pchheader .. "\" -f\"" .. config.abspath .. "\""
	end

	-- append the defines to the command
	if #config.config.defines > 0 then
		table.foreachi(config.config.defines, function (define)
			command = command .. " -D" .. define
		end)
	end

	-- append the include directories to the command
	if #config.config.includedirs > 0 then
		table.foreachi(config.config.includedirs, function (include)
			command = command .. " -I\"" .. include .. "\""
		end)
	end

	-- add the custom build rule
	config.buildmessage		= "Moc'ing " .. config.name
	config.buildcommands	= { command }
	config.buildoutputs		= { output }

end