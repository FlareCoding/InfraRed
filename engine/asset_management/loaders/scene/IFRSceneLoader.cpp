#include "IFRSceneLoader.h"
#include <rapidxml/rapidxml_print.hpp>
#include <fstream>
#include <ecs/Components.h>
using namespace rapidxml;

#include <asset_management/AssetManager.h>

namespace ifr
{
	//===========================================================================================================================================//
	//=====================================================        EXPORTING              =======================================================//
	//===========================================================================================================================================//

	static std::string ComponentTypeToString(ComponentType comp)
	{
		switch (comp)
		{
		case ComponentType::Transform:
		{
			return "Transform";
		}
		case ComponentType::StaticRender:
		{
			return "StaticRender";
		}
		case ComponentType::AnimatedRender:
		{
			return "AnimatedRender";
		}
		case ComponentType::Collider:
		{
			return "Collider";
		}
		}

		return "Unknown";
	}

	static ComponentType StringToComponentType(std::string comp)
	{
		if (comp._Equal("Transform"))			return ComponentType::Transform;
		else if (comp._Equal("StaticRender"))	return ComponentType::StaticRender;
		else if (comp._Equal("AnimatedRender")) return ComponentType::AnimatedRender;
		else if (comp._Equal("Collider"))		return ComponentType::Collider;
		else									return ComponentType::Unknown;
	}

	static void FillComponentNode(Ref<xml_document<>>& document, xml_node<>* component_node, Entity entityID, ComponentType component_type)
	{
		if (component_type == ComponentType::Transform)
		{
			TransformComponent* transform = reinterpret_cast<TransformComponent*>(GetComponent(entityID, component_type));
			std::string position_str = std::to_string(transform->Position.x) + " " + std::to_string(transform->Position.y) + " " + std::to_string(transform->Position.z);
			std::string rotation_str = std::to_string(transform->Rotation.x) + " " + std::to_string(transform->Rotation.y) + " " + std::to_string(transform->Rotation.z);
			std::string scale_str = std::to_string(transform->Scale.x) + " " + std::to_string(transform->Scale.y) + " " + std::to_string(transform->Scale.z);


			xml_node<>* position_node = document->allocate_node(node_element, "position");
			position_node->value(document->allocate_string(position_str.c_str()));
			component_node->append_node(position_node);

			xml_node<>* rotation_node = document->allocate_node(node_element, "rotation");
			rotation_node->value(document->allocate_string(rotation_str.c_str()));
			component_node->append_node(rotation_node);

			xml_node<>* scale_node = document->allocate_node(node_element, "scale");
			scale_node->value(document->allocate_string(scale_str.c_str()));
			component_node->append_node(scale_node);
		}
		else if (component_type == ComponentType::StaticRender)
		{
			StaticRenderComponent* src = reinterpret_cast<StaticRenderComponent*>(GetComponent(entityID, component_type));
			std::string model_name = src->Model.Name;
			
			xml_node<>* model_node = document->allocate_node(node_element, "model");
			model_node->value(document->allocate_string(model_name.c_str()));
			component_node->append_node(model_node);

			/*if (src->Model.Texture)
			{
				std::string texture_name = src->Model.Texture->Name;
				xml_node<>* texture_node = document->allocate_node(node_element, "texture");
				texture_node->value(document->allocate_string(texture_name.c_str()));
				component_node->append_node(texture_node);
			}*/
		}
		else if (component_type == ComponentType::AnimatedRender)
		{
			AnimatedRenderComponent* arc = reinterpret_cast<AnimatedRenderComponent*>(GetComponent(entityID, component_type));
			std::string model_name = arc->Model.Name;
			std::string animation_speed = std::to_string(arc->Model.animation.AnimationSpeed);
			std::string animation_speed_denom = std::to_string(arc->Model.animation.AnimationSpeedDenominator);

			xml_node<>* model_node = document->allocate_node(node_element, "model");
			model_node->value(document->allocate_string(model_name.c_str()));
			component_node->append_node(model_node);

			/*if (arc->Model.texture)
			{
				std::string texture_name = arc->Model.texture->Name;
				xml_node<>* texture_node = document->allocate_node(node_element, "texture");
				texture_node->value(document->allocate_string(texture_name.c_str()));
				component_node->append_node(texture_node);
			}*/

			xml_node<>* animation_speed_node = document->allocate_node(node_element, "animation_speed");
			animation_speed_node->value(document->allocate_string(animation_speed.c_str()));
			component_node->append_node(animation_speed_node);

			xml_node<>* animation_speed_denom_node = document->allocate_node(node_element, "animation_speed_denominator");
			animation_speed_denom_node->value(document->allocate_string(animation_speed_denom.c_str()));
			component_node->append_node(animation_speed_denom_node);
		}
		else if (component_type == ComponentType::Collider)
		{
			ColliderComponent* collider = reinterpret_cast<ColliderComponent*>(GetComponent(entityID, component_type));
			std::string collider_weight = std::to_string(collider->Weight);
			std::string should_render_bounding_volume = std::to_string((int)collider->ShouldRenderBoundingVolume);

			xml_node<>* weight_node = document->allocate_node(node_element, "weight");
			weight_node->value(document->allocate_string(collider_weight.c_str()));
			component_node->append_node(weight_node);

			xml_node<>* renderable_node = document->allocate_node(node_element, "renderable");
			renderable_node->value(document->allocate_string(should_render_bounding_volume.c_str()));
			component_node->append_node(renderable_node);
		}
	}

