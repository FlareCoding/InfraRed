#pragma once
#include <rendering/shaders/Shader.h>
#include <platform/DirectXResources.h>

namespace ifr
{
	class DirectXVertexShader
	{
	public:
		DirectXVertexShader(const std::string& src);
		void CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* layout_description, UINT element_count);

		inline ID3D11VertexShader*	GetShader()			{ return m_Shader.Get(); }
		inline ID3D10Blob*			GetBuffer()			{ return m_ShaderBuffer.Get(); }
		inline ID3D11InputLayout*	GetInputLayout()	{ return m_InputLayout.Get(); }

	private:
		ComPtr<ID3D11VertexShader>	m_Shader = nullptr;
		ComPtr<ID3D10Blob>			m_ShaderBuffer = nullptr;
		ComPtr<ID3D11InputLayout>	m_InputLayout = nullptr;
	};

	class DirectXHullShader
	{
	public:
		DirectXHullShader(const std::string& src);

		inline ID3D11HullShader* GetShader() { return m_Shader.Get(); }
		inline ID3D10Blob* GetBuffer() { return m_ShaderBuffer.Get(); }

	private:
		ComPtr<ID3D11HullShader>	m_Shader = nullptr;
		ComPtr<ID3D10Blob>			m_ShaderBuffer = nullptr;
	};

	class DirectXDomainShader
	{
	public:
		DirectXDomainShader(const std::string& src);

		inline ID3D11DomainShader* GetShader() { return m_Shader.Get(); }
		inline ID3D10Blob* GetBuffer() { return m_ShaderBuffer.Get(); }

	private:
		ComPtr<ID3D11DomainShader>	m_Shader = nullptr;
		ComPtr<ID3D10Blob>			m_ShaderBuffer = nullptr;
	};

	class DirectXPixelShader
	{
	public:
		DirectXPixelShader(const std::string& src);

		inline ID3D11PixelShader*	GetShader() { return m_Shader.Get(); }
		inline ID3D10Blob*			GetBuffer() { return m_ShaderBuffer.Get(); }

	private:
		ComPtr<ID3D11PixelShader>	m_Shader = nullptr;
		ComPtr<ID3D10Blob>			m_ShaderBuffer = nullptr;
	};

	class DirectXShaderInstance : public ShaderInstance
	{
	public:
		DirectXShaderInstance(const std::string& vs_src, const std::string& ps_src, const BufferLayout& layout);
		DirectXShaderInstance(const std::string& vs_src, const std::string& hs_src, const std::string& ds_src, const std::string& ps_src, const BufferLayout& layout);
		void CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* layout_description, UINT element_count);

		inline DirectXVertexShader* GetVertexShader() { return m_VertexShader.get(); }
		inline DirectXPixelShader* GetPixelShader() { return m_PixelShader.get(); }

		virtual void Bind() override;
		virtual void Unbind() override;

	protected:
		std::unique_ptr<DirectXVertexShader>	m_VertexShader;
		std::unique_ptr<DirectXHullShader>		m_HullShader = nullptr;		// optional
		std::unique_ptr<DirectXDomainShader>	m_DomainShader = nullptr;	// optional
		std::unique_ptr<DirectXPixelShader>		m_PixelShader;
	};
}
