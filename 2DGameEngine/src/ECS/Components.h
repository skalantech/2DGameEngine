#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <glm/glm.hpp>
#include <string>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

struct Rigid_body_comp 
{
    glm::vec2 vel;

    Rigid_body_comp(glm::vec2 velocity = glm::vec2(0.0, 0.0)) : vel{ velocity } {}

};

struct Sprite_comp 
{
	std::string asset_id;
	int width;
	int height;
	SDL_Rect src_rect;

	Sprite_comp(std::string asset_id = "", int width = 0, int height = 0, int src_rectX = 0, int src_rectY = 0) : 
		asset_id{ asset_id }, 
		width{ width }, 
		height{ height }, 
		src_rect{src_rectX, src_rectY, width, height} {}
};

struct Transform_comp
{
	glm::vec2 pos;
	glm::vec2 scale;
	double rot;

	Transform_comp(glm::vec2 position = glm::vec2(0, 0), glm::vec2 scale = glm::vec2(1, 1), double rotation = 0.0) 
	{
		this->pos = position;
		this->scale = scale;
		this->rot = rotation;
	}
};
#endif
