local current_dir = _SCRIPT_DIR

function IncludeSDKCommon()
	local refcount = IncludePackage("sourcesdk_common")

	local _project = project()

	defines({_project.serverside and "GAME_DLL" or "CLIENT_DLL", "RAD_TELEMETRY_DISABLED", "GMOD_USE_SOURCESDK"})
	externalincludedirs({
		current_dir,
		current_dir .. "/../game/shared",
		current_dir .. "/../public"
	})

	if _project.serverside then
		externalincludedirs(current_dir .. "/../game/server")
	else
		externalincludedirs(current_dir .. "/../game/client")
	end

	IncludeSDKCommonInternal()

	links("common")
	if refcount == 1 then
		dofile(current_dir .. "/premake5_create_project.lua")
	end

	project(_project.name)
end
