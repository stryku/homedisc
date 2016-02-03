#pragma once

#include "../FilesystemEntry.hpp"

#include <gtest/gtest.h>

TEST( FilesystemEntryTests, stringType )
{
    HD::Filesystem::FilesystemEntry entry;

    EXPECT_EQ( "UNDEF", entry.stringType() );

    entry.type = HD::Filesystem::FilesystemEntryType::DIRECTORY;
    EXPECT_EQ( "DIRECTORY", entry.stringType() );

    entry.type = HD::Filesystem::FilesystemEntryType::FILE;
    EXPECT_EQ( "FILE", entry.stringType() );
}

TEST( FilesystemEntryTests, compateForSet_true )
{
    HD::Filesystem::FilesystemEntry left, right;

    left.type = HD::Filesystem::FilesystemEntryType::DIRECTORY;
    left.path = "/folder/folder/folder";

    right.type = HD::Filesystem::FilesystemEntryType::FILE;
    right.path = "/folder/file.txt";
    EXPECT_EQ( true, HD::Filesystem::FilesystemEntry::compareForSet( left, right ) );


    left.type = HD::Filesystem::FilesystemEntryType::FILE;
    left.path = "/folder/fil.txt";
    EXPECT_EQ( true, HD::Filesystem::FilesystemEntry::compareForSet( left, right ) );


    left.type = HD::Filesystem::FilesystemEntryType::DIRECTORY;
    left.path = "/folder/folder";

    right.type = HD::Filesystem::FilesystemEntryType::DIRECTORY;
    right.path = "/folder/folder/folder";
    EXPECT_EQ( true, HD::Filesystem::FilesystemEntry::compareForSet( left, right ) );
}

TEST( FilesystemEntryTests, compateForSet_false )
{
    HD::Filesystem::FilesystemEntry left, right;

    left.type = HD::Filesystem::FilesystemEntryType::FILE;
    left.path = "/file.txt";

    right.type = HD::Filesystem::FilesystemEntryType::DIRECTORY;
    right.path = "/folder";
    EXPECT_EQ( false, HD::Filesystem::FilesystemEntry::compareForSet( left, right ) );

    left.type = HD::Filesystem::FilesystemEntryType::DIRECTORY;
    left.path = "/longFolderName";
    EXPECT_EQ( false, HD::Filesystem::FilesystemEntry::compareForSet( left, right ) );
}
