
#include <stdio.h>

#define CVAR_IMPLEMENTATION
#include "stb_cvar.h"

cvar_t* r_renderShadow;

void cleanup()
{
    if(stb_cvar_has_opt(r_renderShadow, CVAR_OPT_SAVE)) {
        // Save this cvar to a settings file
        printf("Saving CVAR to file!\n");
    }

    stb_cvar_destroy(r_renderShadow);
}

void draw()
{
    if(stb_cvar_get_bool(r_renderShadow))
    {
        // render shadows!
        printf("Render shadows y'all!\n");
    }
}

int main(int argc, char* argv[])
{
    r_renderShadow = stb_cvar_create("r_renderShadow", CVAR_BOOL, CVAR_OPT_SAVE|CVAR_OPT_RESTART);

    stb_cvar_set_bool(r_renderShadow, true);

    draw();

    cleanup();

    return 0;
}
