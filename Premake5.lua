-- Ontario Tech Teaching and Education Repository (OTTER) Framework
-- Shawn Matthews (Jan 1st, 2020)

-- This premake file sets up an easy to use framework for developing graphics applications, and allows easy addition
-- of other frameworks and libraries where required.
-- Originally named GOOP (Graphics with Object Oriented Programming), this project was renamed in 2020 to reflect it's
-- more general usage for other courses and concepts

-- This build config is inspired by the Hazel Engine build system (https://github.com/TheCherno/Hazel), as well as
-- David Arpe's CMAKE system

-- See the readme for more information

-- TODO:
-- Provide a system for modules to add resources or DLLs to the output projects

-- Determine the root directory where we are calling premake from (this is our working directory)
local rootDir = path.getabsolute(_WORKING_DIR)

-- Get all the directories in our projects directory
local projects = os.matchdirs(rootDir .. "/projects/*")
local modules = os.matchdirs(rootDir .. "/modules/*")
local sampleGroups = os.matchdirs(rootDir .. "/samples/*")

-- Select the last item in the project directory to be our startup project 
-- (this is easily changed in VS, this is just to be handy)
if #projects > 0 then
    startup = path.getbasename(projects[#projects])
else
    startup = ""
end

-- This function concatenates two LUA tables, returning a new result
-- @param table1 The starting table
-- @param table2 The table to append to table1
-- @returns All elements in table1, followed by all elements in table2
function concat(table1, table2)
	local result = {}
	for ix=1, #table1 do
		result[ix] = table1[ix]
	end
	for ix=1,#table2 do
		result[#table1 + ix] = table2[ix]
	end
	return result
end

-- Log what the startup project will be
premake.info("Startup project: " .. startup)

-- This is our solution name
workspace "OTTER"
	-- Processor architecture
	architecture "x64"

	-- Set which project to run on debug
	startproject(startup)

	-- Our configs are basically with and without compiler optimizations
	configurations {
		"Debug",
		"Release"
	}

-- The directory name for our output
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group("Dependencies")
-- These are other dependency projects that we want to include in our solution (each needs their own premake)
include "dependencies/glfw3"
include "dependencies/glad"
include "dependencies/imgui"
include "dependencies/stbs"
include "dependencies/spdlog"
include "dependencies/tinyGLTF"

-- Add all the core dependencies to the project includes
-- We will reserve the first include directory for the project's source
-- We will append our module's include directories to this list
ProjIncludes = {
	"RESERVED",
	"dependencies/glfw3/include",
	"dependencies/glad/include", 
	"dependencies/imgui",
	"dependencies/GLM/include",
	"dependencies/stbs",
	"dependencies/fmod",
	"dependencies/spdlog/include",
	"dependencies/entt",
	"dependencies/cereal",
	"dependencies/gzip",
	"dependencies/tinyGLTF",
	"dependencies/json",
}

-- These are all the default dependencies that require linking
Dependencies = {
	"GLFW",
	"Glad",
	"stbs",
	"ImGui",
	"opengl32.lib",
	"imagehlp.lib",
	"dependencies/fmod/fmod64.lib",
	"dependencies/gzip/zlib.lib",
	"tinyGLTF",
}

-- These are what we are linking to (mostly other projects)
-- We keep it as a seperate list so we don't accidentally link modules together
-- We will append our modules to this list
ProjLinks = { }
for k, v in pairs(Dependencies) do ProjLinks[k] = v end

-- This function handles creating the default project for a module, if no premake folder is given
-- @param folderName The path to the module, as collected from os.matchdirs
function CreateDefaultModule(folderName)

    premake.info(" Adding module from: " .. folderName)
	local projName = path.getname(folderName)
    premake.info(" Selected module name: " .. projName)

    -- We make a list of what to link against, since we don't want to modify Dependencies or the ProjLinks
    local linkList = {}
    for k, v in pairs(Dependencies) do table.insert(linkList, v) end

    -- We'll look for any libs the module may have
    local libs = os.matchfiles(folderName .. "/libs/*.lib")
    premake.info(" Looking for libs at: " .. folderName .. "/libs/*.lib")
    premake.info(" Found " .. #libs .. " libraries")

    -- Iterate over libs and link them to this module, and against all the client projects
    if #libs > 0 then
	    for k, v in pairs(libs) do 
            premake.info("  Adding lib to module: " .. v)
	    	table.insert(linkList, path.getrelative(rootDir, v))
	    end
	end

	local relpath = path.getrelative(rootDir, folderName)
    local srcdir = path.join(relpath, "src")

	project(projName)
		location(relpath)
	    kind "StaticLib"
	    language "C++"
	    cppdialect "C++17"

	    -- Sets RuntimLibrary to MultiThreaded (non DLL version for static linking)
	    staticruntime "on"
	    
	    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	    objdir ("obj/" .. outputdir .. "/%{prj.name}")

	    files {
	        "%{prj.location}\\src\\**.c",
	        "%{prj.location}\\src\\**.cpp",
	        "%{prj.location}\\include\\**.h",
	        "%{prj.location}\\include\\**.hpp"
	    }

	    -- Modules should only link to dependencies by default
	    links(linkList)

		-- We update the reserved include directory to be the project's source directory
		ProjIncludes[1] = path.join(relpath, "include")
		-- Defines what directories we want to include
		includedirs(ProjIncludes)

	    filter "system:windows"
	        systemversion "latest"

	        defines {
	            "WINDOWS",
	        }
	        
	    filter "configurations:Debug"
	        runtime "Debug"
	        symbols "on"

	    filter "configurations:Release"
	        runtime "Release"
	        optimize "on"
        
end

-- Executes an xcopy command to copy all newer files from one folder to another
function CopyFolder(sourcePath, destPath)
	os.execute("xcopy /Q /E /Y /I /C \"" .. sourcePath .. "\" \"" .. destPath .. "\"")
end

if not os.isdir(path.join(rootDir, "shared_assets")) then
	os.mkdir(path.join(rootDir, "shared_assets"))
end
if not os.isdir(path.join(rootDir, "shared_assets", "dll")) then
	os.mkdir(path.join(rootDir, "shared_assets", "dll"))
end
if not os.isdir(path.join(rootDir, "shared_assets", "res")) then
	os.mkdir(path.join(rootDir, "shared_assets", "res"))
end

-- Generate the modules
group("Modules")

for k, v in pairs(modules) do
	-- We will use the relative path for including in premake
	vRel = path.getrelative(rootDir, v)
	premake.info("Adding module: " .. vRel)
	
	-- If there is a premake file for the module, we will use it
	if os.isfile(path.join(vRel, "premake.lua")) or os.isfile(path.join(vRel, "premake5.lua")) then
		include(vRel)
	-- Otherwise we will generate one for the module
	elseif (os.isdir(path.join(vRel, "src"))) or (os.isdir(path.join(vRel, "libs"))) then
	    premake.info(" Generating project for module: " .. vRel)
		CreateDefaultModule(v)
	end

    -- If the module has a dll folder, copy it over into the shared assets
	if os.isdir(path.join(vRel, "dll")) then
		premake.info(" Copying module dlls to shared_assets")
		local sourcePath = path.join(vRel, "dll")
		local destPath = path.join(rootDir, "shared_assets", "dll")
		CopyFolder(sourcePath, destPath)
	end

    -- If the module has a res folder, copy it over into the shared assets
	if os.isdir(path.join(vRel, "res")) then
		premake.info(" Copying module resources to shared_assets")
		local sourcePath = path.join(vRel, "res")
		local destPath = path.join(rootDir, "shared_assets", "res")
		CopyFolder(sourcePath, destPath)
	end

	-- Add the module to the list of projects to link
    table.insert(ProjLinks, path.getbasename(vRel))
end

-- We'll add the include directories for all modules after they have been generated (so that we can use ProjIncludes in module generation)
for k, v in pairs(modules) do
    table.insert(ProjIncludes, path.join(v, "include"))
end


-- This function will create projects for all the paths in a table, and set the group name to the given value
-- @param groupName The name to group the projects under in the workspace
-- @param folders   The table of folders that contain the projects to add
function AddProjects(groupName, folders)

	premake.info("Building Group: " .. groupName)
	group(groupName)

	-- Iterate over all the projects (k is the index)
	for k, proj in pairs(folders) do

		-- Extract the project name
		local name = path.getbasename(proj)
		-- Extract a relative path for the project
		local relpath = path.getrelative(rootDir, proj)

		-- Log the project in the console
		premake.info(" Adding project: " .. name)
		premake.info("  Source: " .. relpath)

		-- Add project to the solution
		project(name)
			-- Root folder of the project
			location(relpath)
			-- Output type of the project
			kind "ConsoleApp"
			-- Language (we are using MSVC)
			language "C++"
			-- C++ version (we are using the c++17 standard)
			cppdialect "C++17"
			-- Sets RuntimLibrary to MultiThreaded (non DLL version for static linking)
			staticruntime "on"

			-- This is where we will output our compiled program
			targetdir ("%{wks.location}\\bin\\" .. outputdir .. "\\%{prj.name}")
			-- This is where we will output our intermediate files
			objdir ("%{wks.location}\\obj\\" .. outputdir .. "\\%{prj.name}")

			-- Set the debug working directory to the output directory
			debugdir ("%{wks.location}bin\\%{outputdir}\\%{prj.name}")

			-- Gets the absolute directory of the current project
			absdir = "%{wks.location}bin\\%{outputdir}\\%{prj.name}"

			-- Gets our project's resource file location
			resdir = "%{prj.location}res"

			-- Gets the location of the project's source code
			local srcdir = path.join(relpath, "src")

			-- These are the commands that get executed after build, but before debugging
			postbuildcommands {
				-- This step copies over anything in the dll folder to the output directory
		  		"(xcopy /Q /E /Y /I /C \"%{wks.location}shared_assets\\dll\" \"%{absdir}\")",
		  		"(xcopy /Q /E /Y /I /C \"%{wks.location}dependencies\\dll\" \"%{absdir}\")",
		  		-- This step ensures that the project has a resource directory
		  		"(IF NOT EXIST \"%{resdir}\" mkdir \"%{resdir}\")",
		  		"(xcopy /Q /E /Y /I /C \"%{wks.location}shared_assets\\res\" \"%{absdir}\")",
		  		-- This step copies all the resources to the output directory
		  		"(xcopy /Q /E /Y /I /C \"%{resdir}\" \"%{absdir}\")"
			} 

			-- Our source files are everything in the src folder
			files {
				"%{prj.location}\\src\\**.h",
				"%{prj.location}\\src\\**.cpp",
				"%{prj.location}\\src\\**.c",
	            "%{prj.location}\\src\\**.hpp"
			}

			-- Disable CRT secure warnings
			defines {
				"_CRT_SECURE_NO_WARNINGS"
			}

			-- We update the reserved include directory to be the project's source directory
			ProjIncludes[1] = srcdir
			-- Defines what directories we want to include
			includedirs(ProjIncludes)

			-- Link to the dependencies and modules
			links(ProjLinks)

			-- This filters for our windows builds
			filter "system:windows"
				systemversion "latest"

				-- Set some defines for the windows builds
				defines {
					"GLFW_INCLUDE_NONE", 
					"WINDOWS"
				}

			-- Filters for our debug configurations
			filter "configurations:Debug"
				runtime "Debug"
				symbols "on"

			-- Filters for release configuration
			filter "configurations:Release"
				runtime "Release"
				optimize "on"
	end

end

-- Add the User Projects and Sample Projects
AddProjects("Projects", projects)

for k, proj in pairs(sampleGroups) do
	local name = path.getbasename(proj);
    local samples = os.matchdirs(proj .. "/*")
    AddProjects("Samples - " .. name, samples)
end