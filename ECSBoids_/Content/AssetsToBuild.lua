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
		{ path = "Meshes/boid.mesh" },
		{ path = "Meshes/goal.mesh" },
		{ path = "Meshes/floor.mesh" },
	},
	
	effects =
	{
		{ path = "Effects/green.effect" },
		{ path = "Effects/yellow.effect" },
		{ path = "Effects/white.effect" },
	},

	prefabs =
	{
		{ path = "Prefabs/boid.prefab" },
		{ path = "Prefabs/goal.prefab" },
		{ path = "Prefabs/floor.prefab" },
	},
}
