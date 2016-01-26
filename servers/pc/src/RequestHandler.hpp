#pragma once

#include "FilesListResponse.hpp"
#include "FileResponse.hpp"
#include <FilesystemAffect.hpp>
#include <MessageType.hpp>

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
            MessageType getMsgType( const pt::ptree &msgTree )
            {
                auto strType = msgTree.get_child( "request.type" ).get_value( "" );

                return stringToMessageType( strType );
            }

        public:
            auto handleFileRequest( const pt::ptree &tree )
            {
                auto relativePath = tree.get_child( "request.path" ).get_value( "" );

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
                        return  FilesListResponse( "C:/moje/programowanie/c++/HomeDisc/servers/pc/test" ).toZmqMessage();//todo

                    case MessageType::FILE_REQ:
                        LOG( "FILE_REQ" );
                        return  handleFileRequest( tree );

                    case MessageType::NEW_FILE:
                        filesystem::FilesystemAffect::createFileFromBase64( "C:/moje/programowanie/c++/HomeDisc/servers/pc/test" + tree.get_child( "request.path" ).get_value( "" ),// todo
                                                                            tree.get_child( "msg.file" ).get_value( "" ) );
                    break;

                    case MessageType::NEW_DIR:
                    filesystem::FilesystemAffect::createDirectory( "C:/moje/programowanie/c++/HomeDisc/servers/pc/test" + tree.get_child( "request.path" ).get_value( "" ) );// todo
                    break;

                    case MessageType::REMOVE_FILE:
                        std::experimental::filesystem::remove( "C:/moje/programowanie/c++/HomeDisc/servers/pc/test" + tree.get_child( "request.path" ).get_value( "" ) );// todo
                    break;

                    case MessageType::REMOVE_DIR:
                        std::experimental::filesystem::remove_all( "C:/moje/programowanie/c++/HomeDisc/servers/pc/test" + tree.get_child( "request.path" ).get_value( "" ) );// todo
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