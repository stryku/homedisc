#pragma once

#include <b64/decode.h>
#include <b64/encode.h>

#include <router/log.h>

#include <zmq.hpp>

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <filesystem>
#include <utility>
#include <string>
#include <memory>

namespace hd
{
    namespace communication
    {
        class FileResponse
        {
        private:
            typedef std::shared_ptr<zmq::message_t> ZmqMessagePtr;

        public:
            FileResponse( const std::experimental::filesystem::path &relativePath ) :
                relativePath( relativePath )
            {}

            auto toZmqMessage() const
            {
                const char *folder = "C:/moje/programowanie/c++/HomeDisc/servers/pc/test";//todo

                auto p = std::experimental::filesystem::path( folder ) / relativePath;

                std::ifstream file( std::experimental::filesystem::path(folder) / relativePath, std::ios::binary );//todo
                base64::encoder encoder;
                boost::property_tree::ptree tree;
                std::ostringstream oss;

                encoder.encode( file, oss );

                tree.add( "resp.path", relativePath.string() );
                tree.add( "resp.file", oss.str() );

                oss.str( "" );
                oss.clear();

                boost::property_tree::write_xml( oss, tree );

                auto str = oss.str() + '\0';

                return std::make_shared<zmq::message_t>( str.begin(), str.end() );
            }

        private:
            std::experimental::filesystem::path relativePath;
        };
    }
}