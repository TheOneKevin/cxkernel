/*
 * File:   page_alloc.c
 * Author: Kevin Dai
 * Email:  kevindai02@outlook.com
 *
 * Created on 2017-08-30T14:37:09-04:00
 *
 * @ Last modified by:   Kevin Dai
 * @ Last modified time: 2017-11-26T12:28:25-05:00
*/

#include "mm/page_alloc.h"
#include "mm/mmtypes.h"
#include "mm/zones.h"

struct page* __get_free_page(uint32_t flags)
{

}

struct page* __alloc_pages(uint32_t flags, size_t len)
{

}

void pmm_init(void)
{

}

static pg_data_t data;
void pmm_early_init(void)
{
    // No SMP and NUMA yet...

}
