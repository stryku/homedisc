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