
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

    int errno;
    
#define ENOMEM 1
#define EINVAL 2

#ifdef __cplusplus
}
#endif

#endif /* ERRNO_H */

