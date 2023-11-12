#include "Game.h"
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include "../ECS/Components.h"
#include "../ECS/Systems.h"


// lib


#include <glm/glm.hpp>
//#include <imgui/imgui.h>
//#include <imgui/imgui_sdl.h>
//#include <imgui/imgui_impl_sdl.h>

//std
#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>

int Game::windowWidth;
int Game::windowHeight;


Game::Game() 
{
    quit = false;
    registry = std::make_unique<Registry>();
    asset_store = std::make_unique<Asset_store>();
    Logger::log("Game constructor called!");
}


Game::~Game() 
{
    Logger::log("Game destructor called!");   
}


void Game::initialize() 
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
    {
        Logger::err("Error initializing SDL.");
        return;
    }
    if (TTF_Init() != 0) 
    {
        Logger::err("Error initializing SDL TTF.");
        return;
    }
    if (Mix_Init(MIX_INIT_OGG) == 0) 
    {
        Logger::err("Error initializing SDL Mix");
        return;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) 
    {
        Logger::err("Error initializing SDL Mix OpenAudio");
        return;
    }

    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    windowWidth = display_mode.w;
    windowHeight = display_mode.h;
    window = SDL_CreateWindow(
        "2D game engine", //NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth / 2,
        windowHeight / 2,
        SDL_WINDOW_SHOWN//SDL_WINDOW_ALLOW_HIGHDPI//SDL_WINDOW_BORDERLESS
    );
    if (!window) 
    {
        Logger::err("Error creating SDL window.");
        return;
    }

    renderer = SDL_CreateRenderer(
        window, 
        -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);  
    if (!renderer) 
    {
        Logger::err("Error creating SDL renderer.");
        return;
    }
    //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    // Initialize the ImGui context
    //ImGui::CreateContext();
    //ImGuiSDL::Initialize(renderer, windowWidth, windowHeight);

  
    // Initialize the camera view with the entire screen area
    //camera.x = 0;
    //camera.y = 0;
    //camera.w = windowWidth;
    //camera.h = windowHeight;
}





void Game::setup() {
    move_sound = Mix_LoadWAV("./assets/sounds/helicopter.wav");
    if (!move_sound) {
        std::cerr << "Mix_LoadWAV failed: " << Mix_GetError() << '\n';
        return;
    }

    back_music = Mix_LoadMUS("./assets/sounds/music.wav");
    if (!back_music) 
    {
        std::cerr << "Mix_LoadMUS failed: " << Mix_GetError() << '\n';
        return;
    }

    font = TTF_OpenFont("./assets/fonts/arial.ttf", 18);
    if (!font) 
    {
        std::cerr << "TTF_OpenFont failed: " << TTF_GetError() << '\n';
        return;
    }

    Mix_PlayMusic(back_music, -1);
    Mix_VolumeMusic(1);
    Mix_Volume(-1, 0);

    glm::vec2 p = glm::vec2(10.0, 20.0);
    glm::vec2 p1 = glm::vec2(100.0, 20.0);
     
    // Add the systems that need to be processed in our game
    registry->add_sys<Movement_system>();
    registry->add_sys<Render_system>();

    // Adding assets to the asset store
    asset_store->add_texture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
    asset_store->add_texture(renderer, "truck-image", "./assets/images/truck-ford-right.png");

    // Create an entity
    Entity tank = registry->create_ent();
    tank.add_comp<Transform_comp>(glm::vec2(10.0, 30.0), glm::vec2(2.0, 2.0), 0.0);
    tank.add_comp<Rigid_body_comp>(glm::vec2(40.0, 0.0));
    tank.add_comp<Sprite_comp>("tank-image", 32, 32);

    Entity truck = registry->create_ent();
    truck.add_comp<Transform_comp>(glm::vec2(50.0, 100.0), glm::vec2(2.0, 2.0), 0.0);
    truck.add_comp<Rigid_body_comp>(glm::vec2(0.0, 50.0));
    truck.add_comp<Sprite_comp>("truck-image", 32, 32);

    key_state = SDL_GetKeyboardState(NULL);
    prev_ticks = SDL_GetTicks64();
}


