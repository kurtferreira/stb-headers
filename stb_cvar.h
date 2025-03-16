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

#define CVAR_USE_C11 // using generics
#ifdef CVAR_USE_C11
    #define stb_cvar_get(x, v) _Generic((v), \
        char**:  ((x)->type == CVAR_STRING  ? (*(char**)&(v) = (x)->str)    : (fprintf(stderr, "Type mismatch!\n"), 0)), \
        bool*:   ((x)->type == CVAR_BOOL    ? (*(bool*)&(v) = (x)->boolean) : (fprintf(stderr, "Type mismatch!\n"), 0)), \
        double*: ((x)->type == CVAR_NUMBER  ? (*(double*)&(v) = (x)->number) : (fprintf(stderr, "Type mismatch!\n"), 0)) \
    )

    #define stb_cvar_set(x, v) \
        _Generic((v), \
            unsigned int: stb_cvar_set_bool, \
            bool: stb_cvar_set_bool, \
            int: stb_cvar_set_bool, \
            double: stb_cvar_set_number, \
            char*: stb_cvar_set_str \
        )(x, v)
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
  double min;
  double max;
  bool must_restart;
  unsigned int flags;

  union {
    char *str;
    double number;
    bool boolean;
  };
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

#ifdef CVAR_SHORTNAME
    #define cvar_create(name, type, flags) stb_cvar_create(name, type, flags)
    #define cvar_destroy(cvar) stb_cvar_destroy(cvar)
    #define cvar_set_number(cvar, value) stb_cvar_set_number(cvar, value)
    #define cvar_set_str(cvar, value) stb_cvar_set_str(cvar, value)
    #define cvar_set_bool(cvar, value) stb_cvar_set_bool(cvar, value)
    #define cvar_set_min(cvar, value) stb_cvar_set_min(cvar, value)
    #define cvar_set_max(cvar, value) stb_cvar_set_max(cvar, value)
    #define cvar_set_opt(cvar, options) stb_cvar_set_opt(cvar, option)
    #define cvar_clear_opt(cvar, option) stb_cvar_clear_opt(cvar, option)
    #define cvar_has_opt(cvar, option) stb_cvar_has_opt(cvar, option)
    #define cvar_get_str(cvar) stb_cvar_get_str(cvar)
    #define cvar_get_number(cvar) stb_cvar_get_number(cvar)
    #define cvar_get_bool(cvar) stb_cvar_get_bool(cvar)
    #ifdef CVAR_USE_C11
        #define cvar_get(cvar) stb_cvar_get(cvar)
        #define cvar_set(cvar, value) stb_cvar_set(cvar, value)
    #endif
#endif

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
  cvar->str = value;
}

void stb_cvar_set_number(cvar_t* cvar, double value) {
  CVAR_ASSERT(cvar);
  CVAR_ASSERT(value > cvar->min && value < cvar->max); // if the user specifies bounds, otherwise we'll simply wrap around

  cvar->number = value;
}

void stb_cvar_set_bool(cvar_t* cvar, bool value) {
  CVAR_ASSERT(cvar);
  cvar->boolean = value;
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
  return (const char*) cvar->str;
}

double stb_cvar_get_number(cvar_t* cvar) {
  CVAR_ASSERT(cvar);
  return cvar->number;
}

bool stb_cvar_get_bool(cvar_t* cvar) {
  CVAR_ASSERT(cvar);
  return cvar->boolean;
}

#endif // STB_CVAR_IMPLEMENTATION
