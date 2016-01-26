#pragma once

#include "Communicator.hpp"
#include <FilesystemEntryList.hpp>
#include <FilesystemEntryListProvider.hpp>
#include <b64/decode.h>
#include <b64/encode.h>

#include <zmq.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <memory>
#include <fstream>
#include <vector>

namespace hd
{
    namespace communication
    {
        class ResponseHandler
        {
        private:
            typedef std::shared_ptr<zmq::message_t> ZmqMessagePtr;
            typedef std::vector<filesystem::FilesystemEntryDifference> Differences;

            //todo
            auto getDifferences( const std::string &serverFilesListXml )
            {
                filesystem::FilesystemEntryList serverList;
                filesystemEntryList.generate( "C:/moje/programowanie/c++/HomeDisc/clients/pc/testnew" );//todo

                serverList.fromXml( serverFilesListXml );

                std::ofstream( "tmp.txt" ) << serverFilesListXml;

                return filesystemEntryList.getDifferences( serverList );
            }

            void saveFile( ZmqMessagePtr msgWithFile )
            {
                namespace pt = boost::property_tree;
                pt::ptree tree;
                std::string strMsg( static_cast<const char*>( msgWithFile->data() ) );
                std::string path( "C:/moje/programowanie/c++/HomeDisc/clients/pc/testnew" );//todo
                base64::decoder decoder;

                pt::read_xml( std::istringstream( strMsg ), tree );

                path += tree.get_child( "resp.path" ).get_value( "" );

                decoder.decode( std::istringstream( tree.get_child( "resp.file" ).get_value( "" ) ), std::ofstream( path, std::ios::binary ) );
            }

            void downloadFile( const std::string &path )
            {
                auto msg = SimpleRequests::file( path );

                communicator.send( msg );

                auto responseWithFile = communicator.recv();

                saveFile( responseWithFile );
            }

            void uploadFile( const std::string &path )
            {
                auto fullpath = "C:/moje/programowanie/c++/HomeDisc/clients/pc/testnew" + path; //todo
                base64::encoder encoder;
                std::ostringstream oss;
                
                encoder.encode( std::ifstream( fullpath, std::ios::binary ), oss );

                auto msg = SimpleRequests::newFile( path, oss.str() );

                communicator.send( msg );

                auto responseOk = communicator.recv();
            }


            void deleteFile( const std::string &path )//todo error check
            {
                auto fullpath = "C:/moje/programowanie/c++/HomeDisc/clients/pc/testnew" + path; //todo

                std::experimental::filesystem::remove( fullpath );
            }

            void deleteRemoteFile( const std::string &path )
            {
                auto msg = SimpleRequests::remove( path );
                communicator.send( msg );

                auto responseOk = communicator.recv();
            }

            void createDirectory( const std::string &path )
            {
                std::experimental::filesystem::create_directory( "C:/moje/programowanie/c++/HomeDisc/clients/pc/testnew" + path );//todo error check
            }

            void deleteDirectory( const std::string &path )
            {
                std::experimental::filesystem::remove_all( "C:/moje/programowanie/c++/HomeDisc/clients/pc/testnew" + path );//todo error check
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
                        case filesystem::DifferenceType::CHANGED_FILE_REMOTE:
                        case filesystem::DifferenceType::NEW_FILE_REMOTE:
                            downloadFile( diff.entryPath.string() );
                        break;

                        case filesystem::DifferenceType::NEW_DIR_REMOTE:
                            createDirectory( diff.entryPath.string() );
                        break;

                        case filesystem::DifferenceType::DELETED_FILE_REMOTE:
                            deleteFile( diff.entryPath.string() );
                        break;

                        case filesystem::DifferenceType::DELETED_DIR_REMOTE:
                            deleteDirectory( diff.entryPath.string() );
                        break;

                        case filesystem::DifferenceType::DELETED_FILE_LOCALLY:
                            deleteRemoteFile( diff.entryPath.string() );
                        break;

                        case filesystem::DifferenceType::DELETED_DIR_LOCALLY:
                            deleteRemoteDirectory( diff.entryPath.string() );
                        break;

                        case filesystem::DifferenceType::CHANGED_FILE_LOCALLY:
                        case filesystem::DifferenceType::NEW_FILE_LOCALLY:
                            uploadFile( diff.entryPath.string() );
                        break;

                        case filesystem::DifferenceType::NEW_DIR_LOCALLY:
                            createRemoteDirectory( diff.entryPath.string() );
                        break;
                    }
                }
            }

        public:
            ResponseHandler( Communicator &communicator ) :
                communicator( communicator )
            {
                filesystemEntryList.generate( "C:/moje/programowanie/c++/HomeDisc/clients/pc/testnew" );
            }

            void handle( ZmqMessagePtr msg )
            {
                std::string strMsg( static_cast<const char*>( msg->data() ) );

                auto differences = getDifferences( strMsg );

                handleDifferences( differences );

                filesystemEntryList.generateOld( "C:/moje/programowanie/c++/HomeDisc/clients/pc/testnew" );
            }

        private:
            Communicator &communicator;
            filesystem::FilesystemEntryList filesystemEntryList;
        };
    }
}