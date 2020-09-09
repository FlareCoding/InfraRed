#include "ShaderGraphNode.h"

namespace ifr
{
	std::string HLSLTypeToString(ShaderGraphHLSLDataType type)
	{
		switch (type)
		{
		case HLSL_INT:		return "int";
		case HLSL_FLOAT:	return "float";
		case HLSL_FLOAT2:	return "float2";
		case HLSL_FLOAT3:	return "float3";
		case HLSL_FLOAT4:	return "float4";
		case HLSL_MAT3:		return "float3x3";
		case HLSL_MAT4:		return "float4x4";
		default:			return "InvalidType";
		}
	}

	uint32_t HLSLTypeSize(ShaderGraphHLSLDataType type)
	{
		switch (type)
		{
		case HLSL_INT:		return 4;
		case HLSL_FLOAT:	return 4;
		case HLSL_FLOAT2:	return 8;
		case HLSL_FLOAT3:	return 12;
		case HLSL_FLOAT4:	return 16;
		case HLSL_MAT3:		return 36;
		case HLSL_MAT4:		return 64;
		default:			return 0;
		}
	}
	
	std::string SGUniformValueNode::GetShaderPassCode() const { return ""; }
}
