#ifndef ECS_H
#define ECS_H

#include "../Logger/Logger.h"
#include <vector>
#include <bitset>
#include <set>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <iostream>


const unsigned int MAX_COMPS = 32;

////////////////////////////////////////////////////////////////////////////////
// Signature
////////////////////////////////////////////////////////////////////////////////
// We use a bitset (1s and 0s) to keep track of which components an entity has,
// and also helps keep track of which entities a system is interested in.
////////////////////////////////////////////////////////////////////////////////
typedef std::bitset<MAX_COMPS> Signature;

struct Icomp 
{
protected:
	static int next_id;
};

// Used to assign a unique id to a component type
template <typename T>       // Placeholder for the several different classes that will be created in compilation
class Component : public Icomp 
{
public:
	// Returns the unique id of Component<T>
	static int get_id() {
		static auto id = next_id++;
		return id;
	}
};

////////////////////////////////////////////////////////////////////////////////
// Entity
////////////////////////////////////////////////////////////////////////////////
class Entity 
{
private:
	int id;

public:
	Entity(int id) : id(id) {};
	Entity(const Entity& ent) = default;  // copy constructor
	int get_id() const;

	// Manage entity tags and groups


	// Operator overloading for entity objects
	Entity& operator =(const Entity& other) = default;      // copy assignment
	bool operator ==(const Entity& other) const { return id == other.id; }
	bool operator !=(const Entity& other) const { return id != other.id; }
	bool operator >(const Entity& other) const { return id > other.id; }
	bool operator <(const Entity& other) const { return id < other.id; }

	// Manage entity components
	template <typename Tcomp, typename ...Targs> void add_comp(Targs&& ...args);
	template <typename Tcomp> void rm_comp();
	template <typename Tcomp> bool has_comp() const;
	template <typename Tcomp> Tcomp& get_comp() const;


	// Hold a pointer to the entity's owner registry
	class Registry* reg{};
};

////////////////////////////////////////////////////////////////////////////////
// System
////////////////////////////////////////////////////////////////////////////////
// The system processes entities that contain a specific signature
////////////////////////////////////////////////////////////////////////////////
class System 
{
private:
	Signature comp_sign;
	std::vector<Entity> ents;

public:
	System() = default;
	~System() = default;

	void add_ent_to_sys(Entity ent);
	void rm_ent_from_sys(Entity ent);
	std::vector<Entity> get_sys_ents() const;
	const Signature& get_comp_sign() const;

	// Defines the component type that entities must have to be considered by the system
	template <typename Tcomp> void req_comp();
};

////////////////////////////////////////////////////////////////////////////////
// Pool
////////////////////////////////////////////////////////////////////////////////
// A pool is just a vector (contiguous data) of objects of type T
////////////////////////////////////////////////////////////////////////////////
class Ipool 
{
public:
	virtual ~Ipool() = default;
};

template <typename T>
class Pool : public Ipool 
{
private:
	// We keep track of the vector of objects and the current number of elements
	std::vector<T> data;

	// Helper maps to keep track of entity ids per index, so the vector is always packed

public:
	Pool(int cap = 100)	{ data.resize(cap); }

	virtual ~Pool() = default;

	bool is_empty() const {	return data.empty(); }

	int get_size() const { return data.size();	}

	void resize(int n) { data.resize(n); }

	void clear() { data.clear(); }

	void add(T obj) { data.push_back(obj); }

	void set(int idx, T obj) { data[idx] = obj;	}

	T& get(int idx) { return static_cast<T&>(data[idx]); }

	T& operator [](unsigned int idx) { return data[idx]; }
};

////////////////////////////////////////////////////////////////////////////////
// Registry
////////////////////////////////////////////////////////////////////////////////
// The registry manages the creation and destruction of entities, add systems,
// and components.
////////////////////////////////////////////////////////////////////////////////
class Registry 
{
private:
	int num_ents = 0;

	// Vector of component pools, each pool contains all the data for a certain compoenent type
	// [Vector index = component type id]
	// [Pool index = entity id]
	std::vector<std::shared_ptr<Ipool>> comp_pools;     // creating IPool so that we don't have to specify the type of Pool (Polymorphism)

	// Vector of component signatures per entity, saying which component is turned "on" for a given entity
	// [Vector index = entity id]
	std::vector<Signature> ent_comp_signs;

	// Map of active systems
	// [Map key = system type id]
	std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

	// Set of entities that are flagged to be added or removed in the next registry Update()
	std::set<Entity> ents_to_add;
	std::set<Entity> ents_to_kill;

	// Entity tags (one tag name per entity)


	// Entity groups (a set of entities per group name)


	// List of free entity ids that were previously removed


public:
	Registry() { Logger::log("Registry constructor called"); }

	~Registry() { Logger::log("Registry destructor called"); }

	// The registry Update() finally processes the entities that are waiting to be added/killed to the systems
	void update();

	// Entity management
	Entity create_ent();


	// Tag management


	// Group management


	// Component management
	template <typename Tcomp, typename ...Targs> void add_comp(Entity ent, Targs&& ...args);
	template <typename Tcomp> void rm_comp(Entity ent);
	template <typename Tcomp> bool has_comp(Entity ent) const;
	template <typename Tcomp> Tcomp& get_comp(Entity ent) const;

