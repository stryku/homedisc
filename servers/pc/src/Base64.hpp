#pragma once

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/remove_whitespace.hpp>

#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

namespace utils
{
    namespace base64
    {
        std::string encode( const std::vector<uint8_t> &bin )
        {
            namespace it = boost::archive::iterators;
            typedef it::insert_linebreaks<it::base64_from_binary<it::transform_width<std::vector<uint8_t>::const_iterator, 6, 8> >, 72 > it_base64_t;

            unsigned int writePaddChars = ( 3 - bin.size() % 3 ) % 3;
            std::string encodedBase64( it_base64_t( bin.begin() ), it_base64_t( bin.end() ) );
            encodedBase64.append( writePaddChars, '=' );

            return encodedBase64;
        }

        std::string encode( std::istream &in )
        {
            std::vector<uint8_t> data;

            std::copy( std::istream_iterator<uint8_t>( in ),
                       std::istream_iterator<uint8_t>(),
                       std::back_inserter( data ) );

            return encode( data );
        }

        std::vector<uint8_t> decode( const std::string &strBase64 )
        {
            namespace it = boost::archive::iterators;
            typedef it::transform_width< it::binary_from_base64<it::remove_whitespace<std::string::const_iterator> >, 8, 6 > it_binary_t;

            unsigned int paddChars = std::count( strBase64.begin(), strBase64.end(), '=' );
            std::replace( strBase64.begin(), strBase64.end(), '=', 'A' ); // replace '=' by base64 encoding of '\0'
            std::vector<uint8_t> result( it_binary_t( strBase64.begin() ), it_binary_t( strBase64.end() ) ); // decode
            result.erase( result.end() - paddChars, result.end() );  // erase padding '\0' characters

            return result;
        }
    }
}