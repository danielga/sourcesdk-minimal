group("SourceSDK")
	project("mathlib")
		kind("StaticLib")
		warnings("Default")
		location("../projects/" .. os.target() .. "/" .. _ACTION)
		targetdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		debugdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		objdir("!%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}/intermediate/%{prj.name}")
		defines({"RAD_TELEMETRY_DISABLED", "MATHLIB_LIB"})
		externalincludedirs({
			"../public",
			"../public/mathlib",
			"../public/tier0"
		})
		files({
			"color_conversion.cpp",
			"halton.cpp",
			"lightdesc.cpp",
			"mathlib_base.cpp",
			"powsse.cpp",
			"sparse_convolution_noise.cpp",
			"sseconst.cpp",
			"sse.cpp",
			"ssenoise.cpp",
			"anorms.cpp",
			"bumpvects.cpp",
			"IceKey.cpp",
			"imagequant.cpp",
			"polyhedron.cpp",
			"quantize.cpp",
			"randsse.cpp",
			"spherical.cpp",
			"simdvectormatrix.cpp",
			"vector.cpp",
			"vmatrix.cpp",
			"almostequal.cpp"
		})
		vpaths({["Source files/*"] = "*.cpp"})

		filter("system:windows or linux")
			files("3dnow.cpp")

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
				"ignored-attributes",
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
