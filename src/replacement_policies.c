//
// This file contains all of the implementations of the replacement_policy
// constructors from the replacement_policies.h file.
//
// It also contains stubs of all of the functions that are added to each
// replacement_policy struct at construction time.
//
// ============================================================================
// NOTE: It is recommended that you read the comments in the
// replacement_policies.h file for further context on what each function is
// for.
// ============================================================================
//

#include "replacement_policies.h"

// LRU Replacement Policy
// ============================================================================
// TODO feel free to create additional structs/enums as necessary


void lru_cache_access(struct replacement_policy *replacement_policy,
                      struct cache_system *cache_system, uint32_t set_idx, uint32_t tag)
{
    // TODO update the LRU replacement policy state given a new memory access
    //Loop through and find highest number to save it. Then loop through, find the tag and change its position in the LRU policy to the highest number + 1.
    //Either scenario, loop through until you find tag and change it to next highest number.
    
    int set_start = set_idx * cache_system->associativity;
   
    int highestNum = 0;
    //loop through all entries and find the highest number.

    for (int i = 0; set_start + i < set_start + cache_system->associativity; i++) {
        if(*((int *)replacement_policy->data + i  + set_start) > highestNum){
            highestNum = *((int *)replacement_policy->data + i + set_start);
        }
    }

    // loop through all entries in a set and find the most recent tag. Change its value in the LRU policy to the highest number + 1.
    // problem: not looping through whole table. Stops with largest value and just changes that slot to the highest number + 1.
    //problem: have duplicate numbers (9s) in set 3
     for(int i = 0; set_start + i < set_start + cache_system->associativity; i++) {
        if(cache_system->cache_lines[set_start + i].tag == tag){
            *((int *)replacement_policy->data + set_start + i) = highestNum + 1;
        }
    }

}

uint32_t lru_eviction_index(struct replacement_policy *replacement_policy,
                            struct cache_system *cache_system, uint32_t set_idx)
{
    // TODO return the index within the set that should be evicted.
    //give the index of the 0th element in the 

    //loop through all of the set and find the element with the smallest number. Return index of that element.
    
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
    // TODO cleanup any additional memory that you allocated in the
    //lru_replacement_policy_new function.
    free(replacement_policy->data);
}

struct replacement_policy *lru_replacement_policy_new(uint32_t sets, uint32_t associativity)
{
    struct replacement_policy *lru_rp = calloc(1, sizeof(struct replacement_policy));
    lru_rp->cache_access = &lru_cache_access;
    lru_rp->eviction_index = &lru_eviction_index;
    lru_rp->cleanup = &lru_replacement_policy_cleanup;

    // TODO allocate any additional memory to store metadata here and assign to
    //lru_rp->data;

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
    // TODO update the RAND replacement policy state given a new memory access
    //Null? Nothing to update for the policy replacement cache access?
}

uint32_t rand_eviction_index(struct replacement_policy *replacement_policy,
                             struct cache_system *cache_system, uint32_t set_idx)
{
  
   
    return rand() % cache_system->associativity;
}

void rand_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{
    // TODO cleanup any additional memory that you allocated in the
    // rand_replacement_policy_new function.
}

struct replacement_policy *rand_replacement_policy_new(uint32_t sets, uint32_t associativity)
{
    // Seed randomness
    srand(time(NULL));

    struct replacement_policy *rand_rp = malloc(sizeof(struct replacement_policy));
    rand_rp->cache_access = &rand_cache_access;
    rand_rp->eviction_index = &rand_eviction_index;
    rand_rp->cleanup = &rand_replacement_policy_cleanup;

    // TODO allocate any additional memory to store metadata here and assign to
    // rand_rp->data.

    return rand_rp;
}

