--[[
	This file lists every asset that must be built by the AssetBuildSystem
]]

return
{
	shaders =
	{
		{ path = "Shaders/Vertex/vertexInputLayout.shader", arguments = { "vertex" } },
	},

	meshes =
	{
		{ path = "Meshes/cube.mesh" },
		{ path = "Meshes/plane.mesh" },
		{ path = "Meshes/pyramid.mesh" },
	},
	
	effects =
	{
		{ path = "Effects/red.effect" },
		{ path = "Effects/white.effect" },
	},
}
