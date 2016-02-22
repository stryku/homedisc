#pragma once

#include <md5/md5.h>

#include <memory>
#include <istream>

namespace Md5
{
    std::string hash( std::istream &in )
    {
        static const size_t BufferSize = 1024*1024;

        size_t numBytesRead;
        MD5 md5;
        std::unique_ptr<char> buffer = std::make_unique<char>();

        while( in )
        {
            in.read( buffer.get(), BufferSize );
            numBytesRead = in.gcount();

            md5.add( buffer.get(), numBytesRead );
        }

        return md5.getHash();
    }
};
