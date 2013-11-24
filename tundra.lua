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
		Default "signals"
	end,
	Configs = {
		{
			Name = "win64-msvc",
			DefaultOnHost = "windows",
			Tools = { { "msvc"; TargetArch = "x64" } },
			Env = {
				CXXOPTS = { 
					"/W4", 
					"/EHsc",
					{ "/MTd", Config = "*-*-debug" }
				}
			}
		},
	},
}