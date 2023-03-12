local current_dir = _SCRIPT_DIR

function IncludeSDKMathlib()
	local refcount = IncludePackage("sourcesdk_mathlib")

	local _project = project()

	externalincludedirs(current_dir .. "/../public/mathlib")
	links("mathlib")

	if refcount == 1 then
		dofile(current_dir .. "/premake5_create_project.lua")
		project(_project.name)
	end
end