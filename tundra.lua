Build {
	Env = {
		CPPPATH = {
			"./include"
		},
	},
	Units = function()
		Program { 
			Name = "signals",
			Config = "*",
			Sources = {
				"tests/signal.cpp"
			}
		}
		Program {
			Name = "bitset",
			Config = "*",
			Sources = {
				"tests/bitset.cpp"
			}
		}
		Default "signals"
	end,
	Configs = {
		{
			Name = "win64-msvc",
			DefaultOnHost = "windows",
			Tools = { { "msvc"; TargetArch = "x64" } },
			Env = {
				CPPDEFS = { "WIN32" },
				CXXOPTS = { 
					"/W4", 
					"/EHsc",
					{ "/MTd", Config = "*-*-debug" },
					"/FS"
				},
				GENERATE_PDB = "1"
			}
		},
	},

	Variants = { "debug", "release" }
}