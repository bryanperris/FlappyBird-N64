#include "system.h"
#include "audio.h"
#include "graphics.h"

#include "ui.h"
#include "background.h"
#include "bird.h"
#include "pipes.h"
#include "collision.h"

#include "global.h"

audio_t *g_audio = NULL;
graphics_t *g_graphics = NULL;

int main(void)
{
    /* Enable interrupts (on the CPU) */
    init_interrupts();

    /* Initialize peripherals */
    dfs_init( DFS_ROM_LOCATION );
    controller_init();

    /* Initialize display */
    g_graphics = graphics_setup(
        RESOLUTION_320x240, DEPTH_16_BPP,
        BUFFERING_DOUBLE, GAMMA_NONE, ANTIALIAS_RESAMPLE
    );

    /* Initialize audio */
    g_audio = audio_setup( FREQUENCY_44KHZ, 1 );
    audio_write_silence();

    /* Initialize game state */
    background_t bg = background_setup( BG_DAY_TIME );
    bird_t bird = bird_setup( BIRD_COLOR_YELLOW );
    pipes_t pipes = pipes_setup();
    ui_t ui = ui_setup();

    /* Run the main loop */
    while(1)
    {
        /* Update controller state */
        controller_scan();
        const controllers_state_t keys = get_keys_down();

        /* Switch between day and night */
        if( keys.c[0].L )
        {
            background_set_time_mode( &bg, !bg.time_mode );
        }

        /* Update game state */
        bird_tick( &bird, keys.c[0] );
        switch (bird.state)
        {
            case BIRD_STATE_TITLE:
            case BIRD_STATE_READY:
                background_tick( &bg );
                pipes_reset( &pipes );
                break;
            case BIRD_STATE_PLAY:
                background_tick( &bg );
                pipes_tick( &pipes );
                collision_tick( &bird, &pipes );
                break;
            default:
                break;
        }

        /* Buffer sound effects */
        audio_tick( g_audio );

        /* Grab a display buffer and start drawing */
        graphics_display_lock( g_graphics );
        {
            /* Draw the game state */
            background_draw( bg );
            pipes_draw( pipes );
            bird_draw( bird );
            ui_draw( ui, bird, bg );
        }
        /* Finish drawing and show the framebuffer */
        graphics_display_flip( g_graphics );
    }

    /* Clean up game state */
    ui_free( &ui );
    pipes_free( &pipes );
    bird_free( &bird );
    background_free( &bg );

    /* Clean up the initialized subsystems */
    audio_free( g_audio );
    g_audio = NULL;
    graphics_free( g_graphics );
    g_graphics = NULL;

    return 0;
}
