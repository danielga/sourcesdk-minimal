group("SourceSDK")
	project("engine")
		kind("StaticLib")
		warnings("Default")
		location("../projects/" .. os.target() .. "/" .. _ACTION)
		targetdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		debugdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		objdir("!%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}/intermediate/%{prj.name}")
		defines({"RAD_TELEMETRY_DISABLED", "ENGINE_STATIC_LIB"})
		externalincludedirs({
			"../public",
			"../common",
			"../public/tier0",
			"../public/tier1",
			"../public/tier2",
			"../engine"
		})
		files({
			"precache.cpp",
		})
		vpaths({["Source files/*"] = "*.cpp"})

		filter("system:windows")
			disablewarnings("4324")
			defines({"_DLL_EXT=.dll", "WIN32"})
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
				"unused-result",
				"invalid-offsetof"
			})
			defines({"_DLL_EXT=.so", "COMPILER_GCC", "POSIX", "_POSIX", "LINUX", "_LINUX", "GNUC", "NO_MALLOC_OVERRIDE"})
			libdirs("../lib/public/linux32")
			linkoptions({
				"-Xlinker --wrap=fopen",
				"-Xlinker --wrap=freopen",
				"-Xlinker --wrap=open",
				"-Xlinker --wrap=creat",
				"-Xlinker --wrap=access",
				"-Xlinker --wrap=__xstat",
				"-Xlinker --wrap=stat",
				"-Xlinker --wrap=lstat",
				"-Xlinker --wrap=fopen64",
				"-Xlinker --wrap=open64",
				"-Xlinker --wrap=opendir",
				"-Xlinker --wrap=__lxstat",
				"-Xlinker --wrap=chmod",
				"-Xlinker --wrap=chown",
				"-Xlinker --wrap=lchown",
				"-Xlinker --wrap=symlink",
				"-Xlinker --wrap=link",
				"-Xlinker --wrap=__lxstat64",
				"-Xlinker --wrap=mknod",
				"-Xlinker --wrap=utimes",
				"-Xlinker --wrap=unlink",
				"-Xlinker --wrap=rename",
				"-Xlinker --wrap=utime",
				"-Xlinker --wrap=__xstat64",
				"-Xlinker --wrap=mount",
				"-Xlinker --wrap=mkfifo",
				"-Xlinker --wrap=mkdir",
				"-Xlinker --wrap=rmdir",
				"-Xlinker --wrap=scandir",
				"-Xlinker --wrap=realpath"
			})

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
			defines({"_DLL_EXT=.dylib", "COMPILER_GCC", "POSIX", "_POSIX", "OSX", "GNUC", "NO_MALLOC_OVERRIDE"})
			files("processor_detect_linux.cpp")
			libdirs("../lib/public/osx32")
