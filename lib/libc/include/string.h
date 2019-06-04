/**
 * @file   string.h
 * @author Kevin Dai \<kevindai02@outlook.com\>
 * 
 * @date Created on Sunday, October 7th 2018, 1:53:15 pm
 * 
 * @date Last modified by:   Kevin Dai
 * @date Last modified time: 2018-10-14T15:59:33-04:00
 */

#pragma once

#ifndef libm
    #include "common.h"
#else
    #define __BEGIN_CDECLS
    #define __END_CDECLS
    #include <stdbool.h>
    #include <stddef.h>
#endif

// As defined in the:
// The Open Group Base Specifications Issue 6
// IEEE Std 1003.1, 2004 Edition
// Copyright © 2001-2004 The IEEE and The Open Group, All Rights reserved.

__BEGIN_CDECLS

int      strncmp(const char *, const char *, size_t);
int      memcmp(const void *, const void *, size_t);
void    *memcpy(void *, const void *, size_t);
void    *memset(void *, int, size_t);
void    *memmove(void *, const void *, size_t);
char    *strcat(char *, const char *);
char    *strcpy(char *, const char *);
char    *strdup(const char *);
int      strcmp(const char *, const char *);
size_t   strlen(const char *);

/*
void    *memccpy(void *restrict, const void *restrict, int, size_t);
void    *memchr(const void *, int, size_t);
char    *strchr(const char *, int);
int      strcoll(const char *, const char *);
size_t   strcspn(const char *, const char *);
char    *strerror(int);
int     *strerror_r(int, char *, size_t);
char    *strncat(char *restrict, const char *restrict, size_t);
int      strncmp(const char *, const char *, size_t);
char    *strncpy(char *restrict, const char *restrict, size_t);
char    *strpbrk(const char *, const char *);
char    *strrchr(const char *, int);
size_t   strspn(const char *, const char *);
char    *strstr(const char *, const char *);
char    *strtok(char *restrict, const char *restrict);
char    *strtok_r(char *, const char *, char **);
size_t   strxfrm(char *restrict, const char *restrict, size_t);
*/

__END_CDECLS