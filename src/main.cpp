// Copyright (C) 2021 Neal Nicdao
//
// Licensed under the Holy Open Software License of the Computer Gods V1
//
// This software comes with ABSOLUTELY NO WARANTEEEEE. By plagiarizing the
// work, you agree to the HIGHEST POSSIBLE PUNISHMENT from the the might of the
// COMPUTER GODS through rolling the DICE.


#include "raylib.h"

#include "firstpersonshooter/fpsand.h"
#include "firstpersonshooter/components/fundamental.h"
#include "firstpersonshooter/components/appearance.h"

#include<thread>
#include <optional>
#include <iostream>

std::optional<fpsand::World> g_world;

struct Foo
{
    int hi_uwu;
};

int what()
{
    std::cout << "hi\n";
    return 10;
}

void test_parallel()
{
    fpsand::TaskRunner<fpsand::TagFPSand, std::function<int(void)> > runner;
    fpsand::TagRegistry<fpsand::TagFPSand> tags;

    fpsand::TaskRunner<fpsand::TagFPSand, std::function<int(void)> >::Task task{
        {tags.id(fpsand::tag_name("hello"))}, {}, {}, &what, false };

    runner.set_max_tags(tags.count());
    runner.queue( {task} );

    auto op = runner.find_available();
    if (op.has_value())
    {
        runner.do_task(op.value());
    }

}

int main(int argc, char** argv)
{
    unsigned int const maxSingletons = 20;

    g_world.emplace(maxSingletons);

    for (fpsand::coord_t y = 30; y < 60; y ++)
    {
        for (fpsand::coord_t x = 30; x < 60; x ++)
        {
            fpsand::Ent ent = g_world->create();
            g_world->emplace<fpsand::CmpPosition>(ent, fpsand::CmpPosition{{x, y}});
            g_world->emplace<fpsand::CmpColor>(ent, fpsand::CmpColor{0, 0, 255});
        }
    }

    const int screenWidth = 128;
    const int screenHeight = 128;
    const int screenScale = 5;

    SetAudioStreamBufferSizeDefault(14400 / 60 * 6);
    InitWindow(screenWidth * screenScale, screenHeight * screenScale, "Nice");

    SetTargetFPS(60);

    RenderTexture2D target = LoadRenderTexture(screenWidth,  screenHeight);
    BeginDrawing();
    
    BeginTextureMode(target);

    ClearBackground(CLITERAL(Color){ 0, 0, 0, 255 });
    
    EndTextureMode();

    EndDrawing();
    
    while (!WindowShouldClose())
    {

        BeginDrawing();

        ClearBackground(CLITERAL(Color){ 0, 0, 0, 255 });

        BeginTextureMode(target);
        
        auto viewDrawables = g_world->view<fpsand::CmpPosition, fpsand::CmpColor>();

        for (auto const [ent, rPos, rColor] : viewDrawables.each())
        {
            DrawPixel(rPos.m_pos.m_x, rPos.m_pos.m_y,
                      CLITERAL(Color){ rColor.m_r, rColor.m_g, rColor.m_b, 255 });
        }

        //DrawRectangle(0, 0, 2, 2, CLITERAL(Color){ 255, 0, 0, 255 });

        EndTextureMode();

        DrawTexturePro(target.texture,
                       (Rectangle){ 0, screenHeight, screenWidth, -screenHeight  },
                       (Rectangle){ 0, 0, screenWidth * screenScale, screenHeight * screenScale },
                       (Vector2){ 0, 0 }, 0, CLITERAL(Color){ 255, 255, 255, 255 });
        
        EndDrawing();
    }
    
    return 0;
}
