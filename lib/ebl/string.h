#pragma once

#include <stddef.h>

namespace ebl {

// As defined in the:
// The Open Group Base Specifications Issue 6
// IEEE Std 1003.1, 2004 Edition
// Copyright © 2001-2004 The IEEE and The Open Group, All Rights reserved.

int      strncmp(const char *, const char *, size_t);
int      memcmp(const void *, const void *, size_t);
void    *memcpy(void *, const void *, size_t);
void    *memset(void *, int, size_t);
void    *memmove(void *, void *, size_t);
char    *strcat(char *, const char *);
char    *strchr(const char *, int);
char    *strcpy(char *, const char *);
char    *strdup(const char *);
int      strcmp(const char *, const char *);
size_t   strlen(const char *);
char    *strstr(const char *, const char *);

} // namespace ebl
