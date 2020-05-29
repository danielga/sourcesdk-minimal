local current_dir = _SCRIPT_DIR

function IncludeSDKTier1()
	local refcount = IncludePackage("sourcesdk_tier1")

	local _project = project()

	externalincludedirs(current_dir .. "/../public/tier1")
	links({"tier1", "LZMA"})

	filter("system:windows")
		links({"vstdlib", "ws2_32", "rpcrt4"})

	filter({"system:linux", "architecture:x86"})
		links("vstdlib")

	filter({"system:linux", "architecture:x86_64"})
		links(_project.serverside and "vstdlib" or "vstdlib_client")

	filter("system:macosx")
		links({"vstdlib", "iconv"})

	if refcount == 1 then
		dofile(current_dir .. "/premake5_create_project.lua")
	end

	project(_project.name)
end
