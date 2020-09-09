#include "DirectXResources.h"

namespace ifr
{
	ComPtr<ID3D11Device>			DirectXResources::s_Device = nullptr;
	ComPtr<ID3D11DeviceContext>		DirectXResources::s_DeviceContext = nullptr;
	ComPtr<ID3D11RenderTargetView>	DirectXResources::s_RenderTargetView = nullptr;
	ComPtr<ID3D11RasterizerState>	DirectXResources::s_RasterizerState = nullptr;
	ComPtr<ID3D11DepthStencilState> DirectXResources::s_DepthStencilState = nullptr;
	ComPtr<ID3D11DepthStencilView>	DirectXResources::s_DepthStencilView = nullptr;
	ComPtr<ID3D11SamplerState>		DirectXResources::s_SamplerState = nullptr;
}
