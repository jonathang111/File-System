#include "CacheRW.h"

namespace CacheRW::Internal{

size_t ComputeOffset(size_t EntryEnd){
size_t roughOffset = static_cast<size_t>(EntryEnd * GROWTH_FACTOR);
size_t allignedOffset = static_cast<size_t>(roughOffset + (BLOCK_SIZE-1)) & ~ (BLOCK_SIZE-1);

return allignedOffset;
}

}


namespace CacheRW{

std::ostream& operator<<(std::ostream& os, const KeyIndexEntry& entry){ //quick print key entry
    os << "[Key: " << entry.key
        << ", Offset: " << entry.offset
        << ", Count: " << entry.count << "]";
    return os;
}

std::optional<CacheMeta> ReadCacheMetaData(StorageInterface& storage){ //optional returns pointer to cacheDB or nothing.
    CacheHeader header;

    if(!storage.isOpen()){ //maybe remove or somehow add name of file.
        std::cout << "Could not open file\n";
         return {};
    }
    storage.seek(0);
    storage.read(&header, sizeof(CacheHeader));
    std::string name = storage.getLabel();
    CacheMeta output(header, name.c_str());
    return output;
}

auto GetFooterArray(CacheMeta cacheMetaData)
    -> std::optional<KeyIndexEntry*>{
    std::ifstream file(cacheMetaData.cacheName, std::ios::binary);
    KeyIndexEntry* output;
    if(file.is_open()){
        output = new KeyIndexEntry[cacheMetaData.keyamt];
        file.seekg(cacheMetaData.footerStart);
        file.read(reinterpret_cast<char*>(output), sizeof(KeyIndexEntry)*cacheMetaData.keyamt);

        return output;
    }
    std::cout << "Could not open " << cacheMetaData.cacheName << std::endl;
    return {};
}

}