#include "ShaderGraph.h"

namespace ifr
{
	ShaderGraph::ShaderGraph()
	{
		CreateInputUVNode();
		CreateInputTimeNode();
	}

	uint32_t ShaderGraph::CreateNodeID()
	{
		++m_AvailableNodeID;
		m_NodeDeclarationMap[m_AvailableNodeID] = false;
		return m_AvailableNodeID;
	}

	void ShaderGraph::ConnectBaseColorInput(const Ref<ShaderGraphNode>& node)
	{
		if (node->OutputType != HLSL_FLOAT4)
		{
			IFR_LOG_ERROR("NodeGraph> BaseColor Input does not have output type of FLOAT4");
			return;
		}

		m_BaseColorInputNode = node;
	}

	void ShaderGraph::CreateInputUVNode()
	{
		m_InputUVNode = MakeRef<SGCustomNode>(CreateNodeID(), HLSL_FLOAT2);
		m_InputUVNode->OutputName = "input.UV";
	}

	void ShaderGraph::CreateInputTimeNode()
	{
		m_InputTimeNode = MakeRef<SGCustomNode>(CreateNodeID(), HLSL_FLOAT);
		m_InputTimeNode->OutputName = "u_Time";
	}

	const Ref<SGCustomNode> ShaderGraph::GetInputUVNode()
	{
		return m_InputUVNode;
	}

	const Ref<SGCustomNode> ShaderGraph::GetInputTimeNode()
	{
		return m_InputTimeNode;
	}

	void ShaderGraph::ShouldUseNormalMap(bool bUse, uint32_t slot)
	{
		m_ShouldUseNormalMap = bUse;
		m_NormalMapSlot = slot;
	}

	void ShaderGraph::ShouldUseEnvironmentMap(bool bUse, uint32_t slot)
	{
		m_ShouldUseEnvironmentMap = bUse;
		m_EnvironmentMapSlot = slot;
	}

	//----------------------------------------------------------------------------------------------------------//
	//----------------------------------------------------------------------------------------------------------//
	//----------------------------------------------------------------------------------------------------------//

	void ShaderGraph::AddNodeHLSLSourceToCodeStream(std::string& src, const Ref<ShaderGraphNode>& node)
	{
		if (m_NodeDeclarationMap.find(node->ID) != m_NodeDeclarationMap.end())
		{
			if (!m_NodeDeclarationMap[node->ID])
			{
				// Adding global declarations if needed
				auto global_code = node->GetShaderGlobalCode();
				if (!global_code.empty())
					GlobalDeclarationsCode += global_code + "\n";

				// Adding uniform declarations if needed
				if (node->IsUniform())
				{
					UniformDeclarationsCode += (HLSLTypeToString(node->OutputType) + " " + node->OutputName + ";\n");
					MaterialUniforms.push_back(UniformVariable(node->OutputName, HLSLTypeSize(node->OutputType)));
				}

				src = node->GetShaderPassCode() + "\n" + src;
				m_NodeDeclarationMap[node->ID] = true;
			}
		}

		for (auto& input : node->Inputs)
			AddNodeHLSLSourceToCodeStream(src, input);
	}

	void ShaderGraph::GenerateHLSLCode()
	{
		// Base Color Pass
		if (m_BaseColorInputNode)
		{
			BaseColorCode = "float4 BaseColor = " + m_BaseColorInputNode->OutputName + ";\n";
			AddNodeHLSLSourceToCodeStream(BaseColorCode, m_BaseColorInputNode);
		}
		else
		{
			BaseColorCode = "float4 BaseColor = float4(0.5f, 0.5f, 0.5f, 1.0f);\n";
		}

		if (m_ShouldUseNormalMap)
		{
			std::string slot_str = std::to_string(m_NormalMapSlot);
			GlobalDeclarationsCode += "Texture2D _NormalMapTexture_ : TEXTURE: register(t" + slot_str  + ");\nSamplerState _NormalMapSampler_ : SAMPLER: register(s" + slot_str + ");\n";

			BaseNormalCode =  "float4 NormalMapValue = 2.0f * _NormalMapTexture_.Sample(_NormalMapSampler_, input.UV) - 1.0f;\n";
			BaseNormalCode += "float3 BaseNormal = normalize(mul(input.TBNMatrix, NormalMapValue.xyz));\n";
		}
		else
		{
			BaseNormalCode = "float3 BaseNormal = input.WorldNormal;\n";
		}

		if (m_ShouldUseEnvironmentMap)
		{
			std::string slot_str = std::to_string(m_EnvironmentMapSlot);
			GlobalDeclarationsCode += "TextureCube _EnvironmentMapTexture_ : TEXTURE: register(t" + slot_str + ");\nSamplerState _EnvironmentMapSampler_ : SAMPLER: register(s" + slot_str + ");\n";
		}

		// Basic material uniforms
		UniformDeclarationsCode += (HLSLTypeToString(HLSL_FLOAT) + " u_Roughness;\n");
		MaterialUniforms.push_back(UniformVariable("u_Roughness", HLSLTypeSize(HLSL_FLOAT)));
	}

