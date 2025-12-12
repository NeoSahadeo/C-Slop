#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef MEMPOOL_H
#define MEMPOOL_H

typedef struct MEM_POOL_BLOCK {
  struct MEM_POOL_BLOCK* next;
} MEM_POOL_BLOCK;

typedef struct MEM_POOL {
  size_t block_size;
  size_t block_count;
  MEM_POOL_BLOCK* base;
  MEM_POOL_BLOCK* free_blocks;
} MEM_POOL;

/**
 * Create a pool of memory,
 * block_count * block_size = bytes of memory
 * */
MEM_POOL* create_pool(size_t block_count, size_t block_size) {
  MEM_POOL* pool = (MEM_POOL*)malloc(sizeof(MEM_POOL));
  if (!pool) {
    fprintf(stderr, "Error Allocating Memory Pool\n");
    exit(EXIT_FAILURE);
  }

  pool->block_size = sizeof(MEM_POOL_BLOCK) + block_size;
  pool->block_count = block_count;

  MEM_POOL_BLOCK* blocks =
      (MEM_POOL_BLOCK*)calloc(1, pool->block_size * block_count);
  if (!blocks) {
    fprintf(stderr, "Error Allocating Blocks for Pool\n");
    exit(EXIT_FAILURE);
  }

  pool->base = blocks;
  pool->free_blocks = blocks;
  MEM_POOL_BLOCK* current = blocks;
  for (size_t x = 0; x < block_count; x++) {
    current->next = (MEM_POOL_BLOCK*)((char*)current + pool->block_size);
    current = current->next;
  }
  current->next = NULL;
  return pool;
}

/**
 * Allocate memory from the pool
 * */
void* alloc_pool(MEM_POOL* pool) {
  if (pool->free_blocks == NULL) {
    fprintf(stderr, "Error Pool Out of Memory!\n");
    return NULL;
  }
  MEM_POOL_BLOCK* block = pool->free_blocks;
  pool->free_blocks = block->next;
  return block;
}

/** Free memory from the pool */
void free_pool(MEM_POOL* pool, void* ptr) {
  MEM_POOL_BLOCK* block = (MEM_POOL_BLOCK*)ptr;
  block->next = pool->free_blocks;
  pool->free_blocks = block;
}

/**
 * Destroys the entire memory pool
 *
 * Does not require freeing individual pools */
void delete_pool(MEM_POOL* pool) {
  free(pool->base);
  free(pool);
}

#endif
