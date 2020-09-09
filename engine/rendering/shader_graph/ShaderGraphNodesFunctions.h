#pragma once
#include "ShaderGraphNode.h"

namespace ifr
{
	class SGLerpNode : public ShaderGraphNode
	{
	public:
		SGLerpNode(uint32_t id, ShaderGraphHLSLDataType output_t) : ShaderGraphNode(id, output_t) {}

		// Inherited via ShaderGraphNode
		virtual std::string GetShaderPassCode() const override;
	};

	class SGSineNode : public ShaderGraphNode
	{
	public:
		SGSineNode(uint32_t id) : ShaderGraphNode(id, HLSL_FLOAT) {}

		// Inherited via ShaderGraphNode
		virtual std::string GetShaderPassCode() const override;
	};

	class SGMaxNode : public ShaderGraphNode
	{
	public:
		SGMaxNode(uint32_t id, ShaderGraphHLSLDataType output_t) : ShaderGraphNode(id, output_t) {}

		// Inherited via ShaderGraphNode
		virtual std::string GetShaderPassCode() const override;
	};

	class SGMultiplyNode : public ShaderGraphNode
	{
	public:
		SGMultiplyNode(uint32_t id, ShaderGraphHLSLDataType output_t) : ShaderGraphNode(id, output_t) {}

		// Inherited via ShaderGraphNode
		virtual std::string GetShaderPassCode() const override;
	};

	class SGSubtractNode : public ShaderGraphNode
	{
	public:
		SGSubtractNode(uint32_t id, ShaderGraphHLSLDataType output_t) : ShaderGraphNode(id, output_t) {}

		// Inherited via ShaderGraphNode
		virtual std::string GetShaderPassCode() const override;
	};

	class SGAddNode : public ShaderGraphNode
	{
	public:
		SGAddNode(uint32_t id, ShaderGraphHLSLDataType output_t) : ShaderGraphNode(id, output_t) {}

		// Inherited via ShaderGraphNode
		virtual std::string GetShaderPassCode() const override;
	};

	class SGDivideNode : public ShaderGraphNode
	{
	public:
		SGDivideNode(uint32_t id, ShaderGraphHLSLDataType output_t) : ShaderGraphNode(id, output_t) {}

		// Inherited via ShaderGraphNode
		virtual std::string GetShaderPassCode() const override;
	};

	enum ShaderGraphMaskType
	{
		MASK_R,
		MASK_G,
		MASK_B,
		MASK_A
	};

	std::string ShaderGraphMaskTypeToString(ShaderGraphMaskType mask);

	class SGMaskNode : public ShaderGraphNode
	{
	public:
		SGMaskNode(uint32_t id) : ShaderGraphNode(id, HLSL_FLOAT) {}

		ShaderGraphMaskType mask = MASK_R;

		// Inherited via ShaderGraphNode
		virtual std::string GetShaderPassCode() const override;
	};

	class SGOneMinusNode : public ShaderGraphNode
	{
	public:
		SGOneMinusNode(uint32_t id, ShaderGraphHLSLDataType output_t) : ShaderGraphNode(id, output_t) {}

		// Inherited via ShaderGraphNode
		virtual std::string GetShaderPassCode() const override;
	};

	class SGPowerNode : public ShaderGraphNode
	{
	public:
		SGPowerNode(uint32_t id) : ShaderGraphNode(id, HLSL_FLOAT) {}

		// Inherited via ShaderGraphNode
		virtual std::string GetShaderPassCode() const override;
	};
}
