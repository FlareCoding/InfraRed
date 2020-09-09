#pragma once
#include <core/IFRCore.h>
#include <map>
#include <vector>
#include <string>

namespace ifr
{
	struct UniformVariable
	{
		UniformVariable() {}
		UniformVariable(const std::string& name, uint32_t size)
			: Name(name), Size(size)
		{
		}

		std::string Name;
		uint32_t Size = 0;
	};

	enum class UniformBufferParentShader
	{
		VERTEX_SHADER,
		HULL_SHADER,
		DOMAIN_SHADER,
		GEOMETRY_SHADER,
		PIXEL_SHADER
	};

	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer();
		
		uint32_t BindingSlot = 0;

		inline uint32_t  GetSize() const { return m_DataSize; }
		inline void*		GetData() const { return m_Data; }
		inline const std::string& GetName() const { return m_Name; }

		void SetVariableValue(const char* name, void* value);
		void SetBufferValue(void* value);

		void GetVariableValue(const char* name, void* out_val);

		virtual void UploadToShader() = 0;
		
		static Ref<UniformBuffer> Create(const std::string& name, const std::vector<UniformVariable>& layout, 
			UniformBufferParentShader parent, uint32_t slot = 0);

	protected:
		UniformBuffer(const std::string& name, const std::vector<UniformVariable>& layout, UniformBufferParentShader parent, uint32_t slot);
		uint32_t	m_DataSize = 0;
		void*		m_Data = nullptr;

	protected:
		const std::string m_Name;

	protected:
		UniformBufferParentShader m_ParentShader;

	protected:
		// const char* --> name
		// Pair.first  --> size
		// Pair.second --> offset
		std::map<std::string, std::pair<uint32_t, uint32_t>> m_UniformVariables;
	};
}