	std::string ShaderGraph::GetVertexShaderHLSLSource(ShaderGraphMeshType mesh_type) const
	{
		if (mesh_type == MESH_STATIC)
		{
			// Static Mesh
			return
				R"(
cbuffer ModelViewProjectionBuffer : register(b0)
{
	float4x4 Transform;
	float4x4 Projection;
	float4x4 View;
};

cbuffer VSBuffer : register(b1)
{
	float3 u_LightPosition;
	float  u_Padding00;
	float3 u_CameraPosition;
	float  u_Padding01;
	float4 u_ClipPlane;
};

struct VS_INPUT
{
	float3 Position : POSITION;
	float2 UV       : UV;
	float3 Normal   : NORMAL;
	float3 Tangent  : TANGENT;
	float3 Binormal : TANGENT;
};

struct VS_OUTPUT
{
	float4 Position         : SV_POSITION;
	float2 UV               : UV1;
	float3 WorldNormal      : COLOR1;
	float3 WorldTangent     : TANGENT1;
	float3 WorldBinormal    : BINORMAL1;
	float3 ToLightVector    : COLOR2;
	float3 ViewVector       : TEXCOORD1;
	float3 ReflectedVector  : TEXCOORD2;
	float3x3 TBNMatrix      : TEXCOORD3;
	float  ClipDistance     : SV_ClipDistance0;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	float4 WorldPosition        = mul(Transform, float4(input.Position, 1.0f));
	float4 ModelViewPosition    = mul(View, WorldPosition);
	float4 FinalPosition        = mul(Projection, ModelViewPosition);

	output.Position         = FinalPosition;

	output.UV               = input.UV;
	output.WorldNormal      = mul(Transform, float4(input.Normal, 0.0f)).xyz;
	output.WorldTangent     = mul(Transform, float4(input.Tangent, 0.0f)).xyz;
	output.WorldBinormal    = mul(Transform, float4(input.Binormal, 0.0f)).xyz;
	output.ToLightVector    = u_LightPosition - WorldPosition.xyz;
	output.ViewVector       = u_CameraPosition - WorldPosition.xyz;
	output.ReflectedVector  = reflect(normalize(WorldPosition.xyz - u_CameraPosition), normalize(output.WorldNormal));
	
	float3 n = normalize(output.WorldNormal);
	float3 t = normalize(output.WorldTangent);
	float3 b = normalize(output.WorldBinormal);

	output.TBNMatrix = float3x3(t, b, n);

	output.ClipDistance = dot(WorldPosition, u_ClipPlane);

	return output;
}

)";
		}
		else
		{
			// Animated Mesh
			return
				R"(
cbuffer ModelViewProjectionBuffer : register(b0)
{
	float4x4 Transform;
	float4x4 Projection;
	float4x4 View;
};

cbuffer VSBuffer : register(b1)
{
	float3 u_LightPosition;
	float  u_Padding00;
	float3 u_CameraPosition;
	float  u_Padding01;
	float4 u_ClipPlane;
};

cbuffer BoneTransformsBuffer : register(b6)
{
	float4x4 u_BoneTransforms[80];
};

struct VS_INPUT
{
	float3 Position : POSITION;
	float2 UV       : UV;
	float3 Normal   : NORMAL;
	float3 Tangent  : TANGENT;
	float3 Binormal : TANGENT;
	int4   BoneIDs  : BONE_IDS;
	float4 BoneWeights : BONE_WEIGHTS;
};

struct VS_OUTPUT
{
	float4 Position         : SV_POSITION;
	float2 UV               : UV1;
	float3 WorldNormal      : COLOR1;
	float3 WorldTangent     : TANGENT1;
	float3 WorldBinormal    : BINORMAL1;
	float3 ToLightVector    : COLOR2;
	float3 ViewVector       : TEXCOORD1;
	float3 ReflectedVector  : TEXCOORD2;
	float3x3 TBNMatrix      : TEXCOORD3;
	float  ClipDistance     : SV_ClipDistance0;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	float4x4 BoneTransform   = u_BoneTransforms[input.BoneIDs[0]] * input.BoneWeights[0];
    BoneTransform			+= u_BoneTransforms[input.BoneIDs[1]] * input.BoneWeights[1];
    BoneTransform			+= u_BoneTransforms[input.BoneIDs[2]] * input.BoneWeights[2];
	BoneTransform			+= u_BoneTransforms[input.BoneIDs[3]] * input.BoneWeights[3];
	BoneTransform			+= u_BoneTransforms[input.BoneIDs[3]] * input.BoneWeights[3];

	float4 TotalLocalPosition = mul(BoneTransform, float4(input.Position, 1.0f));
	float4 TotalLocalNormal   = float4(mul((float3x3)BoneTransform, input.Normal), 0.0);

	float4 WorldPosition        = mul(Transform, TotalLocalPosition);
	float4 ModelViewPosition    = mul(View, WorldPosition);
	float4 FinalPosition        = mul(Projection, ModelViewPosition);

	output.Position         = FinalPosition;

	output.UV               = input.UV;
	output.WorldNormal      = mul(Transform, TotalLocalNormal).xyz;
	output.WorldTangent     = mul(Transform, float4(input.Tangent, 0.0f)).xyz;
	output.WorldBinormal    = mul(Transform, float4(input.Binormal, 0.0f)).xyz;
	output.ToLightVector    = u_LightPosition - WorldPosition.xyz;
	output.ViewVector       = u_CameraPosition - WorldPosition.xyz;
	output.ReflectedVector  = reflect(normalize(WorldPosition.xyz - u_CameraPosition), normalize(output.WorldNormal));
	
	float3 n = normalize(output.WorldNormal);
	float3 t = normalize(output.WorldTangent);
	float3 b = normalize(output.WorldBinormal);

	output.TBNMatrix = float3x3(t, b, n);

	output.ClipDistance = dot(WorldPosition, u_ClipPlane);

	return output;
}

)";
		}
	}

	std::string ShaderGraph::GetPixelShaderHLSLSource() const
	{
		std::string result = "";

		static std::string input_struct =
R"(
struct PS_INPUT
{
	float4 Position        : SV_POSITION;
	float2 UV              : UV1;
	float3 WorldNormal	   : COLOR1;
	float3 WorldTangent    : TANGENT1;
	float3 WorldBinormal   : BINORMAL1;
	float3 ToLightVector   : COLOR2;
	float3 ViewVector	   : TEXCOORD1;
	float3 ReflectedVector : TEXCOORD2;
	float3x3 TBNMatrix     : TEXCOORD3;
	float  ClipDistance    : SV_ClipDistance0;
};
)";

		static std::string ps_buffer =
