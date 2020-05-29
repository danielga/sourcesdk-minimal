local current_dir = _SCRIPT_DIR

function IncludeSDKCommon()
	IncludePackage("sourcesdk_common")

	local _project = project()

	defines({_project.serverside and "GAME_DLL" or "CLIENT_DLL", "RAD_TELEMETRY_DISABLED", "GMOD_USE_SOURCESDK"})
	sysincludedirs({
		current_dir .. "/../common",
		current_dir .. "/../game/shared",
		current_dir .. "/../public"
	})

	if _project.serverside then
		sysincludedirs(current_dir .. "/../game/server")
	else
		sysincludedirs(current_dir .. "/../game/client")
	end

	files({
		current_dir .. "/../interfaces/interfaces.cpp",
		current_dir .. "/../public/interfaces/interfaces.h"
	})
	vpaths({
		["SourceSDK"] = {
			current_dir .. "/../interfaces/interfaces.cpp",
			current_dir .. "/../public/interfaces/interfaces.h"
		}
	})

	IncludeSDKCommonInternal()
end
