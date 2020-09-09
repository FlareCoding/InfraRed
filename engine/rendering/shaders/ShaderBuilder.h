#pragma once
#include <core/IFRCore.h>
#include <string>
#include <vector>

namespace ifr
{
	class ShaderBuilder
	{
	public:
		static std::pair<std::string, std::string> GetDefaultShaderCode();
		static std::pair<std::string, std::string> GetDefaultGUIShaderCode();
		static std::pair<std::string, std::string> GetDefaultAnimShaderCode();
		static std::pair<std::string, std::string> GetDefaultSkyboxShaderCode();
		static std::pair<std::string, std::string> GetDefaultWaterShaderCode();
		static std::pair<std::string, std::string> GetDefaultGrassShaderCode();

		static std::vector<std::string> GetTerrainShaderCode();

		static std::pair<std::string, std::string> ReadShaderSourceFromFile(const std::string& filepath);
		static std::vector<std::string> ReadShaderSourceFromFileWithTesselationStage(const std::string& filepath);
	};
}