R"(
cbuffer PSBuffer : register(b2)
{
	float4 u_LightColor;
	float  u_Time;
};

)";

		std::string material_buffer = "cbuffer MaterialBuffer : register(b3)\n{\n";
		material_buffer.append(UniformDeclarationsCode);
		material_buffer.append("};\n");

		static std::string lighting_functions =
R"(
float4 CalculateDiffuseComponent(float3 UnitNormal, float3 UnitLightVector)
{
    return saturate(dot(UnitLightVector, UnitNormal));
}

float4 CalculateAmbientComponent()
{
    return float4(0, 0, 0, 1);
}

float4 CalculateSpecularComponent(float4 DiffuseComponent, float3 UnitNormal, float3 UnitLightVector, float3 UnitView)
{
    float3 Reflect = normalize(2.0f * DiffuseComponent * UnitNormal - UnitLightVector);
    return pow(saturate(dot(Reflect, UnitView)), 7.0f) * 1.0f;
}

float4 LightingPass(float4 BaseColor, float3 BaseNormal, float3 UnitLightVector, float3 UnitView)
{
    float3 UnitNormal = normalize(BaseNormal);

    float4 DiffuseComponent  = CalculateDiffuseComponent(UnitNormal, UnitLightVector);
    float4 AmbientComponent  = CalculateAmbientComponent();
    float4 SpecularComponent = CalculateSpecularComponent(DiffuseComponent, UnitNormal, UnitLightVector, UnitView);

    return AmbientComponent + (u_LightColor * DiffuseComponent) * BaseColor + SpecularComponent;
}

)";

		static std::string main_decl = "float4 main(PS_INPUT input) : SV_TARGET\n{\n\n";
	
		static std::string normalization_code =
R"(
float3 UnitLightVector = normalize(input.ToLightVector);
float3 UnitView        = normalize(input.ViewVector);
)";

		static std::string light_pass = 
R"(
float4 _LightingPassResult_ = LightingPass(BaseColor, BaseNormal, UnitLightVector, UnitView);
)";

		std::string functions_section = lighting_functions;

		std::string final_color_decl = "";

		if (m_ShouldUseLighting)
			final_color_decl = "float4 _out_FinalColor_ = _LightingPassResult_;\n";
		else
			final_color_decl = "float4 _out_FinalColor_ = BaseColor;\n";

		if (m_ShouldUseEnvironmentMap)
		{
			final_color_decl += "\nfloat4 _ReflectedColor_ = _EnvironmentMapTexture_.Sample(_EnvironmentMapSampler_, input.ReflectedVector);\n";
			final_color_decl += "_out_FinalColor_ = lerp(_out_FinalColor_, _ReflectedColor_, (1.0f - u_Roughness));\n";
		}

		std::string return_statement = "return _out_FinalColor_;";

		result.append(input_struct);
		result.append(ps_buffer);
		result.append(material_buffer);
		result.append(GlobalDeclarationsCode);
		result.append(functions_section);
		result.append(main_decl);
		result.append(normalization_code);
		result.append(BaseColorCode);
		result.append(BaseNormalCode);
		result.append(light_pass);
		result.append(final_color_decl);
		result.append(return_statement);
		result.append("\n}\n");
		return result;
	}

	std::vector<UniformVariable> ShaderGraph::GetMaterialUniforms() const
	{
		return MaterialUniforms;
	}
}
