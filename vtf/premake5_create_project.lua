group("SourceSDK")
	project("vtf")
		kind("StaticLib")
		warnings("Default")
		links("bitmap")
		location("../projects/" .. os.target() .. "/" .. _ACTION)
		targetdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		debugdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		objdir("!%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}/intermediate/%{prj.name}")
		externalincludedirs({"../utils/common", "../public", "../public/tier0", "../public/tier1"})
		files({"vtf.cpp", "convert_x360.cpp", "cvtf.h", "../public/vtf/vtf.h"})
		vpaths({
			["Source files/*"] = "*.cpp",
			["Header files/*"] = {"*.h", "../public/vtf/*.h"}
		})

		IncludeSDKCommonInternal()

		filter("system:windows")
			files({"s3tc_decode.cpp", "s3tc_decode.h"})
