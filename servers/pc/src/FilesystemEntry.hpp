#pragma once

#include <md5/md5.hpp>

#include <string>
#include <filesystem>
#include <fstream>
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
                MD5 md5;

                auto relativePath = dirEntry.path().string().substr( pathToDirectory.string().length() );

                entry.path = relativePath;
                entry.type = ( fs::is_directory( dirEntry ) ? FilesystemEntryType::DIRECTORY : FilesystemEntryType::FILE );

                auto tt = std::chrono::system_clock::to_time_t( fs::last_write_time( dirEntry ) );

                auto tm = *gmtime( &tt );

                entry.modificationDate = std::to_string( tm.tm_year ) +
                                            std::to_string( tm.tm_mon ) +
                                            std::to_string( tm.tm_mday ) +
                                            std::to_string( tm.tm_hour ) +
                                            std::to_string( tm.tm_min ) +
                                            std::to_string( tm.tm_sec );

                if( entry.type != FilesystemEntryType::DIRECTORY )
                    entry.md5 = md5.digestFile( dirEntry.path().string().c_str() );

                return entry;
            }

            auto hash() const
            {
                std::hash<std::string> h;
                std::hash<FilesystemEntryType> ht;

                return h( md5 ) ^ h( modificationDate ) ^ h( path.string() ) ^ ht( type );
            }

            bool operator==( const FilesystemEntry &other ) const
            {
                return path == other.path
                    && md5 == other.md5
                    && modificationDate == other.modificationDate
                    && type == other.type;
            }

            std::string stringType() const
            {
                return ( type == FilesystemEntryType::FILE ? "FILE" : "DIR" );
            }

            friend std::ostream& operator<<( std::ostream &out, const FilesystemEntry &entry )
            {
                out << "path: " << entry.path << "\n" \
                    << "md5: " << entry.md5 << "\n" \
                    << "type: " << entry.stringType() << "\n" \
                    << "mod time: " << entry.modificationDate;

                return out;
            }
        };
    }
}

namespace std
{
    template <>
    struct hash<std::experimental::filesystem::path>
    {
        size_t operator () ( const std::experimental::filesystem::path &p ) const
        {
            return std::hash<std::string>{}( p.string() );
        }
    };
}