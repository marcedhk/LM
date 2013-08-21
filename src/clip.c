#include <stdio.h>
#include <math.h>

#include "PDL.h"

#include <unistd.h>

#include "acl_license.h"

#define LOGGING
#ifdef LOGGING
#include <syslog.h>
#define SYSLOG(...) syslog(__VA_ARGS__)
#else
#define SYSLOG(...) do { } while(0)
#endif

PDL_bool PDL_acl_license(PDL_JSParameters *params)
{
    /* This is where we need to do the work .*/
    int success = 0;
    //success = acl_license(argv[1]);
    SYSLOG(LOG_INFO, "***** plugin acl_license called");
    success = acl_license(NULL);
    return PDL_TRUE;
}

PDL_bool hello_world(PDL_JSParameters *params)
{
    const char *buf;
    if (PDL_GetNumJSParams(params) < 1)
    {
        buf = PDL_GetJSParamString(params, 1);
        if(buf == NULL)
        {
            PDL_JSException(params, "You must send a hellostr param");
            return PDL_TRUE;
        }
    }
    else
    {
        PDL_JSException(params, "No params sent");
        return PDL_TRUE;
    }
    
    char reply[32] = "Hello World!";
    PDL_JSReply(params, reply);

    return PDL_TRUE;
}

static void loop()
{
    SDL_Event event;
    {
        // fire off the js
        const char *params[2];
        params[0] = "foo";
        params[1] = "bar";
        PDL_Err mjErr = PDL_CallJS("testFunc", params, 2);
        if ( mjErr != PDL_NOERROR )
        {
            printf("PDL_CallJS failed. %s\n", PDL_GetError());
        }
    }
    if (SDL_WaitEvent(&event)) {
        SYSLOG(LOG_INFO, "***** plugin event");
        if (event.type == SDL_QUIT) {
            exit(0);
        }
    }
}

int main(int argc, char** argv)
{
    SYSLOG(LOG_INFO, "***** plugin started");
    PDL_Init(0);
    SYSLOG(LOG_INFO, "***** plugin initialized PDL");
    atexit(PDL_Quit);

    SDL_Init(SDL_INIT_VIDEO);
    SYSLOG(LOG_INFO, "***** plugin initialized SDL");
    atexit(SDL_Quit);
    SYSLOG(LOG_INFO, "***** plugin initialized");
    
    // register the js callback
    PDL_RegisterJSHandler("hello_world", hello_world);
    PDL_RegisterJSHandler("acl_license", PDL_acl_license);
    PDL_JSRegistrationComplete();
    PDL_CallJS("ready", NULL, 0);

    while(1)
        loop();

    // Cleanup
    PDL_Quit();
    SYSLOG(LOG_INFO, "***** plugin exiting");
}