// LRU_PREFER_CLEAN Replacement Policy
// ============================================================================
void lru_prefer_clean_cache_access(struct replacement_policy *replacement_policy,
                                   struct cache_system *cache_system, uint32_t set_idx,
                                   uint32_t tag)
{
    // TODO update the LRU_PREFER_CLEAN replacement policy state given a new
    // memory access
    // NOTE: you may be able to share code with the LRU policy

    //after access, find the highest number and assign highestNum + 1 to the index that was just accessed. Same as LRU

    int set_start = set_idx * cache_system->associativity;
   
    int highestNum = 0;
    //loop through all entries and find the highest number.

    printf("start cache access\n");

    for (int i = 0; set_start + i < set_start + cache_system->associativity; i++) {
        if(*((int *)replacement_policy->data + i  + set_start) > highestNum){
            highestNum = *((int *)replacement_policy->data + i + set_start);
        }
    }

    // loop through all entries in a set and find the most recent tag. Change its value in the LRU policy to the highest number + 1.
    // problem: not looping through whole table. Stops with largest value and just changes that slot to the highest number + 1.
    //problem: have duplicate numbers (9s) in set 3
     for(int i = 0; set_start + i < set_start + cache_system->associativity; i++) {
        if(cache_system->cache_lines[set_start + i].tag == tag){
            *((int *)replacement_policy->data + set_start + i) = highestNum + 1;
        }
    }
}

uint32_t lru_prefer_clean_eviction_index(struct replacement_policy *replacement_policy,
                                         struct cache_system *cache_system, uint32_t set_idx)
{
    // TODO return the index within the set that should be evicted.

    //loop through metadata array and see if all lines are dirty. If so, return LRU. If there is 1 clean line, return that index.
    // If there are multiple clean lines, find smallest value of clean lines.

    //loop through and find number of clean lines and also find lowest index in same loop

    int set_start = set_idx * cache_system->associativity;
    int cleanCount = 0;
    int indexOfLowest = 0;
    int lowest = INT32_MAX;

    printf("start evict index\n");

    for(int i = 0 ; set_start + i < set_start + cache_system->associativity; i++){
        if(cache_system->cache_lines[set_start + i].status != MODIFIED){
            cleanCount++;
        }
        if(*((int *) replacement_policy->data + i + set_start) < lowest){
            lowest = *((int *) replacement_policy->data + i + set_start);
            indexOfLowest = i;
        }
    }
        printf("end first loop \n");

     
    // No clean lines, return lowest index
    if(cleanCount == 0){
        return indexOfLowest;
    }

    // only 1 clean line, find that line and return it
    if(cleanCount == 1){
        set_start = set_idx * cache_system->associativity;
        for(int i = 0 ; i + set_start < set_start + cache_system->associativity ; i++){
            if(cache_system->cache_lines[i + set_start].status != MODIFIED){
                return i;
            }
        }
    }
    printf("end second loop \n");

    // multiple clean lines, loop through and find index of LRU and clean line (index of smallest int in meta data array that is also clean)
    int returnIndex = 0;
    lowest = INT32_MAX;
    set_start = set_idx * cache_system->associativity;
    for(int i = 0 ; i + set_start < set_start + cache_system->associativity ; i++){
        int valueInMeta = *((int *) replacement_policy->data + i + set_start);
        if(cache_system->cache_lines[i + set_start].status != MODIFIED && valueInMeta < lowest){
            lowest = valueInMeta;
            returnIndex = i;
        }
    }
    printf("end third loop \n");
    return returnIndex;
}

void lru_prefer_clean_replacement_policy_cleanup(struct replacement_policy *replacement_policy)
{
    // TODO cleanup any additional memory that you allocated in the
    // lru_prefer_clean_replacement_policy_new function.
    free(replacement_policy->data);
}

struct replacement_policy *lru_prefer_clean_replacement_policy_new(uint32_t sets,
                                                                   uint32_t associativity)
{
    struct replacement_policy *lru_prefer_clean_rp = malloc(sizeof(struct replacement_policy));
    lru_prefer_clean_rp->cache_access = &lru_prefer_clean_cache_access;
    lru_prefer_clean_rp->eviction_index = &lru_prefer_clean_eviction_index;
    lru_prefer_clean_rp->cleanup = &lru_prefer_clean_replacement_policy_cleanup;

    // TODO allocate any additional memory to store metadata here and assign to
    // lru_prefer_clean_rp->data.

    int cache_lines_total = sets * associativity;
    int* masterTable = calloc(cache_lines_total, sizeof(int));
    for(int i = 0 ; i < cache_lines_total; i++){
        masterTable[i] = 0;
    }
    lru_prefer_clean_rp->data = masterTable;

    return lru_prefer_clean_rp;
}
