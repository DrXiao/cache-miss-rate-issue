#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "cache.h"

// The message of a cache block.
typedef struct cache_block {
    bool valid;
    uint32_t tag;
    uint32_t access_time;
} cache_block;


// Variables for CPU access.
uint32_t mem_address = 0;
uint32_t cycle = 0;
uint32_t hit = 0;

// Relative variables of cache.
uint32_t cache_size = 0, block_size = 0, set_degree = 0;
uint32_t num_of_cache_blocks = 0;
uint32_t log2_num_of_sets = 0;
cache_block *cache = NULL;

/* 
 * log2_ui - log function for the specific situation.
 * @var : origin variable being a power of 2.
 * 
 * Output
 * --- return value : a uint32_t value ---
 * return the log value of var.
 * */
uint32_t log2_ui(uint32_t var) {
    uint32_t result = 0;
    while ((var & 1) == 0) {
        var >>= 1;
        result += 1;
    }
    return result;
}

/*
 * max - return the maximum between two unsigned integers.
 * @a : first integer.
 * @b : second integer.
 * 
 * Output
 * --- return value : a uint32_t value ---
 * return the maximum (a or b).
 * */
uint32_t max(uint32_t a, uint32_t b) {
    return (a > b) ? a : b;
}

/*
 * system_init - initialize global variables and allocating cache_blocks.
 * @argv : parameter of the main function, but giving the pointer to argv[2].
 *
 * Output
 * --- global variables ---
 * @cache_size, @block_size, @set_degree,
 * @num_of_cache_blocks, @log2_num_of_sets : initial variables related to
 * cache parameters.
 *
 * @cache : a pointer to a dynamically allocated array with cache_block type.
 * */
void system_init(char **argv) {
    sscanf(argv[0], "%u", &cache_size);
    sscanf(argv[1], "%u", &block_size);
    sscanf(argv[2], "%u", &set_degree);
    num_of_cache_blocks = cache_size * 1024 / (block_size * 4);
    cache = (cache_block *)calloc(num_of_cache_blocks, sizeof(cache_block));
    log2_num_of_sets = log2_ui(num_of_cache_blocks / set_degree);
}

/*
 * system_terminate - deallocate the memory of the array with cache_block type.
 *
 * Output
 * --- global variable ---
 * @cache : point to NULL
 * */
void system_terminate(void) {
    free(cache);
    cache = NULL;
}

/*
 * cache_address_access - access the data of the address on a certain cache
 * block.
 * @mem_address : given memory address.
 *
 * Output
 * --- return value : a boolean value ---
 * true (or 1) for 'cache hit'
 * false(or 0) for 'cache miss'
 *
 * */
bool cache_address_access(uint32_t mem_address) {
    uint32_t mem_block_num;
    uint32_t set_number;
    uint32_t tag;
    cache_block *set_ptr;
    uint32_t block_number;
    uint32_t LRU_item_idx;
    uint32_t time_difference;
    // a certain memory block number that the address exists in.
    mem_block_num = mem_address / (4 * block_size);

    // Get tag and mapped set number.
    tag = mem_block_num >> log2_num_of_sets;
    set_number = mem_block_num - (tag << log2_num_of_sets);
    // Find the address of the set.
    set_ptr = cache + (set_number * set_degree);

    // Check hit
    for (block_number = 0;
         block_number < set_degree;
         block_number++) {
        if (set_ptr[block_number].valid && set_ptr[block_number].tag == tag) {
            set_ptr[block_number].access_time = cycle;
            return true;
        }
    }

    LRU_item_idx = 0;
    time_difference = 0;
    // Miss, move the memory block to cache block.
    for (block_number = 0;
         block_number < set_degree;
         block_number++) {
        if (set_ptr[block_number].valid == false) {
            LRU_item_idx = block_number;
            break;
        }
        else {
            uint32_t pre_time_diff = time_difference;
            time_difference =
                max(cycle - set_ptr[block_number].access_time, time_difference);
            LRU_item_idx = (time_difference > pre_time_diff) ? LRU_item_idx = block_number : LRU_item_idx;
        }
    }

    // Replace the LRU item with the new item.
    set_ptr[LRU_item_idx] = (cache_block){.access_time = cycle, .tag = tag, .valid = 1};
    return false;
}
