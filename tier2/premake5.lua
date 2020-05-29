local current_dir = _SCRIPT_DIR

function IncludeSDKTier2()
	IncludePackage("sourcesdk_tier2")

	local _project = project()
	print("WARNING: Project '" .. _project.name .. "' included Source SDK 'tier2' library, which is currently not available in x86-64.")

	filter("architecture:x86")
		externalincludedirs(current_dir .. "/../public/tier2")

		filter({"architecture:x86", "system:windows"})
			links("tier2")

		filter({"architecture:x86", "system:macosx"})
			linkoptions(path.getabsolute(current_dir) .. "/../lib/public/osx32/tier2.a")

		filter({"architecture:x86", "system:linux"})
			linkoptions(path.getabsolute(current_dir) .. "/../lib/public/linux32/tier2.a")

	filter({})
end
