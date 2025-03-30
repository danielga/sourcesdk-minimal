local current_dir = _SCRIPT_DIR

function IncludeSDKTier3()
	IncludePackage("sourcesdk_tier3")

	local _project = project()
	print("WARNING: Project '" .. _project.name .. "' included Source SDK 'tier3' library, which is currently not available in x86-64.")

	filter("architecture:x86")
		externalincludedirs(current_dir .. "/../public/tier3")

		filter({"architecture:x86", "system:windows"})
			links("tier3")

		filter({"architecture:x86", "system:macosx"})
			linkoptions(path.getabsolute(current_dir) .. "/../lib/public/osx32/tier3.a")

		filter({"architecture:x86", "system:linux"})
			linkoptions(path.getabsolute(current_dir) .. "/../lib/public/linux32/tier3.a")

	filter({})
end
