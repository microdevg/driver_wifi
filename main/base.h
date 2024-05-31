#ifndef _BASE_H
#define _BASE_H

#include "esp_err.h"
#include <stdbool.h>



#define NOT_CALLBACK        (NULL)

typedef void (*esp_callback_t)(void);


#define CHECK_RUN_F(f)            if(f)f()     // si f es distinta de NULL, llamar

// printf solo compila cuando se define EN_DEBUG_MESSAGE
#define EN_DEBUG_MESSAGE             

#ifdef EN_DEBUG_MESSAGE
    #define DEBUG_FORMAT                        "[DEBUG] %s"
    #define DEBUG_PRINT(message)                printf(DEBUG_FORMAT,message)
#else
    #define DEBUG_PRINT(message) 
#endif




#endif