#include "sdl_base.hpp"

#include <iostream>

#include <SDL_image.h>
#include <SDL_mixer.h>

#include "init.hpp"
#include "config.hpp"
#include "game_time.hpp"

namespace sdl_base
{

namespace
{

bool is_inited = false;

}

void init()
{
    TRACE_FUNC_BEGIN;

    cleanup();

    is_inited = true;

    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
        TRACE << "Failed to init SDL" << std::endl;
        ASSERT(false);
    }

    if (IMG_Init(IMG_INIT_PNG) == -1)
    {
        TRACE << "Failed to init SDL_image" << std::endl;
        ASSERT(false);
    }

    const int     audio_freq      = 44100;
    const Uint16  audio_format    = MIX_DEFAULT_FORMAT;
    const int     audio_channels  = 2;
    const int     audio_buffers   = 1024;

    const int result =
        Mix_OpenAudio(audio_freq,
                      audio_format,
                      audio_channels,
                      audio_buffers);

    if (result == -1)
    {
        TRACE << "Failed to init SDL_mixer" << std::endl;
        ASSERT(false);
    }

    Mix_AllocateChannels(audio_allocated_channels);

    TRACE_FUNC_END;
}

void cleanup()
{
    if (!is_inited)
    {
        return;
    }

    is_inited = false;

    IMG_Quit();

    Mix_AllocateChannels(0);

    Mix_CloseAudio();

    SDL_Quit();
}

void sleep(const Uint32 duration)
{
    if (is_inited &&
        !config::is_bot_playing())
    {
        if (duration == 1)
        {
            SDL_Delay(duration);
        }
        else // Duration longer than 1 ms
        {
            const Uint32 wait_until = SDL_GetTicks() + duration;

            while (SDL_GetTicks() < wait_until)
            {
                SDL_PumpEvents();
            }
        }
    }
}

} // sdl_base
