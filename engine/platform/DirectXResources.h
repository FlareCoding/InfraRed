#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include <wrl/client.h>
using namespace Microsoft::WRL;

namespace ifr
{
	class DirectXResources
	{
	public:
		static ComPtr<ID3D11Device>				s_Device;
		static ComPtr<ID3D11DeviceContext>		s_DeviceContext;
		static ComPtr<ID3D11RenderTargetView>	s_RenderTargetView;
		static ComPtr<ID3D11RasterizerState>	s_RasterizerState;
		static ComPtr<ID3D11DepthStencilState>	s_DepthStencilState;
		static ComPtr<ID3D11DepthStencilView>	s_DepthStencilView;
		static ComPtr<ID3D11SamplerState>		s_SamplerState;
	};
}
