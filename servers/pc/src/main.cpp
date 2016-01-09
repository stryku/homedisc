#include "FilesystemEntryListProvider.hpp"

#include <iostream>

using namespace hd::filesystem;

int main()
{
    auto list = FilesystemEntryListProvider::getFilesystemEntryList( "C:/moje/programowanie/c++/HomeDisc/servers/pc/test" );
    //auto list = FilesystemEntryListProvider::getFilesystemEntryList( "C:/moje/lib/Boost/boost_1_59_0" );


    std::cout << list;

    auto xml = list.toXml();

    FilesystemEntryList l2;

    l2.fromXml( xml );

    std::cout <<  "\n\n"<< l2;

    return 0;
}