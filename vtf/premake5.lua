local current_dir = _SCRIPT_DIR

function IncludeSDKVTF()
	local refcount = IncludePackage("sourcesdk_vtf")

	local _project = project()

	links("vtf")

	if refcount == 1 then
		dofile(current_dir .. "/premake5_create_project.lua")
		project(_project.name)
	end
end
