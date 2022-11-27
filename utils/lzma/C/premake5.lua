local current_dir = _SCRIPT_DIR

function IncludeSDKLZMA()
	local refcount = IncludePackage("sourcesdk_lzma")

	local _project = project()

	externalincludedirs(current_dir)
	links("LZMA")

	if refcount == 1 then
		dofile(current_dir .. "/premake5_create_project.lua")
		project(_project.name)
	end
end
