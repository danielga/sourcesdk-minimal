local current_dir = _SCRIPT_DIR

function IncludeSDKRaytrace()
	local refcount = IncludePackage("sdkraytrace")

	local _project = project()

	links("raytrace")

	if refcount == 1 then
		include(current_dir .. "/premake5_create_project.lua")
		project(_project.name)
	end
end
