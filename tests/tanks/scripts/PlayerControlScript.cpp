#include "PlayerControlScript.h"
#include "BulletScript.h"

PlayerControlScript::PlayerControlScript(Entity entityID, Ref<Scene> scene, Ref<MaterialInstance> bullet_material) : m_EID(entityID), m_Scene(scene), m_BulletMaterial(bullet_material)
{
	Transform = reinterpret_cast<TransformComponent*>(GetComponent(entityID, ComponentType::Transform));
	Collider = reinterpret_cast<ColliderComponent*>(GetComponent(entityID, ComponentType::Collider));

	m_BulletModel = Assets::FindStaticModel("Cube");
}

void PlayerControlScript::OnUpdate()
{
	float frame_movement_speed = (m_MovemenetSpeed / 1000.0f) * (float)Time::GetTimestep();
	float frame_rotation_speed = (m_RotationSpeed / 10000.0f) * (float)Time::GetTimestep();

	auto front = Transform->GetDirection();

	bool key_w = Input::IsKeyPressed(IFR_KEY_W);
	bool key_s = Input::IsKeyPressed(IFR_KEY_S);
	bool key_a = Input::IsKeyPressed(IFR_KEY_A);
	bool key_d = Input::IsKeyPressed(IFR_KEY_D);

	bool mouse_down = Input::IsMouseButtonPressed(MouseButton::Left);
	bool key_space = Input::IsKeyPressed(IFR_KEY_SPACE);

	Collider->Velocity.x = 0;
	Collider->Velocity.z = 0;

	if (key_w)
	{
		Collider->Velocity += front * frame_movement_speed;
	}
	else if (key_s)
	{
		Collider->Velocity -= front * frame_movement_speed;
	}

	if (key_d)
	{
		Transform->Rotation.y -= frame_rotation_speed;
	}
	else if (key_a)
	{
		Transform->Rotation.y += frame_rotation_speed;
	}

	static double ShotStartTime = 0;
	if (!m_bCanShoot)
	{
		if (Time::GetElapsedTime() - ShotStartTime > m_ReloadTime)
		{
			ShotStartTime = 0;
			m_bCanShoot = true;
		}
	}

	if (mouse_down && m_bCanShoot)
	{
		ShotStartTime = Time::GetElapsedTime();
		Shoot();
		m_bCanShoot = false;
	}
}

Entity PlayerControlScript::SpawnBullet()
{
	auto Bullet = CreateEntity();

	auto BulletTransform = reinterpret_cast<TransformComponent*>(AddComponent(Bullet, ComponentType::Transform));
	BulletTransform->Position = this->Transform->Position + (this->Transform->GetDirection() * 5.0f);
	BulletTransform->Position.y -= 2;
	BulletTransform->Scale = glm::vec3(1, 1, 1);

	auto RenderComponent = reinterpret_cast<StaticRenderComponent*>(AddComponent(Bullet, ComponentType::StaticRender));
	RenderComponent->Model = m_BulletModel;
	RenderComponent->Model.Material = m_BulletMaterial;

	auto Collider = reinterpret_cast<ColliderComponent*>(AddComponent(Bullet, ComponentType::Collider));
	Collider->IsAffectedByGravity = false;
	Collider->Velocity = this->Transform->GetDirection() * 4.0f;

	Collider->CollisionResponseCallback = ([this, Bullet](ColliderComponent& object) -> bool {
		// if the object the bullet collided with is not a tank
		if (object.Weight != 200.0f) return false;

		// if the tank is enemy tank
		if (object.Owner != m_EID)
		{
			// remove the bullet
			m_Scene->RemoveAndDeleteEntity(Bullet);

			// handle the hit
			OnEnemyHit(object.Owner);
		}
		
		return true;
	});

	return Bullet;
}

void PlayerControlScript::Shoot()
{
	Entity Bullet = SpawnBullet();

	auto Script = MakeRef<BulletScript>(Bullet, m_Scene);
	Script->BulletLifetime = 1.0f;

	auto ScriptComp = reinterpret_cast<ScriptComponent*>(AddComponent(Bullet, ComponentType::Script));
	ScriptComp->AddScript(Script);

	m_Scene->AddEntity(Bullet, { ComponentType::StaticRender, ComponentType::Collider, ComponentType::Script });
}

void PlayerControlScript::OnEnemyHit(Entity enemy)
{
	auto Info = reinterpret_cast<InformationComponent*>(GetComponent(enemy, ComponentType::Information));

	auto& Health = Info->Param1;
	auto& Team = Info->Param2;

	Health -= BulletDamage;

	if (Health < 0)
		m_Scene->RemoveAndDeleteEntity(enemy);
}