	static void CreateComponentNode(Ref<xml_document<>>& document, xml_node<>* entity_node, Entity entityID, ComponentType component_type)
	{
		xml_node<>* component_node = document->allocate_node(node_element, "component");
		component_node->append_attribute(document->allocate_attribute("name", document->allocate_string(ComponentTypeToString(component_type).c_str())));

		FillComponentNode(document, component_node, entityID, component_type);

		entity_node->append_node(component_node);
	}

	static void CreateTerrainNode(Ref<xml_document<>>& document, xml_node<>* scene_node, Ref<Terrain>& terrain)
	{
		xml_node<>* terrain_node = document->allocate_node(node_element, "terrain");

		xml_node<>* size_node = document->allocate_node(node_element, "size");
		size_node->value(document->allocate_string(std::to_string(terrain->size).c_str()));
		terrain_node->append_node(size_node);

		xml_node<>* vcount_node = document->allocate_node(node_element, "vcount");
		vcount_node->value(document->allocate_string(std::to_string(terrain->vcount).c_str()));
		terrain_node->append_node(vcount_node);

		xml_node<>* tiling_node = document->allocate_node(node_element, "tiling");
		terrain_node->append_node(tiling_node);

		xml_node<>* scale_node = document->allocate_node(node_element, "scale");
		//scale_node->value(document->allocate_string(std::to_string(terrain->scale).c_str()));
		terrain_node->append_node(scale_node);

		if (terrain->GetHeightmapRef())
		{
			xml_node<>* heightmap_node = document->allocate_node(node_element, "heightmap");
			heightmap_node->value(document->allocate_string(terrain->GetHeightmapRef()->Name.c_str()));
			terrain_node->append_node(heightmap_node);
		}

		scene_node->append_node(terrain_node);
	}

	void IFRSceneLoader::Export(Ref<Scene>& scene, const std::string& filepath)
	{
		std::ofstream file(filepath);
		Ref<xml_document<>> document = MakeRef<xml_document<>>();

		xml_node<>* decl = document->allocate_node(node_declaration);
		decl->append_attribute(document->allocate_attribute("version", "1.0"));
		decl->append_attribute(document->allocate_attribute("encoding", "utf-8"));
		document->append_node(decl);

		// Scene name
		xml_node<>* scene_root_node = document->allocate_node(node_element, "ifrscene");

		char* name = document->allocate_string(scene->GetName().c_str());
		scene_root_node->append_attribute(document->allocate_attribute("name", name));

		// Terrain
		CreateTerrainNode(document, scene_root_node, scene->m_Terrain);

		// Entities
		for (auto& [entityID, component_types] : scene->m_EntityMap)
		{
			xml_node<>* entity_node = document->allocate_node(node_element, "entity");
		
			CreateComponentNode(document, entity_node, entityID, ComponentType::Transform);

			for (auto& component_type : component_types)
				CreateComponentNode(document, entity_node, entityID, component_type);

			scene_root_node->append_node(entity_node);
		}

		document->append_node(scene_root_node);
		file << *document;
		file.close();
		IFR_LOG_SUCCESS("IFRSceneLoader> [+] Successfuly exported scene: " + filepath + " [+]");
	}

	//===========================================================================================================================================//
	//=====================================================        IMPORTING              =======================================================//
	//===========================================================================================================================================// 

	static std::vector<float> ToFloatArray(const char* float_array_str)
	{
		std::stringstream ss;
		ss.str(float_array_str);
		std::string t;

		std::vector<float> out;

		while (ss >> t)
			out.push_back(std::stof(t));

		return out;
	}

	static void ExtractTerrainData(xml_node<>* terrain_node, Ref<Scene>& scene)
	{
		uint32_t	size	= (uint32_t)std::stoi(terrain_node->first_node("size")->value());
		uint32_t	vcount	= (uint32_t)std::stoi(terrain_node->first_node("vcount")->value());
		float		tiling	= std::stof(terrain_node->first_node("tiling")->value());
		float		scale	= std::stof(terrain_node->first_node("scale")->value());
		
		Ref<Texture> MainTexture	= nullptr;
		Ref<Texture> Heightmap		= nullptr;
		Ref<Texture> Blendmap		= nullptr;
		Ref<Texture> Material1		= nullptr;
		Ref<Texture> Material2		= nullptr;
		Ref<Texture> Material3		= nullptr;

		auto* main_texture_node = terrain_node->first_node("texture");
		auto* heightmap_node	= terrain_node->first_node("heightmap");
		auto* blendmap_node		= terrain_node->first_node("blendmap");
		auto* material1_node	= terrain_node->first_node("material1");
		auto* material2_node	= terrain_node->first_node("material2");
		auto* material3_node	= terrain_node->first_node("material3");

		if (main_texture_node)	MainTexture	= Assets::FindTexture(main_texture_node->value());
		if (heightmap_node)		Heightmap	= Assets::FindTexture(heightmap_node->value());
		if (blendmap_node)		Blendmap	= Assets::FindTexture(blendmap_node->value());
		if (material1_node)		Material1	= Assets::FindTexture(material1_node->value());
		if (material2_node)		Material2	= Assets::FindTexture(material2_node->value());
		if (material3_node)		Material3	= Assets::FindTexture(material3_node->value());

		Ref<Terrain> terrain = Terrain::Create(size, vcount, Heightmap);

		scene->SetTerrain(terrain);
	}

