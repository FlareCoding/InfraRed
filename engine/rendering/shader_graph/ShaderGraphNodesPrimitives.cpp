#include "ShaderGraphNodesPrimitives.h"

namespace ifr
{
	SGConstantNode::SGConstantNode(uint32_t id, ShaderGraphHLSLDataType output_t) : ShaderGraphNode(id, output_t) {}

	std::string SGConstantNode::GetShaderPassCode() const
	{
		std::string OutputTypeStr = HLSLTypeToString(OutputType);

		return OutputTypeStr + " " + OutputName + " = " + OutputValue + ";";
	}

	SGFloat2Node::SGFloat2Node(uint32_t id) : ShaderGraphNode(id, HLSL_FLOAT2) {}

	std::string SGFloat2Node::GetShaderPassCode() const
	{
		std::string params = "";
		for (size_t i = 0; i < Inputs.size(); i++)
		{
			params += Inputs[i]->OutputName;

			if (i < Inputs.size() - 1)
				params += ", ";
		}

		return "float2 " + OutputName + " = float2(" + params + ");";
	}

	SGFloat3Node::SGFloat3Node(uint32_t id) : ShaderGraphNode(id, HLSL_FLOAT3) {}

	std::string SGFloat3Node::GetShaderPassCode() const
	{
		std::string params = "";
		for (size_t i = 0; i < Inputs.size(); i++)
		{
			params += Inputs[i]->OutputName;

			if (i < Inputs.size() - 1)
				params += ", ";
		}

		return "float3 " + OutputName + " = float3(" + params + ");";
	}

	SGFloat4Node::SGFloat4Node(uint32_t id) : ShaderGraphNode(id, HLSL_FLOAT4) {}

	std::string SGFloat4Node::GetShaderPassCode() const
	{
		std::string params = "";
		for (size_t i = 0; i < Inputs.size(); i++)
		{
			params += Inputs[i]->OutputName;

			if (i < Inputs.size() - 1)
				params += ", ";
		}

		return "float4 " + OutputName + " = float4(" + params + ");";
	}

	SGTextureSampler2DNode::SGTextureSampler2DNode(uint32_t id) : ShaderGraphNode(id, HLSL_FLOAT4) 
	{
	}

	std::string SGTextureSampler2DNode::GetShaderPassCode() const
	{
		if (Inputs.size() != 1 || !Inputs[0])
		{
			IFR_LOG_ERROR("SGTextureSampler2DNode> Invalid Parameter Count");
			return "";
		}

		std::string SamplerName = TextureName + "Sampler";

		if (!CoordinateCorrectionEnabled)
			return "float4 " + OutputName + " = " + TextureName + ".Sample(" + SamplerName + ", " + Inputs[0]->OutputName + ");";
		else
			return "float4 " + OutputName + " = 2.0f * (" + TextureName + ".Sample(" + SamplerName + ", " + Inputs[0]->OutputName + ")) - 1.0f;";
	}

	std::string SGTextureSampler2DNode::GetShaderGlobalCode() const
	{
		std::string slot = std::to_string(TextureSlot);
		std::string texture_decl = "Texture2D " + TextureName + " : TEXTURE: register(t" + slot + ");\n";
		std::string sampler_decl = "SamplerState " + TextureName + "Sampler : SAMPLER: register(s" + slot + ");\n";

		return texture_decl + sampler_decl;
	}
}
