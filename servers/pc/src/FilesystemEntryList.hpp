#pragma once

#include "FilesystemEntry.hpp"

#include <unordered_set>

namespace hd
{
    namespace filesystem
    {
        class FilesystemEntryList
        {
        
        private:
            std::unordered_set<FilesystemEntry> entries;

        public:
            void add( const FilesystemEntry &entry )
            {
                entries.insert( entry );
            }
        };
    }
}