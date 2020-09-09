#include "UniformBuffer.h"
#include "../Renderer.h"
#include <platform/rendering/shaders/DirectXUniformBuffer.h>

namespace ifr
{
	UniformBuffer::~UniformBuffer()
	{
		if (m_Data) free(m_Data);
	}

	void UniformBuffer::SetVariableValue(const char* name, void* value)
	{
		auto entry = m_UniformVariables.find(name);
		if (entry != m_UniformVariables.end())
		{
			uint32_t size = entry->second.first;
			uint32_t offset = entry->second.second;
			memcpy_s((void*)((char*)m_Data + offset), size, value, size);
		}
	}

	void UniformBuffer::SetBufferValue(void* value)
	{
		memcpy_s(m_Data, m_DataSize, value, m_DataSize);
	}

	void UniformBuffer::GetVariableValue(const char* name, void* out_val)
	{
		auto entry = m_UniformVariables.find(name);
		if (entry != m_UniformVariables.end())
		{
			uint32_t size = entry->second.first;
			uint32_t offset = entry->second.second;
			memcpy_s(out_val, size, (void*)((char*)m_Data + offset), size);
		}
	}

	UniformBuffer::UniformBuffer(const std::string& name, const std::vector<UniformVariable>& layout, UniformBufferParentShader parent, uint32_t slot)
		: m_Name(name), m_ParentShader(parent), BindingSlot(slot)
	{
		uint32_t offset = 0;
		uint32_t size = 0;

		for (auto& element : layout)
		{
			m_UniformVariables[element.Name] = { (uint32_t)element.Size, (uint32_t)offset };
			offset += element.Size;
			size += element.Size;
		}

		m_DataSize = size;

		m_Data = malloc(m_DataSize);
	}

	Ref<UniformBuffer> UniformBuffer::Create(const std::string& name, const std::vector<UniformVariable>& layout, UniformBufferParentShader parent, uint32_t slot)
	{
		return Ref<UniformBuffer>(new DirectXUniformBuffer(name, layout, parent, slot));
	}
}
