#pragma once

#include <MessageType.hpp>

#include <zmq.hpp>

#include <memory>

namespace hd
{
    namespace communication
    {
        class RequestBuilder
        {
        public:
            RequestBuilder& setType( MessageType newType )
            {
                type = newType;
                return *this;
            }

            RequestBuilder& setFile( const std::string &newFile )
            {
                file = newFile;
                return *this;
            }

            RequestBuilder& setPath( const std::string &newPath )
            {
                path = newPath;
                return *this;
            }

            std::shared_ptr<zmq::message_t> build() const
            {
                auto content = createContent();

                return std::make_shared<zmq::message_t>( content.begin(), content.end() );
            }

        private:
            std::string createContent() const
            {
                if( type == MessageType::UNDEF )
                    return ""; // todo throw

                std::string content = "<request><type>";

                content += messageTypeToString( type ) + "</type>";

                if( path.length() > 0 )
                    content += "<path>" + path + "</path>";

                if( file.length() > 0 )
                    content += "<file>" + file + "</file>";

                content += "</request>\0";

                return content;
            }

            MessageType type = MessageType::UNDEF;
            std::string file;
            std::string path;
        };
    }
}