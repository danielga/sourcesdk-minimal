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

	filter("system:windows")
		defines("WIN32")
		disablewarnings("4324")
		libdirs(current_dir .. "/../lib/public")

		filter({"system:windows", "configurations:Debug"})
			linkoptions("/NODEFAULTLIB:\"libcmt\"")

	filter("system:linux")
		disablewarnings({
			"unused-local-typedefs",
			"unused-parameter",
			"strict-aliasing",
			"unknown-pragmas",
			"undef"
		})
		defines({"COMPILER_GCC", "POSIX", "_POSIX", "LINUX", "_LINUX", "GNUC", "NO_MALLOC_OVERRIDE"})
		libdirs(current_dir .. "/../lib/public/linux32")

		filter({"system:linux", "files:**.cpp or **.cxx"})
			disablewarnings("invalid-offsetof")

	filter("system:macosx")
		disablewarnings({
			"unused-local-typedef",
			"unused-parameter",
			"unused-private-field",
			"overloaded-virtual",
			"unknown-pragmas",
			"unused-variable",
			"unknown-warning-option",
			"undef"
		})
		defines({"COMPILER_GCC", "POSIX", "_POSIX", "OSX", "GNUC", "NO_MALLOC_OVERRIDE"})
		libdirs(current_dir .. "/../lib/public/osx32")

		filter({"system:macosx", "files:**.cpp or **.cxx"})
			disablewarnings("invalid-offsetof")

	filter({})
end
