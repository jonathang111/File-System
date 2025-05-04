# File-System
Current TO DOS:
1. Need to create sort system, either into sort into cache, or sort then into cache
2. Handle extensions with multiple '.'
3. Figure out key method for caching
4. QT framework
5. Figure out whether to store filters in another cache
6. Path parsing for directory hierarchy or entry rework

Finished TO DOS:
- symlink issues, fixed using lstat() instead
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

Another problem is when the WSL needs to access mnt/whatever; at this point it needs to go through multiple virtual layers leading to higher latency when using lstat or opendir.