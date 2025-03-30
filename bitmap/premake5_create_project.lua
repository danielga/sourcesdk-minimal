group("SourceSDK")
	project("bitmap")
		kind("StaticLib")
		warnings("Default")
		location("../projects/" .. os.target() .. "/" .. _ACTION)
		targetdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		debugdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		objdir("!%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}/intermediate/%{prj.name}")
		externalincludedirs({"../utils/common", "../public", "../public/tier0", "../public/tier1"})
		files({
			"bitmap.cpp",
			"colorconversion.cpp",
			"floatbitmap.cpp",
			"floatbitmap2.cpp",
			"floatbitmap3.cpp",
			"floatbitmap_bilateralfilter.cpp",
			"floatcubemap.cpp",
			"imageformat.cpp",
			"psd.cpp",
			"psheet.cpp",
			"resample.cpp",
			"tgaloader.cpp",
			"texturepacker.cpp",
			"tgawriter.cpp",
			"../public/bitmap/bitmap.h",
			"../public/bitmap/floatbitmap.h",
			"../public/bitmap/imageformat.h",
			"../public/bitmap/imageformat_declarations.h",
			"../public/bitmap/psd.h",
			"../public/bitmap/psheet.h",
			"../public/bitmap/texturepacker.h",
			"../public/bitmap/tgaloader.h",
			"../public/bitmap/tgawriter.h",
			"../public/bitmap/stb_dxt.h"
		})
		vpaths({
			["Source files/*"] = "*.cpp",
			["Header files/*"] = "../public/bitmap/*.h"
		})

		IncludeSDKCommonInternal()

		filter("system:windows")
			files("floatbitmap4.cpp")
