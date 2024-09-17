
#include "replacement_policies.h"

void lru_cache_access(struct replacement_policy *replacement_policy,
                      struct cache_system *cache_system, uint32_t set_idx, uint32_t tag)
{
    
    int set_start = set_idx * cache_system->associativity;
    struct cache_line *start = &cache_system->cache_lines[set_start];
   
    int highestNum = 0;

    for (int i = 0; set_start + i < set_start + cache_system->associativity; i++) {
        if(*((int *)replacement_policy->data + i  + set_start) > highestNum){
            highestNum = *((int *)replacement_policy->data + i + set_start);
        }
    }
     for(int i = 0; set_start + i < set_start + cache_system->associativity; i++) {
        if(cache_system->cache_lines[set_start + i].tag == tag){
            *((int *)replacement_policy->data + set_start + i) = highestNum + 1;
        }
    }

}

uint32_t lru_eviction_index(struct replacement_policy *replacement_policy,
                            struct cache_system *cache_system, uint32_t set_idx)
{    
    int index = 0;
    int set_start = set_idx * cache_system->associativity;
    int lowest = INT32_MAX;

    for (int i = 0; set_start + i < set_start + cache_system->associativity; i++) {
        if(*((int *) replacement_policy->data + set_start + i) < lowest){
            lowest = *((int *)replacement_policy->data + i + set_start);
            index = i;
        }
    }
    return index;
}

void lru_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{
    free(replacement_policy->data);
}

struct replacement_policy *lru_replacement_policy_new(uint32_t sets, uint32_t associativity)
{
    struct replacement_policy *lru_rp = calloc(1, sizeof(struct replacement_policy));
    lru_rp->cache_access = &lru_cache_access;
    lru_rp->eviction_index = &lru_eviction_index;
    lru_rp->cleanup = &lru_replacement_policy_cleanup;

    int cache_lines_total = sets * associativity;
    int* masterTable = calloc(cache_lines_total, sizeof(int));
    for(int i = 0 ; i < cache_lines_total; i++){
        masterTable[i] = 0;
    }
    lru_rp->data = masterTable;
    
    return lru_rp;
}

// RAND Replacement Policy
// ============================================================================
void rand_cache_access(struct replacement_policy *replacement_policy,
                       struct cache_system *cache_system, uint32_t set_idx, uint32_t tag)
{

}

uint32_t rand_eviction_index(struct replacement_policy *replacement_policy,
                             struct cache_system *cache_system, uint32_t set_idx)
{
    return rand() % cache_system->associativity;
}

void rand_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{
}

struct replacement_policy *rand_replacement_policy_new(uint32_t sets, uint32_t associativity)
{
    srand(time(NULL));

    struct replacement_policy *rand_rp = malloc(sizeof(struct replacement_policy));
    rand_rp->cache_access = &rand_cache_access;
    rand_rp->eviction_index = &rand_eviction_index;
    rand_rp->cleanup = &rand_replacement_policy_cleanup;

    return rand_rp;
}

// LRU_PREFER_CLEAN Replacement Policy
// ============================================================================
void lru_prefer_clean_cache_access(struct replacement_policy *replacement_policy,
                                   struct cache_system *cache_system, uint32_t set_idx,
                                   uint32_t tag)
{
}

uint32_t lru_prefer_clean_eviction_index(struct replacement_policy *replacement_policy,
                                         struct cache_system *cache_system, uint32_t set_idx)
{
    return 0;
}

void lru_prefer_clean_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{

}

struct replacement_policy *lru_prefer_clean_replacement_policy_new(uint32_t sets,
                                                                   uint32_t associativity)
{
    struct replacement_policy *lru_prefer_clean_rp = malloc(sizeof(struct replacement_policy));
    lru_prefer_clean_rp->cache_access = &lru_prefer_clean_cache_access;
    lru_prefer_clean_rp->eviction_index = &lru_prefer_clean_eviction_index;
    lru_prefer_clean_rp->cleanup = &lru_prefer_clean_replacement_policy_cleanup;

    return lru_prefer_clean_rp;
}
