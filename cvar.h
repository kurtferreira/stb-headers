#ifndef STB_CVAR_H
#define STB_CVAR_H

#ifdef CVAR_IMPLEMENTATION
   // This block should only be defined in the .c/.cpp file where the implementation is included.
   #define STB_CVAR_IMPLEMENTATION
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef CVAR_MAX_NAME
  #define CVAR_MAX_NAME 128
#endif 

#ifndef CVAR_ASSERT
  #include <assert.h>
  #define CVAR_ASSERT(x) assert(x)
#endif 

#ifdef __cplusplus
extern "C" {
#endif

enum {
  CVAR_NUMBER,
  CVAR_STRING,
  CVAR_BOOL
};

typedef union {
    char *str;
    double number;
    bool boolean;
} cvar_value_t;

typedef struct {
  char name[CVAR_MAX_NAME];
  char type;
  cvar_value_t value;
} cvar_t;

// #define cvar_get(cvar, type)
// #define cvar_set(cvar, type)

//
// Declarations
//

cvar_t* stb_cvar_create(const char* name, int type);
void stb_cvar_destroy(cvar_t* cvar);
void stb_cvar_set(cvar_t* cvar, cvar_value_t value);
const char* stb_cvar_get_str(cvar_t* cvar);
double stb_cvar_get_number(cvar_t* cvar);
bool stb_cvar_get_bool(cvar_t* cvar);


#ifdef __cplusplus
}
#endif

#endif // STB_CVAR_H

#ifdef STB_CVAR_IMPLEMENTATION


//
// Implementation
//

cvar_t* stb_cvar_create(const char* name, int type) {
  CVAR_ASSERT(strlen(name) < CVAR_MAX_NAME);
  
  
  
  return (cvar_t*) NULL;
}

void stb_cvar_destroy(cvar_t* cvar) {
  CVAR_ASSERT(cvar);
}

void stb_cvar_set(cvar_t* cvar, cvar_value_t value) {
  CVAR_ASSERT(cvar);
  
}

const char* stb_cvar_get_str(cvar_t* cvar) {
  CVAR_ASSERT(cvar);
   
  return cvar->value.str;
}

double stb_cvar_get_number(cvar_t* cvar) {
  CVAR_ASSERT(cvar);
   
  return cvar->value.number;
}

bool stb_cvar_get_bool(cvar_t* cvar) {
  CVAR_ASSERT(cvar);
   
  return cvar->value.boolean;
}

#endif // STB_CVAR_IMPLEMENTATION
