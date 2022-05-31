#include "sentry_alloc.h"
#include "sentry_sync.h"
#include <stdlib.h>
#include <string.h>

#ifndef SENTRY_EXTERNAL_ALLOCATOR

/* on unix platforms we add support for a simplistic page allocator that can
   be enabled to make code async safe */
#    ifdef SENTRY_PLATFORM_UNIX
#        include "sentry_unix_pageallocator.h"
#        define WITH_PAGE_ALLOCATOR
#    endif

void *
sentry_malloc(size_t size)
{
#    ifdef WITH_PAGE_ALLOCATOR
    if (sentry__page_allocator_enabled()) {
        return sentry__page_allocator_alloc(size);
    }
#    endif
    return malloc(size);
}

void
sentry_free(void *ptr)
{
#    ifdef WITH_PAGE_ALLOCATOR
    /* page allocator can't free */
    if (sentry__page_allocator_enabled()) {
        return;
    }
#    endif
    free(ptr);
}

void *
sentry_realloc(void *old_ptr, size_t new_size)
{
#    ifdef WITH_PAGE_ALLOCATOR
    if (sentry__page_allocator_enabled()) {
        /* page allocator can't free */
        return sentry__page_allocator_alloc(new_size);
    }
#    endif
    return realloc(old_ptr, new_size);
}

#endif
