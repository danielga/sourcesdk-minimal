local current_dir = _SCRIPT_DIR

function IncludeSDKBitmap()
	local refcount = IncludePackage("sourcesdk_bitmap")

	local _project = project()

	links("bitmap")

	if refcount == 1 then
		dofile(current_dir .. "/premake5_create_project.lua")
		project(_project.name)
	end
end
