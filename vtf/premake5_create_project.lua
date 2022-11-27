group("SourceSDK")
	project("vtf")
		kind("StaticLib")
		warnings("Default")
		links("bitmap")
		location("../projects/" .. os.target() .. "/" .. _ACTION)
		targetdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		debugdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		objdir("!%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}/intermediate/%{prj.name}")
		defines("RAD_TELEMETRY_DISABLED")
		externalincludedirs({
			"../utils/common",
			"../public",
			"../public/tier0",
			"../public/tier1"
		})
		files({
			"cvtf.h",
			"vtf.cpp",
			"s3tc_decode.h",
			"s3tc_decode.cpp"
		})
		vpaths({["Source files/*"] = "*.cpp"})

		filter("system:windows")
			disablewarnings("4324")
			defines("WIN32")
			libdirs("../lib/public")

			filter({"system:windows", "configurations:Debug"})
				linkoptions("/NODEFAULTLIB:\"libcmt\"")

		filter("system:linux")
			disablewarnings({
				"unused-local-typedefs",
				"unused-parameter",
				"strict-aliasing",
				"unknown-pragmas",
				"undef",
				"invalid-offsetof"
			})
			defines({"COMPILER_GCC", "POSIX", "_POSIX", "LINUX", "_LINUX", "GNUC", "NO_MALLOC_OVERRIDE"})
			libdirs("../lib/public/linux32")

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
				"invalid-offsetof"
			})
			defines({"COMPILER_GCC", "POSIX", "_POSIX", "OSX", "GNUC", "NO_MALLOC_OVERRIDE"})
			libdirs("../lib/public/osx32")
