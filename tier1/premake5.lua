local current_dir = _SCRIPT_DIR

function IncludeSDKTier1()
	local refcount = IncludePackage("sourcesdk_tier1")

	local _project = project()

	sysincludedirs(current_dir .. "/../public/tier1")
	links("tier1")

	filter("system:windows")
		links({"vstdlib", "ws2_32", "rpcrt4"})

	filter("system:linux")
		links(_project.serverside and "vstdlib_srv" or "vstdlib")

	filter("system:macosx")
		links({"vstdlib", "iconv"})

	if refcount == 1 then
		dofile(current_dir .. "/premake5_create_project.lua")
	end

	project(_project.name)
end
