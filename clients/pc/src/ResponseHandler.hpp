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
                auto ourList = filesystem::FilesystemEntryListProvider::getFilesystemEntryList( "C:/moje/programowanie/c++/HomeDisc/clients/pc/test" );//todo

                serverList.fromXml( serverFilesListXml );

                return ourList.getDifferences( serverList );
            }

            void saveFile( ZmqMessagePtr msgWithFile )
            {
                namespace pt = boost::property_tree;
                pt::ptree tree;
                std::string strMsg( static_cast<const char*>( msgWithFile->data() ) );
                std::string path( "C:/moje/programowanie/c++/HomeDisc/clients/pc/test" );//todo
                base64::decoder decoder;

                pt::read_xml( std::istringstream( strMsg ), tree );

                path += tree.get_child( "resp.path" ).get_value( "" );

                decoder.decode( std::istringstream( tree.get_child( "resp.file" ).get_value( "" ) ), std::ofstream( path ) );
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
                auto fullpath = "C:/moje/programowanie/c++/HomeDisc/clients/pc/test" + path; //todo
                base64::encoder encoder;
                std::ostringstream oss;
                
                encoder.encode( std::ifstream( fullpath ), oss );

                auto msg = SimpleRequests::newFile( path, oss.str() );

                communicator.send( msg );

                auto responseOk = communicator.recv();
            }


            void deleteFile( const std::string &path )
            {
                auto fullpath = "C:/moje/programowanie/c++/HomeDisc/clients/pc/test" + path; //todo

                std::experimental::filesystem::remove( fullpath );
            }

            void deleteRemoteFile( const std::string &path )
            {
                auto msg = SimpleRequests::remove( path );
                communicator.send( msg );

                auto responseOk = communicator.recv();
            }

            void handleDifferences( const Differences &differences )
            {
                ZmqMessagePtr msg;

                for( const auto &diff : differences )
                {
                    switch( diff.type )
                    {
                        case filesystem::DifferenceType::CHANGED_OTHER:
                        case filesystem::DifferenceType::NEW_OTHER:
                            downloadFile( diff.entryPath.string() );
                        break;

                        case filesystem::DifferenceType::DELETED_OTHER:
                            deleteFile( diff.entryPath.string() );  
                        default:

                        case filesystem::DifferenceType::DELETED_LOCALLY:
                            deleteRemoteFile( diff.entryPath.string() );
                        break;

                        case filesystem::DifferenceType::CHANGED_LOCALLY:
                        case filesystem::DifferenceType::NEW_LOCALLY:
                            uploadFile( diff.entryPath.string() );
                        break;
                    }
                }
            }

        public:
            ResponseHandler( Communicator &communicator ) :
                communicator( communicator )
            {}

            void handle( ZmqMessagePtr msg )
            {
                std::string strMsg( static_cast<const char*>( msg->data() ) );

                auto differences = getDifferences( strMsg );

                handleDifferences( differences );
            }

        private:
            Communicator &communicator;
        };
    }
}