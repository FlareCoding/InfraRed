#pragma once
#include <core/IFRCore.h>

namespace ifr
{
	enum class ComponentType
	{
		Unknown = -1,
		Transform,
		StaticRender,
		AnimatedRender,
		Collider,
		Script,
		Information
	};

	class IComponent;

	struct ComponentMessage
	{
		IComponent* Sender = nullptr;
		int Message = 0;
		void* AdditionalData;
	};

	class IComponent
	{
	public:
		virtual ComponentType GetType() = 0;
		virtual void OnAttach() = 0;
		virtual void OnUpdate() = 0;

		bool Enabled = false;
		uint64_t Owner = -1;
	};
}
