local current_dir = _SCRIPT_DIR

function IncludeSDKEngine()
	local refcount = IncludePackage("sourcesdk_engine")

	filter({})

	externalincludedirs(current_dir .. "/../engine")
	links("engine")

	local _project = project()

	if refcount == 1 then
		dofile(current_dir .. "/premake5_create_project.lua")
	end

	project(_project.name)
end
