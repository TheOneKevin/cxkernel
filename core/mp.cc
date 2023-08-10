#include "core/mp.h"

static struct core::percpu percpu_arr[16];
struct core::percpu* g::percpu = (core::percpu*) &percpu_arr;
