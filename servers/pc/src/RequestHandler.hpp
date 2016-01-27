#pragma once

#include "FilesListResponse.hpp"
#include "FileResponse.hpp"
#include <FilesystemAffect.hpp>
#include <MessageType.hpp>
#include <MainFolderPath.hpp>

#include <router/log.h>

#include <zmq.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <functional>

namespace HD
{
    namespace Communication
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
            auto handleFileRequest( const pt::ptree &tree ) const
            {
                auto relativePath = tree.get_child( "request.path" ).get_value( "" );

                return FileResponse( relativePath ).toZmqMessage();
            }

            auto handleListRequest() const
            {
                return  FilesListResponse( Filesystem::getMainFolderPath() ).toZmqMessage();
            }

            auto handleNewFile( const pt::ptree &tree ) const
            {
                auto relativePathToFile = tree.get_child( "request.path" ).get_value( "" );
                auto fullPathToFileToCreate = Filesystem::getMainFolderPath() + relativePathToFile;
                auto base64File = tree.get_child( "msg.file" ).get_value( "" );

                Filesystem::FilesystemAffect::createFileFromBase64( fullPathToFileToCreate,
                                                                    base64File );

                return std::make_shared<zmq::message_t>(); //TODO
            }

            auto handleNewDir( const pt::ptree &tree ) const
            {
                auto relativePathToDir = tree.get_child( "request.path" ).get_value( "" );
                auto fullPathToDirToCreate = Filesystem::getMainFolderPath() + relativePathToDir;

                Filesystem::FilesystemAffect::createDirectory( fullPathToDirToCreate );

                return std::make_shared<zmq::message_t>(); //TODO
            }

            auto handleRemoveFile( const pt::ptree &tree ) const
            {
                auto relativePathToFile = tree.get_child( "request.path" ).get_value( "" );
                auto fullPathToFileToDelete = Filesystem::getMainFolderPath() + relativePathToFile;

                Filesystem::FilesystemAffect::deleteFile( fullPathToFileToDelete );// todo

                return std::make_shared<zmq::message_t>(); //TODO
            }

            auto handleRemoveDir( const pt::ptree &tree ) const
            {
                auto relativePathToDir = tree.get_child( "request.path" ).get_value( "" );
                auto fullPathToDirToDelete = Filesystem::getMainFolderPath() + relativePathToDir;

                Filesystem::FilesystemAffect::deleteDirectory( fullPathToDirToDelete );// todo

                return std::make_shared<zmq::message_t>(); //TODO
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
                        return handleListRequest();

                    case MessageType::FILE_REQ:
                        LOG( "FILE_REQ" );
                        return  handleFileRequest( tree );

                    case MessageType::NEW_FILE:
                        LOG( "NEW_FILE" );
                        return handleNewFile( tree );

                    case MessageType::NEW_DIR:
                        LOG( "NEW_DIR" );
                        return handleNewDir( tree );

                    case MessageType::REMOVE_FILE:
                        LOG( "REMOVE_FILE" );
                        return handleRemoveFile( tree );

                    case MessageType::REMOVE_DIR:
                        LOG( "REMOVE_DIR" );
                        return handleRemoveDir( tree );

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