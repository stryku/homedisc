#pragma once

#include "RequestBuilder.hpp"

#include <zmq.hpp>

#include <memory>
#include <string>

namespace HD
{
    namespace Communication
    {
        namespace SimpleRequests
        {
            auto fileList()
            {
                RequestBuilder builder;

                builder.setType( MessageType::LIST_REQ );

                return builder.build();
            }

            auto file( const std::string &path )
            {
                RequestBuilder builder;

                builder.setType( MessageType::FILE_REQ );
                builder.setPath( path );

                return builder.build();
            }

            auto remove( const std::string &path )
            {
                RequestBuilder builder;

                builder.setType( MessageType::REMOVE_FILE );
                builder.setPath( path );

                return builder.build();
            }

            auto removeDirectory( const std::string &path )
            {
                RequestBuilder builder;

                builder.setType( MessageType::REMOVE_DIR );
                builder.setPath( path );

                return builder.build();
            }

            auto newFile( const std::string &path, const std::string &base64File )
            {
                RequestBuilder builder;

                builder.setType( MessageType::NEW_FILE );
                builder.setPath( path );
                builder.setFile( base64File );

                return builder.build();
            }

            auto newDirectory( const std::string &path )
            {
                RequestBuilder builder;

                builder.setType( MessageType::NEW_DIR );
                builder.setPath( path );

                return builder.build();
            }
        }
    }
}