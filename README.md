# File-System
Current TO DOS:
1. Create key sorts for multiple sorting types, then upload to cache.
2. Handle extensions with multiple '.'
4. QT framework

Finished TO DOS:
- symlink issues, fixed using lstat() instead
- memleak
- database creation

Notes:
As of this moment i need to figure out how to sort, cache, and isolate specific file types. Should i create a filter which reprocesses the db and filters the file types i allow?

Cache Layout:
Header:
    magic: "0xFDB,0x01" //just a beginning tag, use to veirfy correct cache.
    section count: 8
SectionHeader[0]:
    name: "all"
    offset: 8 + sizeof(SectionHeader) * 8
    count: 176000
SectionHeader[1]:
    name: "PDF"
    offset: 8 + sizeof(SectionHeader) * 8 + sizeof(Entry) * 176000
    count 1200
SectionHeader[2]:
    etc...

Possible section with directories keyed, each diretory holds some link to the files (maybe reference back to "all" section?) in the directory. So when user requests to see other files in directory, just hash the dir name and use that section to find all other files, then load and show to user.

Another problem is when the WSL needs to access mnt/whatever; at this point it needs to go through multiple virtual layers leading to higher latency when using lstat or opendir. Just for reference, on comparable hardware it takes a WSL ~800 secs to scan ~600k files and the mac 85 sceonds to do ~1mil files.

Cannot replace memcpy, the problem is that in a WSL memory isn't freed until program end. Meaning that if i delete each individual allocated memory in the Entry structure for each index, then it means I'm effectively "freeing" something that can't be used. This translates to double the memory usage every IncreaseDB since old memory is still technically allocated by the WSL. The best approach is to keep memcpy implemented since the char* are manually allocated with new, thus deleteing the old database with delete[] temp2 won't deallocate those pointers. To do that id have to iterate through all the indecies and manually delete each with delete[], but again the problem is that it isn't really deallocated according to the WSL, just not accessable.

One idea to improve the speed of database scanning on WSLs is to use multithreading. 
Instead of having the database recursively open directories and scan, I could instead place it into a thread-safe queue and let other cores scan the file independently. Would need mutex to gaurd the DB since it would be a shared space.

Map will store each key into a group based on the extension, so all I have to do is create the map, sort the keys of the map by alphabetical order, then insert the map by key while tracking its size, and position in the list.

Map will compare addresses if you key by char*, use string instead to compare the actual data.