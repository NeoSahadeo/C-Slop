#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef MEMARENA_H
#define MEMARENA_H

#define _ALIGN_BYTES(offset, size) \
  (offset + size - 1 + sizeof(size_t)) & ~(sizeof(size_t) - 1)

typedef struct MEM_ARENA {
  void* base;
  size_t size;
  size_t offset;
} MEM_ARENA;

/**
 * Create a memory arena
 * */
MEM_ARENA* create_arena(size_t size) {
  MEM_ARENA* arena = (MEM_ARENA*)malloc(sizeof(MEM_ARENA));
  if (!arena) {
    fprintf(stderr, "Failed to allocate arena pointer\n");
    exit(EXIT_FAILURE);
  }

  arena->size = size;
  arena->base = malloc(size);
  if (!arena->base) {
    fprintf(stderr, "Failed to allocate arena block\n");
    exit(EXIT_FAILURE);
  }

  arena->offset = 0;

  return arena;
}

/**
 * Delete an arena
 * */
void delete_arena(MEM_ARENA* arena) {
  free(arena->base);
  free(arena);
}

/**
 * Allocates memory from an arena.
 *
 * If the arena is too small for the aligned memory
 * it will return null.
 * */
void* alloc_arena(MEM_ARENA* arena, size_t size) {
  size_t requested = _ALIGN_BYTES(arena->offset, size);
  if (requested > arena->size) {
    fprintf(stderr, "Failed to request arena memory: %ld\n", size);
    return NULL;
  }

  void* p = (char*)arena->base + arena->offset + requested;
  arena->offset += requested;
  return p;
}

#endif
