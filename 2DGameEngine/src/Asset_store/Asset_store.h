#ifndef ASSETSTORE_H
#define ASSETSTORE_H

#include <map>
#include <string>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

class Asset_store
{
private:
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, TTF_Font*> fonts;
	std::map<std::string, Mix_Chunk*> sounds;

public:
	Asset_store();
	~Asset_store();

	void clear_assets();
	void add_texture(SDL_Renderer* renderer, const std::string& asset_id, const std::string& file_path);
	SDL_Texture* get_texture(const std::string& asset_id);


};
#endif
