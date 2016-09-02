/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   kheap.h
 * Author: Kevin
 *
 * Created on August 29, 2016, 11:36 AM
 */
#include "common.h"

#ifndef KHEAP_H
#define KHEAP_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Allocates the next free chunk of memory
 * @param size The size of the chunk (size_t)
 * @param align Whether you want it page aligned or not
 * @param physicalAddr The return pointer to physical address
 * @return The return address
 */
uint32_t h_kmalloc(size_t size, bool align, uint32_t *physicalAddr);

#ifdef __cplusplus
}
#endif

#endif /* KHEAP_H */

