local current_dir = _SCRIPT_DIR

function IncludeSDKTier2()
	local refcount = IncludePackage("sourcesdk_tier2")

	filter({})

	externalincludedirs(current_dir .. "/../public/tier2")
	links("tier2")

	filter("system:windows")
		links("tier2")

	filter("system:macosx")
		linkoptions(current_dir .. "/../lib/public/osx32/tier2.a")

	filter("system:linux")
		linkoptions(current_dir .. "/../lib/public/linux32/tier2.a")

	filter({})

	local _project = project()

	if refcount == 1 then
		dofile(current_dir .. "/premake5_create_project.lua")
	end

	project(_project.name)
end
