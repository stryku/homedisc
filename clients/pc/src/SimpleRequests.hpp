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
            auto simpleReq( const std::string &content )
            {
                return std::make_shared<zmq::message_t>( content.begin(), content.end() );
            }

            auto fileList()
            {
                return simpleReq( "<msg><type>LIST_REQ</type></msg>" );
            }

            auto file()
            {
                return simpleReq( "<msg><type>FILE_REQ</type></msg>" );
            }
        }
    }
}