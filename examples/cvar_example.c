#include <stdio.h>
#include <stdbool.h>

#define CVAR_IMPLEMENTATION
#define CVAR_USE_C11
#define CVAR_SHORTNAME
#include "../stb_cvar.h"

cvar_t* r_renderShadow;

void cleanup()
{
    if(cvar_has_opt(r_renderShadow, CVAR_OPT_SAVE)) {
        // Save this cvar to a settings file
        printf("Saving CVAR to file!\n");
    }

    cvar_destroy(r_renderShadow);
}

void draw()
{
    if(cvar_get_bool(r_renderShadow))
    {
        // render shadows!
        printf("Render shadows y'all!\n");
    }
}

int main(int argc, char* argv[])
{
    r_renderShadow = cvar_create("r_renderShadow", CVAR_BOOL, CVAR_OPT_SAVE|CVAR_OPT_RESTART);

    cvar_set(r_renderShadow, true);
    cvar_set_bool(r_renderShadow, true);

    draw();

    cleanup();

    return 0;
}
