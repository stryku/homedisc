#pragma once

#include <zmq.hpp>

#include <memory>
#include <string>

namespace hd
{
    namespace communication
    {
        namespace SimpleRequests
        {
            auto simpleReq( std::string content )
            {
                content += '\0';
                return std::make_shared<zmq::message_t>( content.begin(), content.end() );
            }

            auto fileList()
            {
                return simpleReq( "<msg><type>LIST_REQ</type></msg>" );
            }

            auto file( const std::string &path )
            {
                return simpleReq( "<msg><type>FILE_REQ</type><path>" + path + "</path></msg>" );
            }

            auto remove( const std::string &path )
            {
                return simpleReq( "<msg><type>REMOVE_FILE</type><path>" + path + "</path></msg>" );
            }

            auto removeDirectory( const std::string &path )
            {
                return simpleReq( "<msg><type>REMOVE_DIR</type><path>" + path + "</path></msg>" );
            }

            auto newFile( const std::string &path, const std::string &base64 )
            {
                return simpleReq( "<msg><type>NEW_FILE</type><path>" + path + "</path><file>" + base64 + "</file></msg>" );
            }

            auto newDirectory( const std::string &path )
            {
                return simpleReq( "<msg><type>NEW_DIR</type><path>" + path + "</path></msg>" );
            }
        }
    }
}