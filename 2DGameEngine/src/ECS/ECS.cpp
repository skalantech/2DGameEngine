#include "ECS.h"
#include "../Logger/Logger.h"
#include <algorithm>

int Icomp::next_id = 0;     // class' static fields have to be initialized

int Entity::get_id() const { return id; }

void System::add_ent_to_sys(Entity ent) { ents.push_back(ent); }

void System::rm_ent_from_sys(Entity ent) 
{
	ents.erase(std::remove_if(ents.begin(),
		ents.end(),
		[&ent](Entity other) { return ent == other; } ),
		ents.end());
}

std::vector<Entity> System::get_sys_ents() const { return ents; }

const Signature& System::get_comp_sign() const { return comp_sign; }

Entity Registry::create_ent() {
	int ent_id = num_ents++;

	// if there are no free ids waiting to be reused
	if (ent_id >= ent_comp_signs.size()) 
	{
		ent_comp_signs.resize(ent_id + 1);
	}
	
	Entity ent(ent_id);
	ent.reg = this;
	ents_to_add.insert(ent);

	Logger::log("Entity created with id " + std::to_string(ent_id));

	return ent;
}

void Registry::add_ent_to_syss(Entity ent) {
	const auto ent_id = ent.get_id();

	const auto& ent_comp_sign = ent_comp_signs[ent_id];

	for (auto& system : systems) 
	{   // system is an iterator pointer not an object
		const auto& sys_comp_sign = system.second->get_comp_sign();

		// "And" bitwise comparison between the two bitsets to see is the system component signature and the
		// entity component signature are compatible
		bool is_interested = (ent_comp_sign & sys_comp_sign) == sys_comp_sign;

		if (is_interested) 
		{
			system.second->add_ent_to_sys(ent);
		}
	}
}

void Registry::update() {
	// Here is where we actually insert/delete the entities that are waiting to be added/removed.
	// We do this because we don't want to confuse our Systems by adding/removing entities in the middle
	// of the frame logic. Therefore, we wait until the end of the frame to update and perform the
	// creation and deletion of entities.


	// Add the entities that are waiting to be created to the active Systems
	for (auto ent : ents_to_add) 
	{
		add_ent_to_syss(ent);
	}
	ents_to_add.clear();

	// Remove the entities that are waiting to be killed from the active Systems

}
