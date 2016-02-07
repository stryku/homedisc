#pragma once

#include <b64/decode.h>

#include <sstream>
#include <fstream>
#include <string>
#include <boost/filesystem.hpp>

namespace HD
{
    namespace Filesystem
    {
        namespace FilesystemAffect
        {
            void createFileFromBase64( const std::string &path, const std::string &b64 )
            {
                base64::decoder decoder;
                std::istringstream iss( b64 );
                std::ofstream out( path, std::ios::binary );

                decoder.decode( iss, out );
            }

            bool createDirectory( const std::string &path )
            {
                return boost::filesystem::create_directory( path );
            }

            void deleteDirectory( const std::string &path )
            {
                boost::filesystem::remove_all( path );//todo error check
            }

            void deleteFile( const std::string &path )//todo error check
            {
                boost::filesystem::remove( path );
            }
        }
    }
}
