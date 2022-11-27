local current_dir = _SCRIPT_DIR

function IncludeSDKTier3()
	IncludePackage("sourcesdk_tier3")

	filter({})

	externalincludedirs(current_dir .. "/../public/tier3")

	filter("system:windows")
		links("tier3")

	filter("system:macosx")
		linkoptions(current_dir .. "/../lib/public/osx32/tier3.a")

	filter("system:linux")
		linkoptions(current_dir .. "/../lib/public/linux32/tier3.a")

	filter({})
end
