#include "include/mempool.h"

int main() {
  MEM_POOL* pool = create_pool(10, 10);
  MEM_POOL_BLOCK* p = alloc_pool(pool);
  delete_pool(pool);
  return 0;
}
