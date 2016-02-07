#pragma once

#include <md5/md5.hpp>

#include <string>
#include <boost/filesystem.hpp>
#include <fstream>
#include <chrono>
#include <ctime>
#include <stdexcept>

namespace HD
{
    namespace Filesystem
    {
        namespace fs = boost::filesystem;

        enum class FilesystemEntryType
        {
            FILE,
            DIRECTORY,
            UNDEF
        };

        std::string filesystemEntryTypeToString( FilesystemEntryType type )
        {
            switch( type )
            {
                case FilesystemEntryType::FILE: return "FILE";
                case FilesystemEntryType::DIRECTORY: return "DIRECTORY";
                case FilesystemEntryType::UNDEF: return "UNDEF";
                default: return "UNDEF";
            }
        }

        std::size_t filesystemEntryTypeHash( FilesystemEntryType type )
        {
            switch( type )
            {
                case FilesystemEntryType::FILE: return 1;
                case FilesystemEntryType::DIRECTORY: return 2;
                case FilesystemEntryType::UNDEF: return 3;
                default: return -1;
            }
        }

        FilesystemEntryType stringtoFilesystemEntryType( const std::string &s )
        {
            if( s == "FILE" ) return FilesystemEntryType::FILE;
            if( s == "DIRECTORY" ) return FilesystemEntryType::DIRECTORY;

            return FilesystemEntryType::UNDEF;
        }

        class UndefEntryType : public std::runtime_error
        {
        public:
            UndefEntryType( const char *msg = "Undefined entry type" ) :
                std::runtime_error( msg )
            {}
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

                auto lastWrite = fs::last_write_time( dirEntry );

//                 auto tt = std::chrono::system_clock::to_time_t( lastWrite );

                auto tm = *gmtime( &lastWrite );

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

                return h( md5 ) ^ h( modificationDate ) ^ h( path.string() ) ^ filesystemEntryTypeHash( type );
            }

            bool operator==( const FilesystemEntry &other ) const
            {
                return path == other.path
                    && md5 == other.md5
                    && modificationDate == other.modificationDate
                    && type == other.type;
            }

            bool olderThan( const FilesystemEntry &other ) const
            {
                return modificationDate < other.modificationDate;
            }

            static bool compareForSet( const FilesystemEntry &lhs, const FilesystemEntry &rhs )
            {
                if( lhs.type != rhs.type || lhs.type == FilesystemEntryType::FILE )
                {
                    if( lhs.type == FilesystemEntryType::DIRECTORY && rhs.type == FilesystemEntryType::FILE )
                        return true;

                    if( lhs.type == FilesystemEntryType::FILE && rhs.type == FilesystemEntryType::DIRECTORY )
                        return false;
                }

                return lhs.path.string().length() < rhs.path.string().length();
            }

            struct SetPred
            {
                bool operator()( const FilesystemEntry &lhs, const FilesystemEntry &rhs )
                {
                    if( lhs.type != rhs.type || lhs.type == FilesystemEntryType::FILE )
                    {
                        if( lhs.type == FilesystemEntryType::DIRECTORY && rhs.type == FilesystemEntryType::FILE )
                            return true;

                        if( lhs.type == FilesystemEntryType::FILE && rhs.type == FilesystemEntryType::DIRECTORY )
                            return false;
                    }

                    return lhs.path.string() < rhs.path.string();
                }
            };

            /*bool operator<( const FilesystemEntry &rhs ) const
            {
                if( type != rhs.type || type == FilesystemEntryType::FILE )
                {
                    if( type == FilesystemEntryType::DIRECTORY && rhs.type == FilesystemEntryType::FILE )
                        return true;

                    if( type == FilesystemEntryType::FILE && rhs.type == FilesystemEntryType::DIRECTORY )
                        return false;
                }

                return path.string().length() < rhs.path.string().length();
            }*/

            std::string stringType() const
            {
                return filesystemEntryTypeToString( type );
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
    struct hash<boost::filesystem::path>
    {
        size_t operator () ( const boost::filesystem::path &p ) const
        {
            return std::hash<std::string>{}( p.string() );
        }
    };
}
