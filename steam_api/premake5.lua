local current_dir = _SCRIPT_DIR

function IncludeSteamAPI()
	IncludePackage("steam_api")
	filter({})
	externalincludedirs(current_dir .. "/../public/steam")
	links("steam_api")
end
