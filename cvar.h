#ifndef STB_CVAR_H
#define STB_CVAR_H

#ifdef CVAR_IMPLEMENTATION
   // This block should only be defined in the .c/.cpp file where the implementation is included.
   #define STB_CVAR_IMPLEMENTATION
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef CVAR_MAX_NAME
  #define CVAR_MAX_NAME 128
#endif 

#ifndef CVAR_ASSERT
  #include <assert.h>
  #define CVAR_ASSERT(x) assert(x)
#endif 

#ifndef CVAR_ALLOC
  #include <assert.h>
  #define CVAR_ALLOC(size) malloc(size)
  #define CVAR_FREE(ptr) free(ptr)
#endif 

#ifdef __cplusplus
extern "C" {
#endif

enum {
  CVAR_NUMBER = 0,
  CVAR_STRING = 1,
  CVAR_BOOL = 2
};

typedef struct {
  char name[CVAR_MAX_NAME];
  char type;
  union { 
    char *str;
    double number;
    bool boolean;
  } value;
} cvar_t;

#ifndef CVAR_NOSHORTHAND
  #define cvar_get(cvar) \
    cvar->type == CVAR_NUMBER ? \
      cvar->value.number : ( \
        cvar->type == CVAR_STRING ? cvar->value.str : cvar->value.boolean \
      )
      
  #define cvar_set(cvar, val) \
    switch(cvar->type) { \
      case CVAR_NUMBER: stb_cvar_set_number(cvar, val);break; \
      case CVAR_STRING: stb_cvar_set_str(cvar, val);break; \
      case CVAR_BOOL: stb_cvar_set_bool(cvar, val);break; \
    };

#endif

//
// Declarations
//

cvar_t* stb_cvar_create(const char* name, int type);
void stb_cvar_destroy(cvar_t* cvar);
void stb_cvar_set_number(cvar_t* cvar, double value);
void stb_cvar_set_str(cvar_t* cvar, char* value);
void stb_cvar_set_bool(cvar_t* cvar, bool value);
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
  
  cvar_t* cvar = CVAR_ALLOC(sizeof(cvar_t));
  CVAR_ASSERT(cvar != NULL);
  cvar->type = type;
  strcpy(cvar->name, name);
   
  return cvar;
}

void stb_cvar_destroy(cvar_t* cvar) {
  CVAR_ASSERT(cvar);
  CVAR_FREE(cvar);
}

 
void stb_cvar_set_str(cvar_t* cvar, char* value) {
  CVAR_ASSERT(cvar);
  cvar->value.str = value;
}

void stb_cvar_set_number(cvar_t* cvar, double value) {
  CVAR_ASSERT(cvar);
  cvar->value.number = value;
}

void stb_cvar_set_bool(cvar_t* cvar, bool value) {
  CVAR_ASSERT(cvar);
  cvar->value.boolean = value;
}

const char* stb_cvar_get_str(cvar_t* cvar) {
  CVAR_ASSERT(cvar);
  return (const char*) cvar->value.str;
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
