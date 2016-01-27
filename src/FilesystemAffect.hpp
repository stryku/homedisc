#pragma once

#include <b64/decode.h>

#include <sstream>
#include <fstream>
#include <string>
#include <filesystem>

namespace hd
{
    namespace filesystem
    {
        namespace FilesystemAffect
        {
            void createFileFromBase64( const std::string &path, const std::string &b64 )
            {
                base64::decoder decoder;

                decoder.decode( std::istringstream( b64 ), 
                                std::ofstream( path, std::ios::binary ) );
            }

            bool createDirectory( const std::string &path )
            {
                return std::experimental::filesystem::create_directory( path );
            }

            void deleteDirectory( const std::string &path )
            {
                std::experimental::filesystem::remove_all( path );//todo error check
            }

            void deleteFile( const std::string &path )//todo error check
            {
                std::experimental::filesystem::remove( path );
            }
        }
    }
}