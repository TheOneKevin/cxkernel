/*
 * File:   errno.h
 * Author: kevin
 *
 * Created on December 11, 2016, 8:33 AM
 */

#ifndef ERRNO_H
#define ERRNO_H

#ifdef __cplusplus
extern "C" {
#endif

typedef int status_t;

#define ENOMEM 1 // No memory left
#define EINVAL 2 // Invalid input
#define ENOENT 3 // File/dir not found
#define ENODEV 4 // Devices all full/device not found
#define EWRITE 5 // Writing to a read-only medium
#define ENOFRM 6 // No free frames left
#define ENIMPL 7 // Not implemented yet

#ifdef __cplusplus
}
#endif

#endif /* ERRNO_H */
