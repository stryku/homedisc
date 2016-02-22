#pragma once

#include <md5/md5.h>

#include <memory>
#include <stdexcept>
#include <fstream>
#include <string>

namespace Md5
{
    std::string hashStream( std::istream &in )
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

    std::string hashFile( const std::string &filename )
    {
        std::ifstream in( filename, std::ios::binary );

        if( !in.good() )
            throw std::runtime_error( "Can't open file: " + filename );

        return hashStream( in );
    }

};
