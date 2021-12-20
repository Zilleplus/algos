# summary chapter 7: use case RLU cache

## Generic cache

*ADT:*
`
class cache:
    init(maxSize)
    get(key)
    set(set)
    size()
`

The values in a cache can go _stale_. 

A cache collision happens when 2 processes try to access the same not avaialable cache value. This will cause the cache to be calculated twice. This can be avoided by adding a _loading_ state.

## LRU: Last Recently Used

operations:
- store
- check if available
- get by name
- get number of stored elements
- get oldest entry

The entries can be stored in:
1. ballanced tree: good trade off
2. hash table: fast access slow removal of oldest
3. linked list: slow on retrieval but fast removal of oldest.

`
LRUCache
    maxSize: int
    hashTable: HashTable<name>
    elements: list
    tail: node
`

To get fast access and fast removal both a hash table and linked list can be used. The linked list containts the values itself, and the hash table containts pointer to the values. This to avoid depencies on internal hash table implementation. 

## LFU/MFU Least fequently used/ most frequently used
