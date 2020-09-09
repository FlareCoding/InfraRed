#include "DirectXShader.h"
#include <logger/Logger.h>

namespace ifr
{
	static DXGI_FORMAT ShaderDataTypeToDxgiFormat(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
		case ShaderDataType::Float2:   return DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
		case ShaderDataType::Float3:   return DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
		case ShaderDataType::Float4:   return DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
		case ShaderDataType::Mat3:     return DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
		case ShaderDataType::Mat4:     return DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
		case ShaderDataType::Int:      return DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
		case ShaderDataType::Int2:     return DXGI_FORMAT::DXGI_FORMAT_R32G32_UINT;
		case ShaderDataType::Int3:     return DXGI_FORMAT::DXGI_FORMAT_R32G32B32_UINT;
		case ShaderDataType::Int4:     return DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_UINT;
		case ShaderDataType::Bool:     return DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
		}

		return DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
	}

	static void GetDirectXVertexShaderLayout(const BufferLayout& layout, std::vector<D3D11_INPUT_ELEMENT_DESC>& _Out_ desc, UINT& _Out_ elem_count)
	{
		for (auto& element : layout)
		{
			D3D11_INPUT_CLASSIFICATION Classification = D3D11_INPUT_PER_VERTEX_DATA;
			UINT InstanceDataStepRate = 0;

			if (element.ElementInputClass == BufferElementInputClass::INPUT_PER_INSTANCE_DATA)
			{
				Classification = D3D11_INPUT_PER_INSTANCE_DATA;
				InstanceDataStepRate = 1;
			}

			D3D11_INPUT_ELEMENT_DESC InputElemDesc;
			ZeroMemory(&InputElemDesc, sizeof(D3D11_INPUT_ELEMENT_DESC));

			InputElemDesc.SemanticName = element.Name.c_str();
			InputElemDesc.SemanticIndex = 0;
			InputElemDesc.Format = ShaderDataTypeToDxgiFormat(element.Type);
			InputElemDesc.InputSlot = element.InputSlot;
			InputElemDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			InputElemDesc.InputSlotClass = Classification;
			InputElemDesc.InstanceDataStepRate = InstanceDataStepRate;

			desc.push_back(InputElemDesc);
		}

		elem_count = (UINT)layout.GetElements().size();
	}

	DirectXVertexShader::DirectXVertexShader(const std::string& src)
	{
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
		flags |= D3DCOMPILE_DEBUG;
#endif
		ComPtr<ID3DBlob> error_messages = nullptr;
		HRESULT result = D3DCompile(
			src.c_str(),
			src.length(),
			nullptr,
			nullptr,
			nullptr,
			"main", "vs_5_0",
			D3DCOMPILE_ENABLE_STRICTNESS, 0,
			m_ShaderBuffer.GetAddressOf(),
			error_messages.GetAddressOf()
		);

		if (FAILED(result) && error_messages)
		{
			const char* error_msg = (const char*)error_messages->GetBufferPointer();
			IFR_LOG_FUNC_ERROR(error_msg);
			error_messages->Release();
			return;
		}

		result = DirectXResources::s_Device->CreateVertexShader(m_ShaderBuffer.Get()->GetBufferPointer(), m_ShaderBuffer.Get()->GetBufferSize(), NULL, m_Shader.GetAddressOf());
		if (FAILED(result))
		{
			std::string error_msg = ("Failed to compile vertex shader");
			IFR_LOG_ERROR(error_msg);
			return;
		}
	}

	void DirectXVertexShader::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* layout_description, UINT element_count)
	{
		HRESULT result = DirectXResources::s_Device->CreateInputLayout(layout_description, element_count, m_ShaderBuffer->GetBufferPointer(), m_ShaderBuffer->GetBufferSize(), m_InputLayout.GetAddressOf());
		if (FAILED(result))
		{
			IFR_LOG_ERROR("Failed to create ID3D11InputLayout");
			return;
		}
	}

	DirectXHullShader::DirectXHullShader(const std::string& src)
	{
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
		flags |= D3DCOMPILE_DEBUG;
#endif
		ComPtr<ID3DBlob> error_messages = nullptr;
		HRESULT result = D3DCompile(
			src.c_str(),
			src.length(),
			nullptr,
			nullptr,
			nullptr,
			"main", "hs_5_0",
			D3DCOMPILE_ENABLE_STRICTNESS, 0,
			m_ShaderBuffer.GetAddressOf(),
			error_messages.GetAddressOf()
		);

		if (FAILED(result) && error_messages)
		{
			const char* error_msg = (const char*)error_messages->GetBufferPointer();
			IFR_LOG_FUNC_ERROR(error_msg);
			error_messages->Release();
			return;
		}

		result = DirectXResources::s_Device->CreateHullShader(m_ShaderBuffer.Get()->GetBufferPointer(), m_ShaderBuffer.Get()->GetBufferSize(), NULL, m_Shader.GetAddressOf());
		if (FAILED(result))
		{
			std::string error_msg = ("Failed to compile hull shader");
			IFR_LOG_ERROR(error_msg);
			return;
		}
	}

	DirectXPixelShader::DirectXPixelShader(const std::string& src)
	{
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
		flags |= D3DCOMPILE_DEBUG;
#endif
		ComPtr<ID3DBlob> error_messages = nullptr;
		HRESULT result = D3DCompile(
			src.c_str(),
			src.length(),
			nullptr,
			nullptr,
			nullptr,
			"main", "ps_5_0",
			D3DCOMPILE_ENABLE_STRICTNESS, 0,
			m_ShaderBuffer.GetAddressOf(),
			error_messages.GetAddressOf()
		);

		if (FAILED(result) && error_messages)
		{
			const char* error_msg = (const char*)error_messages->GetBufferPointer();
			IFR_LOG_FUNC_ERROR(error_msg);
			error_messages->Release();
			return;
		}

		result = DirectXResources::s_Device->CreatePixelShader(m_ShaderBuffer.Get()->GetBufferPointer(), m_ShaderBuffer.Get()->GetBufferSize(), NULL, m_Shader.GetAddressOf());
		if (FAILED(result))
		{
			std::string error_msg = ("Failed to compile pixel shader");
			IFR_LOG_ERROR(error_msg);
			return;
		}
	}

	DirectXDomainShader::DirectXDomainShader(const std::string& src)
	{
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
		flags |= D3DCOMPILE_DEBUG;
#endif
		ComPtr<ID3DBlob> error_messages = nullptr;
		HRESULT result = D3DCompile(
			src.c_str(),
			src.length(),
			nullptr,
			nullptr,
			nullptr,
			"main", "ds_5_0",
			D3DCOMPILE_ENABLE_STRICTNESS, 0,
			m_ShaderBuffer.GetAddressOf(),
			error_messages.GetAddressOf()
		);

		if (FAILED(result) && error_messages)
		{
			const char* error_msg = (const char*)error_messages->GetBufferPointer();
			IFR_LOG_FUNC_ERROR(error_msg);
			error_messages->Release();
			return;
		}

		result = DirectXResources::s_Device->CreateDomainShader(m_ShaderBuffer.Get()->GetBufferPointer(), m_ShaderBuffer.Get()->GetBufferSize(), NULL, m_Shader.GetAddressOf());
		if (FAILED(result))
		{
			std::string error_msg = ("Failed to compile domain shader");
			IFR_LOG_ERROR(error_msg);
			return;
		}
	}

	DirectXShaderInstance::DirectXShaderInstance(const std::string& vs_src, const std::string& ps_src, const BufferLayout& layout)
	{
		m_VertexShader = std::make_unique<DirectXVertexShader>(vs_src);
		m_PixelShader = std::make_unique<DirectXPixelShader>(ps_src);

		std::vector<D3D11_INPUT_ELEMENT_DESC> d3d11layout;
		UINT element_count = 0;
		GetDirectXVertexShaderLayout(layout, d3d11layout, element_count);

		CreateInputLayout(&d3d11layout[0], element_count);
	}

	DirectXShaderInstance::DirectXShaderInstance(const std::string& vs_src, const std::string& hs_src, const std::string& ds_src, const std::string& ps_src, const BufferLayout& layout)
	{
		m_VertexShader	= std::make_unique<DirectXVertexShader>(vs_src);
		m_HullShader	= std::make_unique<DirectXHullShader>(hs_src);
		m_DomainShader	= std::make_unique<DirectXDomainShader>(ds_src);
		m_PixelShader	= std::make_unique<DirectXPixelShader>(ps_src);

		std::vector<D3D11_INPUT_ELEMENT_DESC> d3d11layout;
		UINT element_count = 0;
		GetDirectXVertexShaderLayout(layout, d3d11layout, element_count);

		CreateInputLayout(&d3d11layout[0], element_count);
	}

	void DirectXShaderInstance::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* layout_description, UINT element_count)
	{
		m_VertexShader->CreateInputLayout(layout_description, element_count);
	}

	void DirectXShaderInstance::Bind()
	{
		DirectXResources::s_DeviceContext->IASetInputLayout(m_VertexShader->GetInputLayout());
		
		DirectXResources::s_DeviceContext->VSSetShader(m_VertexShader->GetShader(), NULL, 0);
		DirectXResources::s_DeviceContext->PSSetShader(m_PixelShader->GetShader(), NULL, 0);

		if (m_HullShader)
			DirectXResources::s_DeviceContext->HSSetShader(m_HullShader->GetShader(), NULL, 0);
		else
			DirectXResources::s_DeviceContext->HSSetShader(nullptr, NULL, 0);

		if (m_DomainShader)
			DirectXResources::s_DeviceContext->DSSetShader(m_DomainShader->GetShader(), NULL, 0);
		else
			DirectXResources::s_DeviceContext->DSSetShader(nullptr, NULL, 0);

		DirectXResources::s_DeviceContext->PSSetSamplers(0, 1, DirectXResources::s_SamplerState.GetAddressOf());
		DirectXResources::s_DeviceContext->PSSetSamplers(1, 1, DirectXResources::s_SamplerState.GetAddressOf());
		DirectXResources::s_DeviceContext->PSSetSamplers(2, 1, DirectXResources::s_SamplerState.GetAddressOf());
		DirectXResources::s_DeviceContext->PSSetSamplers(3, 1, DirectXResources::s_SamplerState.GetAddressOf());
		DirectXResources::s_DeviceContext->PSSetSamplers(4, 1, DirectXResources::s_SamplerState.GetAddressOf());

		if (m_HullShader)
		{
			DirectXResources::s_DeviceContext->HSSetSamplers(0, 1, DirectXResources::s_SamplerState.GetAddressOf());
			DirectXResources::s_DeviceContext->HSSetSamplers(1, 1, DirectXResources::s_SamplerState.GetAddressOf());
			DirectXResources::s_DeviceContext->HSSetSamplers(2, 1, DirectXResources::s_SamplerState.GetAddressOf());
			DirectXResources::s_DeviceContext->HSSetSamplers(3, 1, DirectXResources::s_SamplerState.GetAddressOf());
			DirectXResources::s_DeviceContext->HSSetSamplers(4, 1, DirectXResources::s_SamplerState.GetAddressOf());
		}

		if (m_DomainShader)
		{
			DirectXResources::s_DeviceContext->DSSetSamplers(0, 1, DirectXResources::s_SamplerState.GetAddressOf());
			DirectXResources::s_DeviceContext->DSSetSamplers(1, 1, DirectXResources::s_SamplerState.GetAddressOf());
			DirectXResources::s_DeviceContext->DSSetSamplers(2, 1, DirectXResources::s_SamplerState.GetAddressOf());
			DirectXResources::s_DeviceContext->DSSetSamplers(3, 1, DirectXResources::s_SamplerState.GetAddressOf());
			DirectXResources::s_DeviceContext->DSSetSamplers(4, 1, DirectXResources::s_SamplerState.GetAddressOf());
		}
	}

	void DirectXShaderInstance::Unbind() {}
}
