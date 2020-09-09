#include "ShaderGraphNodesFunctions.h"

namespace ifr
{
	std::string SGLerpNode::GetShaderPassCode() const
	{
		if (Inputs.size() < 2 || !Inputs[0] || !Inputs[1])
		{
			IFR_LOG_ERROR("SGLerpNode> Invalid Paramter Count");
			return "";
		}

		std::string& InputAName = Inputs[0]->OutputName;
		std::string& InputBName = Inputs[1]->OutputName;

		std::string Amount = "0.5f";
		if (Inputs.size() > 2 && Inputs[2])
			Amount = Inputs[2]->OutputName;

		std::string OutputTypeStr = HLSLTypeToString(OutputType);
		return OutputTypeStr + " " + OutputName + " = lerp(" + InputAName + ", " + InputBName + ", " + Amount + ");";
	}
	
	std::string SGSineNode::GetShaderPassCode() const
	{
		if (Inputs.size() != 1 || !Inputs[0])
		{
			IFR_LOG_ERROR("SGSineNode> Invalid Parameter Count");
			return "";
		}

		std::string OutputTypeStr = HLSLTypeToString(OutputType);
		return OutputTypeStr + " " + OutputName + " = sin(" + Inputs[0]->OutputName + ");";
	}

	std::string SGMaxNode::GetShaderPassCode() const
	{
		if (Inputs.size() != 2 || !Inputs[0] || !Inputs[1])
		{
			IFR_LOG_ERROR("SGMaxNode> Invalid Parameter Count");
			return "";
		}

		std::string OutputTypeStr = HLSLTypeToString(OutputType);
		return OutputTypeStr + " " + OutputName + " = max(" + Inputs[0]->OutputName + ", " + Inputs[1]->OutputName + ");";
	}
	
	std::string SGMultiplyNode::GetShaderPassCode() const
	{
		if (Inputs.size() < 1 || !Inputs[0])
		{
			IFR_LOG_ERROR("SGMultiplyNode> Invalid Parameter Count");
			return "";
		}

		std::string second_factor = "0.5f";
		if (Inputs.size() > 1 && Inputs[1])
			second_factor = Inputs[1]->OutputName;

		std::string OutputTypeStr = HLSLTypeToString(OutputType);
		return OutputTypeStr + " " + OutputName + " = mul(" + Inputs[0]->OutputName + ", " + second_factor + ");";
	}
	
	std::string SGDivideNode::GetShaderPassCode() const
	{
		if (Inputs.size() != 2 || !Inputs[0] || !Inputs[1])
		{
			IFR_LOG_ERROR("SGDivideNode> Invalid Parameter Count");
			return "";
		}

		std::string OutputTypeStr = HLSLTypeToString(OutputType);
		return OutputTypeStr + " " + OutputName + " = " + Inputs[0]->OutputName + " / " + Inputs[1]->OutputName + ";";
	}

	std::string SGAddNode::GetShaderPassCode() const
	{
		if (Inputs.size() != 2 || !Inputs[0] || !Inputs[1])
		{
			IFR_LOG_ERROR("SGAddNode> Invalid Parameter Count");
			return "";
		}

		std::string OutputTypeStr = HLSLTypeToString(OutputType);
		return OutputTypeStr + " " + OutputName + " = " + Inputs[0]->OutputName + " + " + Inputs[1]->OutputName + ";";
	}

	std::string SGSubtractNode::GetShaderPassCode() const
	{
		if (Inputs.size() != 2 || !Inputs[0] || !Inputs[1])
		{
			IFR_LOG_ERROR("SGSubtractNode> Invalid Parameter Count");
			return "";
		}

		std::string OutputTypeStr = HLSLTypeToString(OutputType);
		return OutputTypeStr + " " + OutputName + " = " + Inputs[0]->OutputName + " - " + Inputs[1]->OutputName + ";";
	}
	
	std::string ShaderGraphMaskTypeToString(ShaderGraphMaskType mask)
	{
		switch (mask)
		{
		case MASK_R: return "r";
		case MASK_G: return "g";
		case MASK_B: return "b";
		case MASK_A: return "a";
		default: return "";
		}
	}

	std::string SGMaskNode::GetShaderPassCode() const
	{
		if (Inputs.size() != 1 || !Inputs[0])
		{
			IFR_LOG_ERROR("SGMaskNode> Invalid Parameter Count");
			return "";
		}

		auto mask_str = ShaderGraphMaskTypeToString(mask);
		return "float " + OutputName + " = " + Inputs[0]->OutputName + "." + mask_str + ";";
	}

	std::string SGOneMinusNode::GetShaderPassCode() const
	{
		if (Inputs.size() != 1 || !Inputs[0])
		{
			IFR_LOG_ERROR("SGOneMinusNode> Invalid Parameter Count");
			return "";
		}

		std::string OutputTypeStr = HLSLTypeToString(OutputType);
		return OutputTypeStr + " " + OutputName + " = 1 - " + Inputs[0]->OutputName + ";";
	}
	
	std::string SGPowerNode::GetShaderPassCode() const
	{
		if (Inputs.size() != 2 || !Inputs[0] || !Inputs[1])
		{
			IFR_LOG_ERROR("SGPowerNode> Invalid Parameter Count");
			return "";
		}

		return "float " + OutputName + " = pow(" + Inputs[0]->OutputName + ", " + Inputs[1]->OutputName + ");";
	}
}
