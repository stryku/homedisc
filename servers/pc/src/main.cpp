#include <router/Server.hpp>
#include "RequestHandler.hpp"

#include <iostream>

using namespace HD::Filesystem;


INITIALIZE_EASYLOGGINGPP

int main()
{
    Server server; 
    HD::Communication::RequestHandler reqHandler;

    server.run( reqHandler.getHandleFunction() );

    //auto list = FilesystemEntryListProvider::getFilesystemEntryList( "C:/moje/programowanie/c++/HomeDisc/servers/pc/test" );
    ////auto list = FilesystemEntryListProvider::getFilesystemEntryList( "C:/moje/lib/Boost/boost_1_59_0" );


    //std::cout << list;

    //list.copyToOld();

    //auto list2 = FilesystemEntryListProvider::getFilesystemEntryList( "C:/moje/programowanie/c++/HomeDisc/servers/pc/testnew" );

    //std::cout << list;

    //auto dif = list.getDifferences( list2 );



    //std::cout << "\n---------------------\n\n\n";

    //for( auto a : dif )
    //{
    //    std::cout << a << "\n\n";
    //}





    //auto xml = list.toXml();

    //FilesystemEntryList l2;

    //l2.fromXml( xml );

    //std::cout <<  "\n\n"<< l2;

    return 0;
}