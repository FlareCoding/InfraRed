#pragma once
#include <core/IFRCore.h>
#include <memory>
#include <string>
#include <vector>

namespace ifr
{
	enum ShaderGraphHLSLDataType
	{
		HLSL_INT,
		HLSL_FLOAT,
		HLSL_FLOAT2,
		HLSL_FLOAT3,
		HLSL_FLOAT4,
		HLSL_MAT3,
		HLSL_MAT4,
		HLSL_TEXTURE_SAMPLER_2D,
		HLSL_TEXTURE_SAMPLER_CUBE
	};

	std::string HLSLTypeToString(ShaderGraphHLSLDataType type);
	uint32_t	HLSLTypeSize(ShaderGraphHLSLDataType type);

	class ShaderGraphNode
	{
	public:
		ShaderGraphNode(uint32_t id, ShaderGraphHLSLDataType output_t)
			: ID(id), OutputType(output_t) {}

		std::vector<Ref<ShaderGraphNode>> Inputs;
		ShaderGraphHLSLDataType OutputType;
		const uint32_t ID;

		std::string OutputName = "";

		void AddInput(const Ref<ShaderGraphNode>& node) { Inputs.push_back(node); }

		virtual std::string GetShaderPassCode() const = 0;
		virtual std::string GetShaderGlobalCode() const { return ""; }
		virtual bool IsUniform() const { return false; }
	};

	class SGCustomNode : public ShaderGraphNode
	{
	public:
		SGCustomNode(uint32_t id, ShaderGraphHLSLDataType output_t) : ShaderGraphNode(id, output_t) {}

		void SetShaderPassCode(const std::string& code) { m_ShaderPassCode = code; }
		void SetShaderGlobalCode(const std::string& code) { m_ShaderGlobalCode = code; }

		// Inherited via ShaderGraphNode
		virtual std::string GetShaderPassCode() const override { return m_ShaderPassCode; }
		virtual std::string GetShaderGlobalCode() const override { return m_ShaderGlobalCode; }

	private:
		std::string m_ShaderPassCode = "";
		std::string m_ShaderGlobalCode = "";
	};

	class SGUniformValueNode : public ShaderGraphNode
	{
	public:
		SGUniformValueNode(uint32_t id, ShaderGraphHLSLDataType output_t) : ShaderGraphNode(id, output_t) {}

		// Inherited via ShaderGraphNode
		virtual std::string GetShaderPassCode() const override;
		virtual bool IsUniform() const override { return true; }
	};
}
