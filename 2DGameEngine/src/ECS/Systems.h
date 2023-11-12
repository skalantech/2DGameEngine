#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "../ECS/ECS.h"
#include "../ECS/Components.h"
#include "../Asset_store/Asset_store.h"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


class Movement_system : public System 
{
public:
    Movement_system()
    {
        req_comp<Transform_comp>();
        req_comp<Rigid_body_comp>();
    }

    void update(double dt) 
    {
        // Loop all entities that the system is interested in
        for (auto ent : get_sys_ents()) {
            // Update entity position based on its velocity
            auto& transform = ent.get_comp<Transform_comp>();
            const auto rigidbody = ent.get_comp<Rigid_body_comp>();

            transform.pos.x += rigidbody.vel.x * dt;
            transform.pos.y += rigidbody.vel.y * dt;
        }
    }
};

class Render_system : public System 
{
public:
    Render_system() 
    {
        req_comp<Transform_comp>();
        req_comp<Sprite_comp>();
    }

    void update(SDL_Renderer* renderer, std::unique_ptr<Asset_store>& asset_store) {
        // Loop all entities that the system is interested in
        for (auto ent : get_sys_ents()) {
            const auto& transform = ent.get_comp<Transform_comp>();
            const auto sprite = ent.get_comp<Sprite_comp>();



            // Set the source rectangle of our original sprite texture
            SDL_Rect src_rect = sprite.src_rect;

            // Set the destination rectangle of
            SDL_Rect dst_rect = {
                static_cast<int>(transform.pos.x),
                static_cast<int>(transform.pos.y),
                static_cast<int>(sprite.width * transform.scale.x),
                static_cast<int>(sprite.height * transform.scale.y)
            };

            SDL_RenderCopyEx(renderer, asset_store->get_texture(sprite.asset_id), &src_rect, &dst_rect, transform.rot, NULL, SDL_FLIP_NONE);

            //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            //SDL_RenderFillRect(renderer, &obj_rect);

            // Draw a PNG texture
        }
    }
};

#endif