#include "CacheRW.h"

namespace CacheRW{

std::ostream& operator<<(std::ostream& os, const KeyIndexEntry& entry){ //quick print key entry
    os << "[Key: " << entry.key
        << ", Offset: " << entry.offset
        << ", Count: " << entry.count << "]";
    return os;
}

size_t ComputeOffset(size_t EntryEnd){
    size_t roughOffset = static_cast<size_t>(EntryEnd * GROWTH_FACTOR);

    size_t allignedOffset = static_cast<size_t>(roughOffset + (BLOCK_SIZE-1)) & ~ (BLOCK_SIZE-1);

    return allignedOffset;
}
}