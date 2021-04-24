group("SourceSDK")
	project("LZMA")
		kind("StaticLib")
		warnings("Default")
		defines("_7ZIP_ST")
		location("../../../projects/" .. os.target() .. "/" .. _ACTION)
		targetdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		debugdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		objdir("!%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}/intermediate/%{prj.name}")
		files({
			"7zAlloc.c",
			"7zArcIn.c",
			"7zBuf.c",
			"7zBuf2.c",
			"7zCrc.c",
			"7zCrcOpt.c",
			"7zDec.c",
			"7zFile.c",
			"7zStream.c",
			"Aes.c",
			"AesOpt.c",
			"Alloc.c",
			"Bcj2.c",
			"Bcj2Enc.c",
			"Bra.c",
			"Bra86.c",
			"BraIA64.c",
			"CpuArch.c",
			"Delta.c",
			"DllSecur.c",
			"LzFind.c",
			"Lzma2Dec.c",
			"Lzma2Enc.c",
			"Lzma86Dec.c",
			"Lzma86Enc.c",
			"LzmaDec.c",
			"LzmaEnc.c",
			"LzmaLib.c",
			"Ppmd7.c",
			"Ppmd7Dec.c",
			"Ppmd7Enc.c",
			"Sha256.c",
			"Sort.c",
			"Xz.c",
			"XzCrc64.c",
			"XzCrc64Opt.c",
			"XzDec.c",
			"XzEnc.c",
			"XzIn.c"
		})
		vpaths({
			["Header files/*"] = "*.h",
			["Source files/*"] = "*.c",
		})

		filter("system:windows")
			files({"LzFindMt.c", "Lzma2DecMt.c", "MtCoder.c", "MtDec.c", "Threads.c"})
