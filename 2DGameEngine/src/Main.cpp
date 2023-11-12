#include "./Game/Game.h"

#include <sol/sol.hpp>
#include <iostream>


int nativeCppCubeFunction(int n) {
    return n * n * n;
}


void test_lua() {
    sol::state lua;
    lua.open_libraries(sol::lib::base);

    // This is how to expose and bind a C++ function to be used by the Lua script
    lua["cube"] = nativeCppCubeFunction;

    lua.script_file("./assets/scripts/myscript.lua");

    //this is how we get the value of a global variable from our Lua script
    int someVariableInsideCpp = lua["some_variable"];
    std::cout << "The value of some_variable in C++ is " << someVariableInsideCpp << std::endl;

    // this is how we can get the values from inside a Lua table
    bool isFullScreen = lua["config"]["fullscreen"];
    sol::table config = lua["config"];
    int width = config["resolution"]["width"];
    int height = config["resolution"]["height"];
    std::cout << "We read the value fullscreen: " << isFullScreen << ", width: " << width << ", height: " << height << std::endl;

    // this is how to invoke a Lua function from inside C++
    sol::function  functionFactorial = lua["factorial"];
    int functionResult = functionFactorial(5);
    std::cout << "The Lua script calculated 5! as " << functionResult << std::endl;
}


int main(int argc, char* argv[]) {
    Game game;

    game.initialize();
    game.run();
    game.destroy();

    
    return 0;
}