void Game::update() {
    curr_ticks = SDL_GetTicks64();
    // If we are too fast, waste some time until we reach the MILLISECS_PER_FRAME
    int time_wait = MILLISECS_PER_FRAME - (SDL_GetTicks64() - mills_prev_frame);
    if (time_wait > 0 && time_wait <= MILLISECS_PER_FRAME) {
        SDL_Delay(time_wait);
    }

    // Calculate FPS
    frame_time = SDL_GetTicks64() - prev_ticks;
    if (frame_time >= 1000) {
        fps = frames;
        frames = 0;
        prev_ticks = SDL_GetTicks64();
    }
    frames++;
    /*printf("fps: %d", fps);*/
    // The difference in ticks since the last frame, converted to seconds
    dt = SDL_GetTicks64() - mills_prev_frame;
    
    // Store the "previous" frame time
    mills_prev_frame = SDL_GetTicks64();

    // Update the registry to process the entities that are waiting to be created/deleted
    registry->update();

    // Invoke all the systems that need to update 
    registry->get_sys<Movement_system>().update(dt / 1000);
}


void Game::process_input() {

    while (SDL_PollEvent(&sdl_event)) {
        // ImGui SDL input
        //ImGui_ImplSDL2_ProcessEvent(&sdl_event);
        //ImGuiIO& io = ImGui::GetIO();
        //int mouseX, mouseY;

        //const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
        //io.MousePos = ImVec2(mouseX, mouseY);
        //io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
        //io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

        // Handle SDL core events (close window, key pressed, etc.)
        if (sdl_event.type == SDL_QUIT) {
            quit = true;
        }
        if (sdl_event.type == SDL_KEYDOWN) {
            if (sdl_event.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }
        }
    }
    if (key_state[SDL_SCANCODE_W]) {
        p1.y -= dt / 5;
    }
    if (key_state[SDL_SCANCODE_S]) {
        p1.y += dt / 5;
    }
    if (key_state[SDL_SCANCODE_A]) {
        p1.x -= dt / 5;
    }
    if (key_state[SDL_SCANCODE_D]) {
        p1.x += dt / 5;
    }
}                


void Game::render() {
        SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
        SDL_RenderClear(renderer);

        // Draw FPS text
        SDL_Color textColor = { 0, 255, 0, 255 };
        std::string fpsText = "FPS: " + std::to_string(fps);
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, fpsText.c_str(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = { 10, 10, textSurface->w, textSurface->h };

        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);

        // Draw a PNG texture
        SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, surface);
        
        p.x += dt / 5;
        p.y += dt / 5;

        if (p.x > windowWidth) {
            p.x = 0;
        }
        if (p.y > windowHeight) {
            p.y = 0;
        }
        if (p1.x > windowWidth) {
            p1.x = 0;
        }
        if (p1.x < 0) {
            p1.x = windowWidth;
        }
        if (p1.y > windowHeight) {
            p1.y = 0;
        }
        if (p1.y < 0) {
            p1.y = windowHeight;
        }
 
        SDL_Rect dstRect = {
            static_cast<int>(p.x),
            static_cast<int>(p.y),
            32, 32 };

        SDL_Rect dstRect1 = {
        static_cast<int>(p1.x),
        static_cast<int>(p1.y),
        32, 32 };

        SDL_RenderCopy(renderer, texture, NULL, &dstRect);
        SDL_RenderCopy(renderer, texture1, NULL, &dstRect1);
        SDL_DestroyTexture(texture);
        SDL_DestroyTexture(texture1);
        SDL_FreeSurface(surface);

        registry->get_sys<Render_system>().update(renderer, asset_store);

        SDL_RenderPresent(renderer);
}


void Game::run() {
    setup();
    while (!quit) {
        update();
        process_input();
        render();
    }
}


void Game::destroy() {
    //ImGuiSDL::Deinitialize();
    //ImGui::DestroyContext();
    Mix_FreeChunk(move_sound);
    Mix_FreeMusic(back_music);
    Mix_CloseAudio();
    Mix_Quit();
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
