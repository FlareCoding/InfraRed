#pragma once
#include "ShaderGraphNode.h"

namespace ifr
{
	class SGConstantNode : public ShaderGraphNode
	{
	public:
		SGConstantNode(uint32_t id, ShaderGraphHLSLDataType output_t);

		template <typename T>
		void SetValue(T val);

		// Inherited via ShaderGraphNode
		virtual std::string GetShaderPassCode() const override;

	private:
		std::string OutputValue = "";
	};

	template<typename T>
	inline void SGConstantNode::SetValue(T val)
	{
		OutputValue = std::to_string(val);
	}

	class SGFloat2Node : public ShaderGraphNode
	{
	public:
		SGFloat2Node(uint32_t id);

		// Inherited via ShaderGraphNode
		virtual std::string GetShaderPassCode() const override;
	};

	class SGFloat3Node : public ShaderGraphNode
	{
	public:
		SGFloat3Node(uint32_t id);

		// Inherited via ShaderGraphNode
		virtual std::string GetShaderPassCode() const override;
	};

	class SGFloat4Node : public ShaderGraphNode
	{
	public:
		SGFloat4Node(uint32_t id);

		// Inherited via ShaderGraphNode
		virtual std::string GetShaderPassCode() const override;
	};

	class SGTextureSampler2DNode : public ShaderGraphNode
	{
	public:
		SGTextureSampler2DNode(uint32_t id);

		// Inherited via ShaderGraphNode
		virtual std::string GetShaderPassCode() const override;
		virtual std::string GetShaderGlobalCode() const override;

		std::string TextureName = "Anon";
		uint32_t TextureSlot = 0;
		bool CoordinateCorrectionEnabled = false;
	};
}
