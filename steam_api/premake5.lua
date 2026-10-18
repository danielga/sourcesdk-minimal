local current_dir = _SCRIPT_DIR

function IncludeSteamAPI()
	IncludePackage("steam_api")
	filter({})
	externalincludedirs(current_dir .. "/../public/steam")

	filter({"platforms:x86"})
		links("steam_api")

	filter({"platforms:x86_64"})
		links("steam_api64")
end
