#include "FilesystemEntryListProvider.hpp"

#include <iostream>

using namespace hd::filesystem;

int main()
{
    auto list = FilesystemEntryListProvider::getFilesystemEntryList( "C:/moje/programowanie/c++/HomeDisc/servers/pc/test" );

    std::cout << list;

    std::cout << list.toXml();

    return 0;
}