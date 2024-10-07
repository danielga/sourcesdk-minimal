group("SourceSDK")
	project("tier1")
		kind("StaticLib")
		warnings("Default")
		location("../projects/" .. os.target() .. "/" .. _ACTION)
		targetdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		debugdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		objdir("!%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}/intermediate/%{prj.name}")
		defines({"RAD_TELEMETRY_DISABLED", "TIER1_STATIC_LIB"})
		externalincludedirs({
			"../public",
			"../public/tier0",
			"../public/tier1"
		})
		files({
			"bitbuf.cpp",
			"byteswap.cpp",
			"characterset.cpp",
			"checksum_crc.cpp",
			"checksum_md5.cpp",
			"checksum_sha1.cpp",
			"commandbuffer.cpp",
			"convar.cpp",
			"datamanager.cpp",
			"diff.cpp",
			"generichash.cpp",
			"ilocalize.cpp",
			"interface.cpp",
			"keyvalues.cpp",
			"kvpacker.cpp",
			"lzmaDecoder.cpp",
			"mempool.cpp",
			"memstack.cpp",
			"NetAdr.cpp",
			"splitstring.cpp",
			"rangecheckedvar.cpp",
			"reliabletimer.cpp",
			"stringpool.cpp",
			"strtools.cpp",
			"strtools_unicode.cpp",
			"tier1.cpp",
			"tokenreader.cpp",
			"sparsematrix.cpp",
			"uniqueid.cpp",
			"utlbuffer.cpp",
			"utlbufferutil.cpp",
			"utlstring.cpp",
			"utlsymbol.cpp",
			"utlbinaryblock.cpp",
			"snappy.cpp",
			"snappy-sinksource.cpp",
			"snappy-stubs-internal.cpp"
		})
		vpaths({["Source files/*"] = "*.cpp"})

		IncludeSDKLZMA()

		filter("system:windows")
			disablewarnings("4324")
			defines({"_DLL_EXT=.dll", "WIN32"})
			files("processor_detect.cpp")
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
			files({
				"processor_detect_linux.cpp",
				"qsort_s.cpp",
				"pathmatch.cpp"
			})
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
