-- lua.lua

premake.modules.lua = {}
local m = premake.modules.lua
local p = premake

newaction {
	trigger = "smart_arguments",
	description = "Generate Smart Command Line Arguments for projects.",

	onProject = function(prj)
		if string.find(prj.name, "_ut") then
			p.generate(prj, prj.name .. ".args.json", m.ArgumentsForUT)
		end
	end,
}

function m.ArgumentsForUT(prj)
	p.w('{')
	p.w('	"FileVersion": 2,')
	p.w('	"Id": "1611723482f0f0cf8b087ca6f7665731",')
	p.w('	"Items": [')
	p.w('		{')
	p.w('			"Id": "54a79fe785234c82b842b6cfde546d62",')
	p.w('			"Command": "--wait-for-keypress exit"')
	p.w('		},')
	p.w('	]')
	p.w('}')
end

return m