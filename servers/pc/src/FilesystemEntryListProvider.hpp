#pragma once

#include "FilesystemEntryList.hpp"

#include <filesystem>
#include <fstream>

namespace hd
{
    namespace filesystem
    {
        struct FilesystemEntryListProvider
        {
            static FilesystemEntryList getFilesystemEntryList( const std::string &path )
            {
                FilesystemEntryList list;

                if( fs::exists( path ) 
                    && fs::is_directory( path ) )
                {
                    for( auto& p : fs::recursive_directory_iterator( path ) )
                        list.add( FilesystemEntry::create( p, path ) );
                }

                return list;
            }
        };
    }
}