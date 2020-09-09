#include "DirectXRenderingAPI.h"
#include <platform/DirectXResources.h>

namespace ifr
{
	void DirectXRenderingAPI::ClearScreenColor(const glm::vec4& color)
	{
		float bgcolor[] = { color.r, color.g, color.b, color.a };
		DirectXResources::s_DeviceContext->ClearRenderTargetView(DirectXResources::s_RenderTargetView.Get(), bgcolor);
	}

	void DirectXRenderingAPI::ClearScreenBuffers()
	{
		DirectXResources::s_DeviceContext->ClearDepthStencilView(DirectXResources::s_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		DirectXResources::s_DeviceContext->RSSetState(DirectXResources::s_RasterizerState.Get());
		DirectXResources::s_DeviceContext->OMSetDepthStencilState(DirectXResources::s_DepthStencilState.Get(), 0);
	}

	void DirectXRenderingAPI::DrawIndexed(const Ref<VertexArray>& va)
	{
		DirectXResources::s_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		DirectXResources::s_DeviceContext->DrawIndexed(va->GetIndexBuffer()->GetCount(), 0, 0);
	}

	void DirectXRenderingAPI::DrawInstanced(const Ref<VertexArray>& va, uint32_t count)
	{
		DirectXResources::s_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		DirectXResources::s_DeviceContext->DrawIndexedInstanced(va->GetIndexBuffer()->GetCount(), count, 0, 0, 0);
	}

	void DirectXRenderingAPI::DrawIndexedControlPointPatchList(const Ref<VertexArray>& va)
	{
		DirectXResources::s_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
		DirectXResources::s_DeviceContext->DrawIndexed(va->GetIndexBuffer()->GetCount(), 0, 0);
	}

	void DirectXRenderingAPI::SetWireframe(bool wf)
	{
		D3D11_RASTERIZER_DESC rasterizerdesc;
		ZeroMemory(&rasterizerdesc, sizeof(D3D11_RASTERIZER_DESC));

		rasterizerdesc.FillMode = wf ? D3D11_FILL_MODE::D3D11_FILL_WIREFRAME : D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerdesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		HRESULT result = DirectXResources::s_Device->CreateRasterizerState(&rasterizerdesc, DirectXResources::s_RasterizerState.GetAddressOf());
		if (FAILED(result))
			IFR_LOG_ERROR("Failed to create new rasterizer state");

		DirectXResources::s_DeviceContext->RSSetState(DirectXResources::s_RasterizerState.Get());
	}

	void DirectXRenderingAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = (FLOAT)x;
		viewport.TopLeftY = (FLOAT)y;

		viewport.Width = (FLOAT)width;
		viewport.Height = (FLOAT)height;

		viewport.MinDepth = 0;
		viewport.MaxDepth = 1;

		DirectXResources::s_DeviceContext->RSSetViewports(1, &viewport);
	}
}
