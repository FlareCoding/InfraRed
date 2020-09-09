#pragma once
#include <unordered_map>
#include "ShaderGraphNodesPrimitives.h"
#include "ShaderGraphNodesFunctions.h"
#include <rendering/shaders/UniformBuffer.h>

namespace ifr
{
	enum ShaderGraphMeshType
	{
		MESH_STATIC,
		MESH_ANIMATED
	};

	class ShaderGraph
	{
	public:
		ShaderGraph();

		uint32_t CreateNodeID();
		void ConnectBaseColorInput(const Ref<ShaderGraphNode>& node);
		void GenerateHLSLCode();

		std::string GetVertexShaderHLSLSource(ShaderGraphMeshType mesh_type) const;
		std::string GetPixelShaderHLSLSource() const;
		std::vector<UniformVariable> GetMaterialUniforms() const;

		const Ref<SGCustomNode> GetInputUVNode();
		const Ref<SGCustomNode> GetInputTimeNode();

		void SetLitMode(bool lit) { m_ShouldUseLighting = lit; }
		void ShouldUseNormalMap(bool bUse, uint32_t slot = 1);
		void ShouldUseEnvironmentMap(bool bUse, uint32_t slot = 2);

	private:
		void AddNodeHLSLSourceToCodeStream(std::string& src, const Ref<ShaderGraphNode>& node);

		std::string BaseColorCode;
		std::string BaseNormalCode;
		std::string GlobalDeclarationsCode;
		
		std::string UniformDeclarationsCode;
		std::vector<UniformVariable> MaterialUniforms;

		bool m_ShouldUseLighting = true;

		bool m_ShouldUseNormalMap = false;
		uint32_t m_NormalMapSlot = 1;

		bool m_ShouldUseEnvironmentMap = false;
		uint32_t m_EnvironmentMapSlot = 2;

	private:
		void CreateInputUVNode();
		void CreateInputTimeNode();

		Ref<SGCustomNode> m_InputUVNode = nullptr;
		Ref<SGCustomNode> m_InputTimeNode = nullptr;

	private:
		Ref<ShaderGraphNode> m_BaseColorInputNode = 0;

		uint32_t m_AvailableNodeID = 0;
		std::unordered_map<uint32_t, bool> m_NodeDeclarationMap;
	};
}
