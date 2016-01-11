#pragma once

#include <b64/decode.h>

#include <sstream>
#include <fstream>
#include <string>

namespace hd
{
    namespace filesystem
    {
        namespace FilesystemAffect
        {
            void createFileFromBase64( const std::string &path, const std::string &b64 )
            {
                base64::decoder decoder;

                decoder.decode( std::istringstream( b64 ), std::ofstream( path, std::ios::binary ) );
            }
        }
    }
}