	static void ExtractComponentData(ComponentType type, xml_node<>* node, Entity entityID, Ref<Scene>& scene)
	{
		switch (type)
		{
		case ComponentType::Transform:
		{
			auto* position_node = node->first_node("position");
			auto* rotation_node = node->first_node("rotation");
			auto* scale_node = node->first_node("scale");

			std::vector<float> position_floats  = ToFloatArray(position_node->value());
			std::vector<float> rotation_floats  = ToFloatArray(rotation_node->value());
			std::vector<float> scale_floats		= ToFloatArray(scale_node->value());

			auto* transform = reinterpret_cast<TransformComponent*>(GetComponent(entityID, type));
			transform->Position = { position_floats[0], position_floats[1], position_floats[2] };
			transform->Rotation = { rotation_floats[0], rotation_floats[1], rotation_floats[2] };
			transform->Scale	= { scale_floats[0],    scale_floats[1],    scale_floats[2] };

			break;
		}
		case ComponentType::StaticRender:
		{
			auto* model_node = node->first_node("model");
			auto* texture_node = node->first_node("texture"); // optional (not guaranteed to be present)

			auto* src = reinterpret_cast<StaticRenderComponent*>(AddComponent(entityID, type));
			src->Model = Assets::FindStaticModel(model_node->value());

			/*if (texture_node)
				src->Model.Texture = Assets::FindTexture(texture_node->value());*/

			break;
		}
		case ComponentType::AnimatedRender:
		{
			auto* model_node = node->first_node("model");
			auto* texture_node = node->first_node("texture"); // optional (not guaranteed to be present)

			auto* arc = reinterpret_cast<AnimatedRenderComponent*>(AddComponent(entityID, type));
			arc->Model = Assets::FindAnimatedModel(model_node->value());

			/*if (texture_node)
				arc->Model.texture = Assets::FindTexture(texture_node->value());*/

			auto* anim_speed_node = node->first_node("animation_speed");
			auto* anim_speed_denom_node = node->first_node("animation_speed_denominator");

			arc->Model.animation.AnimationSpeed = std::stof(anim_speed_node->value());
			arc->Model.animation.AnimationSpeedDenominator = std::stof(anim_speed_denom_node->value());

			arc->Model.animation.Play();

			break;
		}
		case ComponentType::Collider:
		{
			auto* weight_node = node->first_node("weight");
			auto* renderable_node = node->first_node("renderable");

			auto* collider = reinterpret_cast<ColliderComponent*>(AddComponent(entityID, type));
			collider->Weight = std::stof(weight_node->value());
			collider->ShouldRenderBoundingVolume = (bool)std::stoi(renderable_node->value());

			break;
		}
		default:
			break;
		}
	}

	static void ExtractEntityData(xml_node<>* entity_node, Ref<Scene>& scene)
	{
		Entity entity = CreateEntity();
		std::vector<ComponentType> component_types;

		for (xml_node<>* component_node = entity_node->first_node("component"); component_node; component_node = component_node->next_sibling("component"))
		{
			ComponentType component_type = StringToComponentType(component_node->first_attribute("name")->value());
			ExtractComponentData(component_type, component_node, entity, scene);

			component_types.push_back(component_type);
		}

		scene->AddEntity(entity, component_types);
	}

	Ref<Scene> IFRSceneLoader::Import(const std::string& filepath)
	{
		Ref<xml_document<>> documents = MakeRef<xml_document<>>();
		std::ifstream file(filepath);

		std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		buffer.push_back('\0');
		documents->parse<0>(&buffer[0]);
	
		xml_node<>* scene_node = documents->first_node();

		// Scene name
		std::string scene_name = scene_node->first_attribute("name")->value();
		Ref<Scene> scene = Scene::Create(scene_name, nullptr, nullptr);

		// Terrain
		ExtractTerrainData(scene_node->first_node("terrain"), scene);

		// Entities
		for (xml_node<>* entity_node = scene_node->first_node("entity"); entity_node; entity_node = entity_node->next_sibling("entity"))
		{
			ExtractEntityData(entity_node, scene);
		}

		file.close();

		IFR_LOG_SUCCESS("IFRSceneLoader> [+] Loaded scene: " + filepath + " [+]");
		return scene;
	}
}
