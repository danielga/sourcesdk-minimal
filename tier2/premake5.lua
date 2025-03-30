local current_dir = _SCRIPT_DIR

function IncludeSDKTier2()
	local refcount = IncludePackage("sourcesdk_tier2")

	local _project = project()
	print("WARNING: Project '" .. _project.name .. "' included Source SDK 'tier2' library, which is currently not available in x86-64.")

	externalincludedirs(current_dir .. "/../public/tier2")
	links("tier2")

	filter({})

	local _project = project()

	if refcount == 1 then
		dofile(current_dir .. "/premake5_create_project.lua")
	end

	project(_project.name)
end
