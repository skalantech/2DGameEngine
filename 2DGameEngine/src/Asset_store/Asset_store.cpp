#include "./Asset_store.h"
#include "../Logger/Logger.h"

#include <SDL2/SDL_image.h>

Asset_store::Asset_store() 
{
	Logger::log("Asset_store constructor called!");
}

Asset_store::~Asset_store() 
{
	clear_assets();
	Logger::log("Asset_store destructor called!");

}

void Asset_store::clear_assets() 
{
	for (auto texture : textures)
	{
		SDL_DestroyTexture(texture.second);
	}
	textures.clear();
}

void Asset_store::add_texture(SDL_Renderer* renderer, const std::string& asset_id, const std::string& file_path) 
{
	SDL_Surface* surface = IMG_Load(file_path.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	// Add the texture to the map
	textures.emplace(asset_id, texture);

	Logger::log("New texture added to the asset store with id = " + asset_id);
}

SDL_Texture* Asset_store::get_texture(const std::string& asset_id) 
{
	return textures[asset_id];
}