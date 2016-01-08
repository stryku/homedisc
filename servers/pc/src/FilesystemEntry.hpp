#pragma once

#include <string>
#include <filesystem>
#include <chrono>
#include <ctime>

namespace hd
{
    namespace filesystem
    {
        namespace fs = std::experimental::filesystem;

        enum class FilesystemEntryType
        {
            FILE,
            DIRECTORY
        };

        struct FilesystemEntry
        {
            fs::path path;
            std::string modificationDate;
            std::string md5;
            FilesystemEntryType type;

            static FilesystemEntry create( const fs::directory_entry &dirEntry,
                                           const fs::path &pathToDirectory )
            {
                FilesystemEntry entry;
                MD5 md5( std::ifstream( dirEntry.path.string() ) );

                auto relativePath = dirEntry.path.string().substr( pathToDirectory.string().length() );

                entry.path = relativePath;
                entry.md5 = md5.hex_digest();
                entry.type = ( fs::is_directory( dirEntry ) ? FilesystemEntryType::DIRECTORY : FilesystemEntryType::FILE );

                auto tt = std::chrono::system_clock::to_time_t( fs::last_write_time( dirEntry ) );

                auto tm = *gmtime( &tt );

                entry.modificationDate = std::to_string( tm.tm_year ) +
                                         std::to_string( tm.tm_mon ) +
                                         std::to_string( tm.tm_mday ) +
                                         std::to_string( tm.tm_hour ) +
                                         std::to_string( tm.tm_min ) +
                                         std::to_string( tm.tm_sec );

                return entry;
            }
        };
    }
}