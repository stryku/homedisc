#pragma once

#include "FilesListResponse.hpp"
#include "FileResponse.hpp"
#include <FilesystemAffect.hpp>

#include <router/log.h>

#include <zmq.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <functional>

namespace hd
{
    namespace communication
    {
        namespace pt = boost::property_tree;

        class RequestHandler
        {
        private:
            enum class MessageType
            {
                LIST_REQ,
                FILE_REQ,
                NEW_FILE,
                REMOVE,

                UNDEF
            };

            MessageType getMsgType( const pt::ptree &msgTree )
            {
                auto strType = msgTree.get_child( "msg.type" ).get_value( "" );

                if( strType == "LIST_REQ" ) return MessageType::LIST_REQ;
                if( strType == "FILE_REQ" ) return MessageType::FILE_REQ;
                if( strType == "NEW_FILE" ) return MessageType::NEW_FILE;
                if( strType == "REMOVE" ) return MessageType::REMOVE;

                return MessageType::UNDEF;
            }

        public:

            auto handleFileRequest( const pt::ptree &tree )
            {
                auto relativePath = tree.get_child( "msg.path" ).get_value( "" );

                return FileResponse( relativePath ).toZmqMessage();
            }

            auto handle( const zmq::message_t &msg )
            {
                std::string strMsg( static_cast<const char*>( msg.data() ) );
                std::string strResponse;
                pt::ptree tree;

                pt::read_xml( std::istringstream( strMsg ), tree );

                switch( getMsgType( tree ) )
                {
                    case MessageType::LIST_REQ: 
                        LOG( "LIST_REQ" );
                        return  FilesListResponse( "C:/moje/programowanie/c++/HomeDisc/servers/pc/test" ).toZmqMessage();

                    case MessageType::FILE_REQ:
                        LOG( "FILE_REQ" );
                        return  handleFileRequest( tree );

                    case MessageType::NEW_FILE:
                        filesystem::FilesystemAffect::createFileFromBase64( "C:/moje/programowanie/c++/HomeDisc/servers/pc/test" + tree.get_child( "msg.path" ).get_value( "" ),// todo
                                                                            tree.get_child( "msg.file" ).get_value( "" ) );
                    break;

                    case MessageType::REMOVE:
                        std::experimental::filesystem::remove( "C:/moje/programowanie/c++/HomeDisc/servers/pc/test" + tree.get_child( "msg.path" ).get_value( "" ) );// todo
                    break;
                    default:
                    break;
                }

                return std::make_shared<zmq::message_t>();
            }

            auto getHandleFunction()
            {
                return std::bind( &RequestHandler::handle, this, std::placeholders::_1 );
            }
        };
    }
}