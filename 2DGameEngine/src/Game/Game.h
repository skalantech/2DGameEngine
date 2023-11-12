#ifndef GAME_H
#define GAME_H

#include "../ECS/ECS.h"
#include "../Asset_store/Asset_store.h"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <glm/glm.hpp>

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game 
{
private:
	bool quit{};
	long double mills_prev_frame{};
	long double dt{};
	int frames{};
	int fps{};

	SDL_Window* window{};
	SDL_Renderer* renderer{};
	TTF_Font* font{};
	const Uint8* key_state{};
	Mix_Chunk* move_sound{};
	Mix_Music* back_music{};
	SDL_Event sdl_event{};
	//SDL_Rect camera{};
	Uint32 prev_ticks{};
	Uint32 curr_ticks{};
	Uint32 frame_time{};
	glm::vec2 p{};
	glm::vec2 p1{};

	std::unique_ptr<Registry> registry{};
	std::unique_ptr<Asset_store> asset_store{};

public:
	Game();
	~Game();
	void initialize();
	void run();
	void setup();
	void process_input();
	void update();
	void render();
	void destroy();

	static int windowWidth;
	static int windowHeight;
	//static int mapWidth;
	//static int mapHeight;
};

#endif
