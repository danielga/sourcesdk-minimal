group("SourceSDK")
	project("tier2")
		kind("StaticLib")
		warnings("Default")
		location("../projects/" .. os.target() .. "/" .. _ACTION)
		targetdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		debugdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		objdir("!%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}/intermediate/%{prj.name}")
		defines({"RAD_TELEMETRY_DISABLED", "TIER2_STATIC_LIB"})
		externalincludedirs({
			"../public",
			"../public/tier0",
			"../public/tier1",
			"../public/tier2",
			"../common"
		})
		files({"tier2.cpp"})
		vpaths({["Source files/*"] = "*.cpp"})

		IncludeSDKCommonInternal()
		IncludeSDKLZMA()
