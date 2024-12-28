function Dump(o)
	if type(o) == 'table' then
		local s = '{ '
		for k,v in pairs(o) do
			if type(k) ~= 'number' then k = '"'..k..'"' end
			s = s .. '['..k..'] = ' .. Dump(v) .. ','
		end
		return s .. '} '
	else
	   return tostring(o)
	end
end
 
function ScriptPath()
	local str = debug.getinfo(2, "S").source:sub(2)
	return str:match("(.*/)")
end

function SetTargetDir()
	targetdir(TargetDir .. "/%{prj.name}")
end

function SetObjectDir()
	objdir(ObjectDir .. "/%{prj.name}")
end

TargetDir = "%{wks.location}/Bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
ObjectDir = "%{wks.location}/Bin-int/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

RootDir = ScriptPath()

IncludeDir = {}
IncludeDir["MWL"]			= string.format("%s/MWL/MWL/include", RootDir)
IncludeDir["WangMauna"] 	= string.format("%s/WangMauna/WangMauna/include", RootDir)
IncludeDir["MOGL"]			= string.format("%s/MOGL/MOGL/include", RootDir)
IncludeDir["MML"]			= string.format("%s/MML/MML/include", RootDir)
IncludeDir["MUI"]			= string.format("%s/MUI/MUI/include", RootDir)
IncludeDir["MFL"]			= string.format("%s/MFL/MFL/include", RootDir)
IncludeDir["Scriptorium"]	= string.format("%s/Scriptorium/Scriptorium/include", RootDir)
IncludeDir["Monk"]			= string.format("%s/Monk/include", RootDir)