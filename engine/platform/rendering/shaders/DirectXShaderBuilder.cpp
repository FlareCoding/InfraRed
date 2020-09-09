#include "DirectXShaderBuilder.h"
#include <rendering/shaders/ShaderBuilder.h>

namespace ifr
{
	const std::string& DirectXShaderBuilder::ShadersFolder =
		"shaders\\";

	std::pair<std::string, std::string> DirectXShaderBuilder::GetDefaultShaderCode()
	{
		return ShaderBuilder::ReadShaderSourceFromFile(ShadersFolder + "Default.hlshader");
	}

	std::pair<std::string, std::string> DirectXShaderBuilder::GetDefaultGUIShaderCode()
	{
		return ShaderBuilder::ReadShaderSourceFromFile(ShadersFolder + "DefaultGUI.hlshader");
	}

	std::pair<std::string, std::string> DirectXShaderBuilder::GetDefaultAnimShaderCode()
	{
		return ShaderBuilder::ReadShaderSourceFromFile(ShadersFolder + "DefaultAnim.hlshader");
	}

	std::pair<std::string, std::string> DirectXShaderBuilder::GetDefaultSkyboxShaderCode()
	{
		return ShaderBuilder::ReadShaderSourceFromFile(ShadersFolder + "Skybox.hlshader");
	}

	std::pair<std::string, std::string> DirectXShaderBuilder::GetDefaultWaterShaderCode()
	{
		return ShaderBuilder::ReadShaderSourceFromFile(ShadersFolder + "Water.hlshader");
	}

	std::pair<std::string, std::string> DirectXShaderBuilder::GetDefaultGrassShaderCode()
	{
		return ShaderBuilder::ReadShaderSourceFromFile(ShadersFolder + "Grass.hlshader");
	}

	std::vector<std::string> DirectXShaderBuilder::GetTerrainShaderCode()
	{
		return ShaderBuilder::ReadShaderSourceFromFileWithTesselationStage(ShadersFolder + "Terrain.hlshader");
	}
}
