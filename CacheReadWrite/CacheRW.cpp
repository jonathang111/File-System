#include "CacheRW.h"

namespace CacheRW{

std::ostream& operator<<(std::ostream& os, const KeyIndexEntry& entry){ //quick print key entry
    os << "[Key: " << entry.key
        << ", Offset: " << entry.offset
        << ", Count: " << entry.count << "]";
    return os;
}

}