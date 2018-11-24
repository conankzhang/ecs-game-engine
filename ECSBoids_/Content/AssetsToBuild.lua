--[[
	This file lists every asset that must be built by the AssetBuildSystem
]]

return
{
	shaders =
	{
		{ path = "Shaders/Vertex/vertexInputLayout.shader", arguments = { "vertex" } },
	},

	prefabs =
	{
		{ path = "Prefabs/boid.prefab" },
		{ path = "Prefabs/goal.prefab" },
		{ path = "Prefabs/floor.prefab" },
	},
}
