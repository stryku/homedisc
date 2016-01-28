#pragma once

#include "Communicator.hpp"
#include "Errors.hpp"
#include <FilesystemEntryList.hpp>
#include <FilesystemEntryListProvider.hpp>
#include <MainFolderPath.hpp>
#include <FilesystemAffect.hpp>

#include <b64/decode.h>
#include <b64/encode.h>

#include <zmq.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <memory>
#include <fstream>
#include <vector>

namespace HD
{
    namespace Communication
    {
        class ResponseHandler
        {
        private:
            typedef std::shared_ptr<zmq::message_t> ZmqMessagePtr;
            typedef std::vector<Filesystem::FilesystemEntryDifference> Differences;

            //todo
            auto getDifferences( const std::string &serverFilesListXml )
            {
                Filesystem::FilesystemEntryList serverList;

                filesystemEntryList.generate( Filesystem::getMainFolderPath() );

                try
                {
                    serverList.fromXml( serverFilesListXml );
                    return filesystemEntryList.getDifferences( serverList );
                }
                catch( boost::property_tree::xml_parser_error &e )
                {
                    Error::reportError( Error::ErrorType::BROKEN_LIST_FROM_SERVER, e.what() );
                }

                return Differences();
            }

            void saveFile( ZmqMessagePtr msgWithFile )
            {
                namespace pt = boost::property_tree;

                pt::ptree tree;
                std::string strMsg( static_cast<const char*>( msgWithFile->data() ) );
                std::string path( Filesystem::getMainFolderPath() );

                pt::read_xml( std::istringstream( strMsg ), tree );

                path += tree.get_child( "resp.path" ).get_value( "" );

                auto base64File = tree.get_child( "resp.file" ).get_value( "" );

                Filesystem::FilesystemAffect::createFileFromBase64( path, base64File );
            }

            void downloadFile( const std::string &relativePath )
            {
                auto msg = SimpleRequests::file( relativePath );

                communicator.send( msg );

                auto responseWithFile = communicator.recv();

                saveFile( responseWithFile );
            }

            void uploadFile( const std::string &relativePath )
            {
                auto fullpath = Filesystem::getMainFolderPath() + relativePath; //todo
                base64::encoder encoder;
                std::ostringstream oss;
                
                encoder.encode( std::ifstream( fullpath, std::ios::binary ), oss );

                auto msg = SimpleRequests::newFile( relativePath, oss.str() );

                communicator.send( msg );

                auto responseOk = communicator.recv();
            }

            void deleteFile( const std::string &relativePath )//todo error check
            {
                auto fullpath = Filesystem::getMainFolderPath() + relativePath; //todo

                Filesystem::FilesystemAffect::deleteFile( fullpath );
            }

            void createDirectory( const std::string &relativePath )
            {
                auto fullpath = Filesystem::getMainFolderPath() + relativePath; 

                Filesystem::FilesystemAffect::createDirectory( fullpath );//todo error check
            }

            void deleteDirectory( const std::string &relativePath )
            {
                auto fullpath = Filesystem::getMainFolderPath() + relativePath; 

                Filesystem::FilesystemAffect::deleteDirectory( fullpath );//todo error check
            }

            void deleteRemoteFile( const std::string &path )
            {
                auto msg = SimpleRequests::remove( path );
                communicator.send( msg );

                auto responseOk = communicator.recv();
            }

            void deleteRemoteDirectory( const std::string &path )//TODO move to another method
            {
                auto msg = SimpleRequests::removeDirectory( path );
                communicator.send( msg );

                auto responseOk = communicator.recv();
            }

            void createRemoteDirectory( const std::string &path )//TODO move to another method
            {
                auto msg = SimpleRequests::newDirectory( path );
                communicator.send( msg );

                auto responseOk = communicator.recv();
            }

            void handleDifferences( const Differences &differences )
            {

                for( const auto &diff : differences )
                {
                    switch( diff.type )
                    {
                        case Filesystem::DifferenceType::CHANGED_FILE_REMOTE:
                        case Filesystem::DifferenceType::NEW_FILE_REMOTE:
                            downloadFile( diff.entryPath.string() );
                        break;

                        case Filesystem::DifferenceType::NEW_DIR_REMOTE:
                            createDirectory( diff.entryPath.string() );
                        break;

                        case Filesystem::DifferenceType::DELETED_FILE_REMOTE:
                            deleteFile( diff.entryPath.string() );
                        break;

                        case Filesystem::DifferenceType::DELETED_DIR_REMOTE:
                            deleteDirectory( diff.entryPath.string() );
                        break;

                        case Filesystem::DifferenceType::DELETED_FILE_LOCALLY:
                            deleteRemoteFile( diff.entryPath.string() );
                        break;

                        case Filesystem::DifferenceType::DELETED_DIR_LOCALLY:
                            deleteRemoteDirectory( diff.entryPath.string() );
                        break;

                        case Filesystem::DifferenceType::CHANGED_FILE_LOCALLY:
                        case Filesystem::DifferenceType::NEW_FILE_LOCALLY:
                            uploadFile( diff.entryPath.string() );
                        break;

                        case Filesystem::DifferenceType::NEW_DIR_LOCALLY:
                            createRemoteDirectory( diff.entryPath.string() );
                        break;
                    }
                }
            }

        public:
            ResponseHandler( Communicator &communicator ) :
                communicator( communicator )
            {
                filesystemEntryList.generate( Filesystem::getMainFolderPath() );
            }

            void handle( ZmqMessagePtr msg )
            {
                std::string strMsg( static_cast<const char*>( msg->data() ) );

                auto differences = getDifferences( strMsg );

                handleDifferences( differences );

                filesystemEntryList.generateOld( Filesystem::getMainFolderPath() );
            }

        private:
            Communicator &communicator;
            Filesystem::FilesystemEntryList filesystemEntryList;
        };
    }
}