	// System management
	template <typename Tsys, typename ...Targs> void add_sys(Targs&& ...args);
	template <typename Tsys> void rm_sys();
	template <typename Tsys> bool has_sys() const;
	template <typename Tsys> Tsys& get_sys() const;

	// Checks the component signature of an entity and add or remove the entity to the systems
	// that are interested in it
	void add_ent_to_syss(Entity ent);
};

// Template function implementation
// Template functions are not real functions yet, they are placeholder for the functions of different types
// that the compiler is going to create after compilation

// System
template <typename Tcomp>
void System::req_comp() 
{
	const auto comp_id = Component<Tcomp>::get_id();
	comp_sign.set(comp_id);
}

// Registry
template <typename Tsys, typename ...Targs>
void Registry::add_sys(Targs&& ...args) 
{
	std::shared_ptr<Tsys> new_sys = std::make_shared<Tsys>(std::forward<Targs>(args)...);
	systems.insert(std::make_pair(std::type_index(typeid(Tsys)), new_sys));
}

template <typename Tsys>
void Registry::rm_sys() 
{
	auto system = systems.find(std::type_index(typeid(Tsys))); // system is an iterator pointer not an object
	systems.erase(system);
}

template <typename Tsys>
bool Registry::has_sys() const 
{
	return systems.find(std::type_index(typeid(Tsys))) != systems.end();
}

template <typename Tsys>
Tsys& Registry::get_sys() const 
{
	auto system = systems.find(std::type_index(typeid(Tsys)));
	return *(std::static_pointer_cast<Tsys>(system->second));
}

template <typename Tcomp, typename ...Targs>
void Registry::add_comp(Entity ent, Targs&& ...args) 
{
	const auto comp_id = Component<Tcomp>::get_id();
	const auto ent_id = ent.get_id();

	// If the component id is greater than the current size of the componentPools, then resize the vector
	if (comp_id >= comp_pools.size()) 
	{
		comp_pools.resize(comp_id + 1, nullptr);    // resizing a vector is very costly it is better to allocate more memory from the getgo!
	}

	// If we still don't have a Pool for that component type
	if (!comp_pools[comp_id]) 
	{
		std::shared_ptr<Pool<Tcomp>> new_comp_pool(new Pool<Tcomp>()); // = std::make_shared<Pool<Tcomp>>();
		comp_pools[comp_id] = new_comp_pool;
	}

	// Get the pool of component values for that component type
	std::shared_ptr<Pool<Tcomp>> comp_pool = std::static_pointer_cast<Pool<Tcomp>>(comp_pools[comp_id]);

	// If the entity id is greater than the current size of the component pool, then resize the pool
	if (ent_id >= comp_pool->get_size()) 
	{
		comp_pool->resize(num_ents);
	}

	// Create a new Component object of the type T, and forward the various parameters to the constructor
	Tcomp new_comp(std::forward<Targs>(args)...);

	// Add the new component to the component pool list, using the entity id as index
	comp_pool->set(ent_id, new_comp);

	// Finally, change the component signature of the entity and set the component id on the bitset to 1
	ent_comp_signs[ent_id].set(comp_id);

	Logger::log("Component id = " + std::to_string(comp_id) + " was added to entity id " + std::to_string(ent_id));

	//std::cout << "COMPONENT ID " << comp_id << " --> POOL SIZE: " << comp_pool->get_size() << std::endl;
}

template <typename Tcomp>
void Registry::rm_comp(Entity ent) 
{
	const auto comp_id = Component<Tcomp>::get_id();
	const auto ent_id = ent.get_id();

	// Remove the component from the component list for that entity

	//Set this component signature for that entity to false
	ent_comp_signs[ent_id].set(comp_id, false);

	Logger::Log("Component id = " + std::to_string(comp_id) + " was removed from entity id " + std::to_string(ent_id));
}

template <typename Tcomp> 
bool Registry::has_comp(Entity ent) const
{
	const auto comp_id = Component<Tcomp>::get_id();
	const auto ent_id = ent.get_id();
	return ent_comp_signs[ent_id].test(comp_id);
}

template <typename Tcomp>
Tcomp& Registry::get_comp(Entity ent) const {
	const auto comp_id = Component<Tcomp>::get_id();
	const auto ent_id = ent.get_id();
	auto comp_pool = std::static_pointer_cast<Pool<Tcomp>>(comp_pools[comp_id]);
	return comp_pool->get(ent_id);
}

// Entity
template <typename Tcomp, typename ...Targs>
void Entity::add_comp(Targs&& ...args) {
	reg->add_comp<Tcomp>(*this, std::forward<Targs>(args)...);
}

template <typename Tcomp>
void Entity::rm_comp() {
	reg->rm_comp<Tcomp>(*this);
}

template <typename Tcomp>
bool Entity::has_comp() const {
	return reg->has_comp<Tcomp>(*this);
}

template <typename Tcomp>
Tcomp& Entity::get_comp() const {
	return reg->get_comp<Tcomp>(*this);
}

#endif
