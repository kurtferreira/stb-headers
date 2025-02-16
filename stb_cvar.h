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
#include <float.h> // for double min and max

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

// CVAR options
#define CVAR_OPT_SAVE       (1<<1)   // Save the CVAR to a file (this is up to the implementor)
#define CVAR_OPT_RESTART    (1<<2)   // Changing this Cvar will result in a restart of some systems (again, up to the implementor)
#define CVAR_OPT_VISIBLE    (1<<3)   // The cvar is a menu-visible setting
// Define your own options over this point starting with (1<<4)

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
  double min;
  double max;
  bool must_restart;
  unsigned int flags;

} cvar_t;

//
// Declarations
//

cvar_t* stb_cvar_create(const char* name, int type, unsigned int flags);
void stb_cvar_destroy(cvar_t* cvar);
void stb_cvar_set_number(cvar_t* cvar, double value);
void stb_cvar_set_str(cvar_t* cvar, char* value);
void stb_cvar_set_bool(cvar_t* cvar, bool value);

void stb_cvar_set_min(cvar_t* cvar, double value);
void stb_cvar_set_max(cvar_t* cvar, double value);
void stb_cvar_set_opt(cvar_t* cvar, unsigned int option);
void stb_cvar_clear_opt(cvar_t* cvar, unsigned int option);
bool stb_cvar_has_opt(cvar_t* cvar, unsigned int option);

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

cvar_t* stb_cvar_create(const char* name, int type, unsigned int flags) {
  CVAR_ASSERT(strlen(name) < CVAR_MAX_NAME);

  cvar_t* cvar = CVAR_ALLOC(sizeof(cvar_t));
  CVAR_ASSERT(cvar != NULL);
  cvar->type = type;
   cvar->must_restart = false;
   cvar->flags = flags;
   cvar->min = DBL_MIN;
   cvar->max = DBL_MAX;
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
  CVAR_ASSERT(value > cvar->min && value < cvar->max); // if the user specifies bounds, otherwise we'll simply wrap around

  cvar->value.number = value;
}

void stb_cvar_set_bool(cvar_t* cvar, bool value) {
  CVAR_ASSERT(cvar);
  cvar->value.boolean = value;
}

void stb_cvar_set_min(cvar_t* cvar, double value) {
   CVAR_ASSERT(cvar);
   cvar->min = value;
}

void stb_cvar_set_max(cvar_t* cvar, double value) {
   CVAR_ASSERT(cvar);
   cvar->max = value;
}

void stb_cvar_set_opt(cvar_t* cvar, unsigned int option) {
   CVAR_ASSERT(cvar);
   cvar->flags |= option;
}

void stb_cvar_clear_opt(cvar_t* cvar, unsigned int option) {
   CVAR_ASSERT(cvar);
   cvar->flags &= ~option;
}

bool stb_cvar_has_opt(cvar_t* cvar, unsigned int option) {
   CVAR_ASSERT(cvar);
   return cvar->flags & option;
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
