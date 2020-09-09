#include "ShaderBuilder.h"
#include <logger/Logger.h>
#include <rendering/Renderer.h>
#include <platform/rendering/shaders/DirectXShaderBuilder.h>
#include <sstream>
#include <fstream>

namespace ifr
{
	std::pair<std::string, std::string> ShaderBuilder::GetDefaultShaderCode()
	{
		return DirectXShaderBuilder::GetDefaultShaderCode();
	}

	std::pair<std::string, std::string> ShaderBuilder::GetDefaultGUIShaderCode()
	{
		return DirectXShaderBuilder::GetDefaultGUIShaderCode();
	}

	std::pair<std::string, std::string> ShaderBuilder::GetDefaultAnimShaderCode()
	{
		return DirectXShaderBuilder::GetDefaultAnimShaderCode();
	}

	std::pair<std::string, std::string> ShaderBuilder::GetDefaultSkyboxShaderCode()
	{
		return DirectXShaderBuilder::GetDefaultSkyboxShaderCode();
	}

	std::pair<std::string, std::string> ShaderBuilder::GetDefaultWaterShaderCode()
	{
		return DirectXShaderBuilder::GetDefaultWaterShaderCode();
	}

	std::pair<std::string, std::string> ShaderBuilder::GetDefaultGrassShaderCode()
	{
		return DirectXShaderBuilder::GetDefaultGrassShaderCode();
	}

	std::vector<std::string> ShaderBuilder::GetTerrainShaderCode()
	{
		return DirectXShaderBuilder::GetTerrainShaderCode();
	}

	std::pair<std::string, std::string> ShaderBuilder::ReadShaderSourceFromFile(const std::string& filepath)
	{
		enum class ShaderType
		{
			NONE = -1, VERTEX = 0, PIXEL = 1
		};

		std::stringstream streams[2];

		std::ifstream file;
		try
		{
			file.open(filepath);
			
			std::string line;
			ShaderType type = ShaderType::NONE;
			
			while (getline(file, line))
			{
				if (line.find("#shader") != std::string::npos)
				{
					if (line.find("vertex") != std::string::npos)
						type = ShaderType::VERTEX;
					else if (line.find("pixel") != std::string::npos)
						type = ShaderType::PIXEL;
				}
				else
				{
					streams[(int)type] << line << "\n";
				}
			}
		}
		catch (std::ifstream::failure e)
		{
			IFR_LOG_ERROR(e.what());
			IFR_LOG_FUNC_ERROR("Failed to load shader: " + filepath);
			return { "", "" };
		}

		return { streams[0].str(), streams[1].str() };
	}

	std::vector<std::string> ShaderBuilder::ReadShaderSourceFromFileWithTesselationStage(const std::string& filepath)
	{
		enum class ShaderType
		{
			NONE = -1, VERTEX_SHADER = 0, HULL_SHADER = 1, DOMAIN_SHADER = 2, PIXEL_SHADER = 3
		};

		std::stringstream streams[4];

		std::ifstream file;
		try
		{
			file.open(filepath);

			std::string line;
			ShaderType type = ShaderType::NONE;

			while (getline(file, line))
			{
				if (line.find("#shader") != std::string::npos)
				{
					if (line.find("vertex") != std::string::npos)
						type = ShaderType::VERTEX_SHADER;
					else if (line.find("hull") != std::string::npos)
						type = ShaderType::HULL_SHADER;
					else if (line.find("domain") != std::string::npos)
						type = ShaderType::DOMAIN_SHADER;
					else if (line.find("pixel") != std::string::npos)
						type = ShaderType::PIXEL_SHADER;
				}
				else
				{
					streams[(int)type] << line << "\n";
				}
			}
		}
		catch (std::ifstream::failure e)
		{
			IFR_LOG_ERROR(e.what());
			IFR_LOG_FUNC_ERROR("Failed to load shader: " + filepath);
			return { "", "" };
		}

		return { streams[0].str(), streams[1].str(), streams[2].str(), streams[3].str() };
	}
}
