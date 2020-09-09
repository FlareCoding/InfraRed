#pragma once
#include <platform/DirectXResources.h>
#include <vector>

namespace ifr
{
	class AdapterData
	{
	public:
		AdapterData(IDXGIAdapter* adapter);
		IDXGIAdapter* m_Adapter;
		DXGI_ADAPTER_DESC m_Description;
	};

	class AdapterReader
	{
	public:
		static std::vector<AdapterData> GetAdapters();
	private:
		static std::vector<AdapterData> m_Adapters;
	};

}
