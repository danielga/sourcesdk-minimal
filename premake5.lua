local current_dir = _SCRIPT_DIR

function IncludeSDKCommonInternal()
	filter({})

	defines({
		"RAD_TELEMETRY_DISABLED",
		"NO_STRING_T",
		"VECTOR",
		"VERSION_SAFE_STEAM_API_INTERFACES",
		"PROTECTED_THINGS_ENABLE"
	})

	filter("system:windows")
		defines({"_DLL_EXT=.dll", "WIN32", "COMPILER_MSVC"})
		disablewarnings("4324")

		filter({"system:windows", "architecture:x86"})
			defines("COMPILER_MSVC32")
			libdirs(current_dir .. "/lib/public")

		filter({"system:windows", "architecture:x86_64"})
			defines({"COMPILER_MSVC64", "PLATFORM_64BITS", "WIN64", "_WIN64"})
			libdirs(current_dir .. "/lib/public/x64")

		filter({"system:windows", "configurations:Debug"})
			linkoptions("/NODEFAULTLIB:\"libcmt\"")

	filter("system:linux")
		disablewarnings({
			"unused-local-typedefs",
			"unused-parameter",
			"strict-aliasing",
			"unknown-pragmas",
			"undef",
			"ignored-attributes"
		})
		defines({
			"_DLL_EXT=.so",
			"COMPILER_GCC",
			"POSIX",
			"_POSIX",
			"LINUX",
			"_LINUX",
			"GNUC",
			"SWDS"
		})

		filter({"system:linux", "architecture:x86"})
			libdirs(path.getabsolute(current_dir) .. "/lib/public/linux32")

		filter({"system:linux", "architecture:x86_64"})
			defines("PLATFORM_64BITS")
			libdirs(path.getabsolute(current_dir) .. "/lib/public/linux64")

	filter("system:macosx")
		disablewarnings({
			"unused-local-typedef",
			"unused-parameter",
			"unused-private-field",
			"overloaded-virtual",
			"unknown-pragmas",
			"unused-variable",
			"unknown-warning-option",
			"undef",
			"expansion-to-defined"
		})
		defines({
			"_DLL_EXT=.dylib",
			"COMPILER_GCC",
			"POSIX",
			"_POSIX",
			"OSX",
			"_OSX",
			"GNUC",
			"_DARWIN_UNLIMITED_SELECT",
			"FD_SETSIZE=10240",
			"OVERRIDE_V_DEFINES",
			"SWDS"
		})

		filter({"system:macosx", "architecture:x86"})
			libdirs(path.getabsolute(current_dir) .. "/lib/public/osx32")

		filter({"system:macosx", "architecture:x86_64"})
			defines("PLATFORM_64BITS")
			libdirs(path.getabsolute(current_dir) .. "/lib/public/osx64")

		filter({"system:linux or macosx", "language:C++"})
			disablewarnings("invalid-offsetof")

	filter({})
end

include("common")
include("tier0")
include("tier1")
include("tier2")
include("tier3")
include("mathlib")
include("raytrace")
include("bitmap")
include("vtf")
include("steam_api")
include("utils/lzma/C")
