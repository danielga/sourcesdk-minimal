local current_dir = _SCRIPT_DIR

function IncludeSDKTier0()
	IncludePackage("sourcesdk_tier0")

	local _project = project()

	externalincludedirs(current_dir .. "/../public/tier0")

	filter("system:windows or macosx")
		links("tier0")

	filter("system:linux")
		links(_project.serverside and "tier0_srv" or "tier0")

	filter({})
end