//#define SDL_MAIN_HANDLED
//#include <SDL.h>
//#include <SDL_image.h>
//#include <SDL_mixer.h>
//#include <SDL_ttf.h>
//#include <glm/glm.hpp>
//#include <iostream>
//#include <string>
//
//
//const int SCREEN_WIDTH = 1600;
//const int SCREEN_HEIGHT = 1280;
//
//int main(int argc, char* argv[]) {
//    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
//        std::cerr << "SDL_Init failed: " << SDL_GetError() << '\n';
//        return 1;
//    }
//    if (TTF_Init() == -1) {
//        std::cerr << "TTF_Init failed: " << TTF_GetError() << '\n';
//        return 1;
//    }
//
//    if (Mix_Init(MIX_INIT_OGG) == 0) {
//        std::cerr << "Mix_Init failed: " << Mix_GetError() << '\n';
//        return 1;
//    }
//
//    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
//        std::cerr << "Mix_OpenAudio failed: " << Mix_GetError() << '\n';
//    }
//
//    SDL_Window* window = SDL_CreateWindow(
//        "Moving Cube Game", 
//        SDL_WINDOWPOS_UNDEFINED, 
//        SDL_WINDOWPOS_UNDEFINED, 
//        SCREEN_WIDTH, 
//        SCREEN_HEIGHT, 
//        SDL_WINDOW_SHOWN);
//    
//    SDL_Renderer* renderer = SDL_CreateRenderer(
//        window, 
//        -1, 
//        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
//    
//    Mix_Chunk* moveSound = Mix_LoadWAV("./assets/sounds/helicopter.wav");
//    if (!moveSound) {
//        std::cerr << "Mix_LoadWAV failed: " << Mix_GetError() << '\n';
//        return 1;
//    }
//
//    Mix_Music* backgroudMusic = Mix_LoadMUS("./assets/sounds/music.wav");
//    if (!backgroudMusic) {
//        std::cerr << "Mix_LoadMUS failed: " << Mix_GetError() << '\n';
//        return 1;
//    }
//
//    TTF_Font* font = TTF_OpenFont("./assets/fonts/arial.ttf", 18);
//    if (!font) {
//        std::cerr << "TTF_OpenFont failed: " << TTF_GetError() << '\n';
//        return 1;
//    }
//
//    glm::vec2 p = glm::vec2(10.0, 20.0);
//    glm::vec2 p1 = glm::vec2(100.0, 20.0);
//
//    SDL_Event event;
//
//    bool quit = false;
//    long double dt;
//    long double millisecsPreviousFrame = 0.0;
//
//    int frames = 0;
//    int fps = 0;
//
//    Mix_PlayMusic(backgroudMusic, -1);
//    Mix_VolumeMusic(5);
//    Mix_Volume(-1, 5);
//    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
//    Uint32 prevTicks = SDL_GetTicks64();
//    while (!quit) {
//      
//        // Calculate FPS
//        Uint32 currentTicks = SDL_GetTicks64();
//        Uint32 frameTime = currentTicks - prevTicks;
//        if (frameTime >= 1000) {
//            fps = frames;
//            frames = 0;
//            prevTicks = currentTicks;
//        }
//        frames++;
//
//        dt = currentTicks - millisecsPreviousFrame;
//        millisecsPreviousFrame = currentTicks;
//
//        while (SDL_PollEvent(&event) != 0) {
//            if (event.type == SDL_QUIT) {
//                quit = true;
//            }
//            if (event.type == SDL_KEYDOWN) {
//                if (event.key.keysym.sym == SDLK_ESCAPE) {
//                    quit = true;
//                }
//            }
//        }
//
//        
//        if (keyboardState[SDL_SCANCODE_W]) {
//            p1.y -= dt / 5;
//            Mix_PlayChannel(-1, moveSound, 0);
//        }
//        if (keyboardState[SDL_SCANCODE_S]) {
//            p1.y += dt / 5;
//            Mix_PlayChannel(-1, moveSound, 0);
//        }
//        if (keyboardState[SDL_SCANCODE_A]) {
//            p1.x -= dt / 5;
//            Mix_PlayChannel(-1, moveSound, 0);
//        }
//        if (keyboardState[SDL_SCANCODE_D]) {
//            p1.x += dt / 5;
//            Mix_PlayChannel(-1, moveSound, 0);
//        }
//
//        SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
//        SDL_RenderClear(renderer);
//
//        // Draw FPS text
//        SDL_Color textColor = { 0, 255, 0, 255 };
//        std::string fpsText = "FPS: " + std::to_string(fps);
//        SDL_Surface* textSurface = TTF_RenderText_Solid(font, fpsText.c_str(), textColor);
//        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
//        SDL_Rect textRect = { 10, 10, textSurface->w, textSurface->h };
//
//        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
//        SDL_DestroyTexture(textTexture);
//        SDL_FreeSurface(textSurface);
//
//        // Draw a PNG texture
//        SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
//        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
//        SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, surface);
//        
//        p.x += dt / 5;
//        p.y += dt / 5;
//
//        if (p.x > SCREEN_WIDTH) {
//            p.x = 0;
//        }
//        if (p.y > SCREEN_HEIGHT) {
//            p.y = 0;
//        }
//        if (p1.x > SCREEN_WIDTH) {
//            p1.x = 0;
//        }
//        if (p1.x < 0) {
//            p1.x = SCREEN_WIDTH;
//        }
//        if (p1.y > SCREEN_HEIGHT) {
//            p1.y = 0;
//        }
//        if (p1.y < 0) {
//            p1.y = SCREEN_HEIGHT;
//        }
// 
//        SDL_Rect dstRect = {
//            static_cast<int>(p.x),
//            static_cast<int>(p.y),
//            32, 32 };
//
//        SDL_Rect dstRect1 = {
//        static_cast<int>(p1.x),
//        static_cast<int>(p1.y),
//        32, 32 };
//
//        SDL_RenderCopy(renderer, texture, NULL, &dstRect);
//        SDL_RenderCopy(renderer, texture, NULL, &dstRect1);
//        SDL_DestroyTexture(texture);
//        SDL_DestroyTexture(texture1);
//        SDL_FreeSurface(surface);
//
//        SDL_RenderPresent(renderer);
//    }
//
//    Mix_FreeChunk(moveSound);
//    Mix_FreeMusic(backgroudMusic);
//    Mix_CloseAudio();
//    Mix_Quit();
//
//    TTF_CloseFont(font);
//    TTF_Quit();
//
//    SDL_DestroyRenderer(renderer);
//    SDL_DestroyWindow(window);
//    SDL_Quit();
//
//    return 0;
//}
