#pragma once

#include <FilesystemEntryListProvider.hpp>

#include <router/log.h>

#include <zmq.hpp>

#include <filesystem>
#include <utility>
#include <string>
#include <memory>

namespace HD
{
    namespace Communication
    {
        class FilesListResponse
        {
        private:
            typedef std::shared_ptr<zmq::message_t> ZmqMessagePtr;

        public:
            FilesListResponse( const std::experimental::filesystem::path &path ) :
                path( path )
            {}

            auto toZmqMessage() const
            {
                Filesystem::FilesystemEntryList list;
                
                list.generate( path.string() );
                auto strXml = list.toXml() + '\0';

                std::ofstream( "serv.txt" ) << strXml;
                //LOG( strXml );

                //zmq::message_t msg( const_cast<char*>( strXml.c_str() ), strXml.size() + 1, nullptr );
                //LOG( static_cast<const char*>( msg.data() ) );

                return std::make_shared<zmq::message_t>( strXml.begin(), strXml.end() );
            }

            std::string toString() const
            {
                auto list = Filesystem::FilesystemEntryListProvider::getFilesystemEntryList( path.string() );
                return list.toXml();
            }

        private:
            std::experimental::filesystem::path path;
        };
    }
}