#pragma once

#include "../FilesystemEntryList.hpp"

#include <gtest/gtest.h>

TEST( FilesystemEntryList, fromXml_emptyXmlList_noThrow )
{
    HD::Filesystem::FilesystemEntryList list;
    std::string emptyXmlList{ "<fel></fel>" };

    EXPECT_NO_THROW( list.fromXml( emptyXmlList ) );
}

TEST( FilesystemEntryList, fromXml_notEmptyXmlList_noThrow )
{
    HD::Filesystem::FilesystemEntryList list;
    std::string xmlList{ "<fel><md5>md5</md5><path>path</path><moddate>moddate</moddate><type>DIRECTORY</type></fel>" };

    EXPECT_NO_THROW( list.fromXml( xmlList ) );
}

TEST( FilesystemEntryList, fromXml_completeEntryValidType_noThrow )
{
    HD::Filesystem::FilesystemEntryList list;
    std::string xmlListDir{ "<fel><md5>md5</md5><path>path</path><moddate>moddate</moddate><type>DIRECTORY</type></fel>" };
    std::string xmlListFile{ "<fel><md5>md5</md5><path>path</path><moddate>moddate</moddate><type>FILE</type></fel>" };

    EXPECT_NO_THROW( list.fromXml( xmlListDir ) );
    EXPECT_NO_THROW( list.fromXml( xmlListFile ) );
}

TEST( FilesystemEntryList, fromXml_completeEntryNotValidType_throwUndefEntryType )
{
    HD::Filesystem::FilesystemEntryList list;
    std::string xmlListEmptyType{ "<fel><md5>md5</md5><path>path</path><moddate>moddate</moddate><type></type></fel>" };
    std::string xmlListRubbishType{ "<fel><md5>md5</md5><path>path</path><moddate>moddate</moddate><type>asads</type></fel>" };

    EXPECT_THROW( list.fromXml( xmlListEmptyType ), HD::Filesystem::UndefEntryType );
    EXPECT_THROW( list.fromXml( xmlListRubbishType ), HD::Filesystem::UndefEntryType );
}

