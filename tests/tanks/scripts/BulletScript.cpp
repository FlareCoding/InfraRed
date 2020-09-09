#include "BulletScript.h"

BulletScript::BulletScript(Entity ID, Ref<Scene> scene) : m_EID(ID), m_Scene(scene)
{
	SpawnTimestamp = Time::GetElapsedTime();
}

void BulletScript::OnUpdate()
{
	if (Time::GetElapsedTime() - SpawnTimestamp > BulletLifetime)
	{
		m_Scene->RemoveAndDeleteEntity(m_EID);
	}
}
