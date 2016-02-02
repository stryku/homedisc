#pragma once

#include "../SimpleRequests.hpp"

#include <gtest/gtest.h>

TEST( SimpleRequestsTests, fileList )
{
    std::string expectedRequestContent{ "<request><type>LIST_REQ</type></request>" };
    expectedRequestContent += '\0';

    auto request = HD::Communication::SimpleRequests::fileList();

    auto expectedSize = expectedRequestContent.size();
    auto buildedSize = request->size();

    EXPECT_EQ( expectedSize, buildedSize );

    auto cmpResult = std::memcmp( expectedRequestContent.data(),
                                  request->data(),
                                  expectedSize );

    EXPECT_EQ( 0, cmpResult );
}

TEST( SimpleRequestsTests, file )
{
    std::string fileRelativePath{ "relativePath" };
    std::string expectedRequestContent{ "<request><type>FILE_REQ</type><path>"
                                        + fileRelativePath
                                        + "</path></request>"
                                        + '\0' };

    auto request = HD::Communication::SimpleRequests::file( fileRelativePath );

    auto expectedSize = expectedRequestContent.size();
    auto buildedSize = request->size();

    EXPECT_EQ( expectedSize, buildedSize );

    auto cmpResult = std::memcmp( expectedRequestContent.data(),
                                  request->data(),
                                  expectedSize );

    EXPECT_EQ( 0, cmpResult );
}

TEST( SimpleRequestsTests, remove )
{
    std::string fileRelativePath{ "relativePath" };
    std::string expectedRequestContent{ "<request><type>REMOVE_FILE</type><path>"
                                        + fileRelativePath
                                        + "</path></request>"
                                        + '\0' };

    auto request = HD::Communication::SimpleRequests::remove( fileRelativePath );

    auto expectedSize = expectedRequestContent.size();
    auto buildedSize = request->size();

    EXPECT_EQ( expectedSize, buildedSize );

    auto cmpResult = std::memcmp( expectedRequestContent.data(),
                                  request->data(),
                                  expectedSize );

    EXPECT_EQ( 0, cmpResult );
}

TEST( SimpleRequestsTests, removeDirectory )
{
    std::string fileRelativePath{ "relativePath" };
    std::string expectedRequestContent{ "<request><type>REMOVE_DIR</type><path>"
                                        + fileRelativePath
                                        + "</path></request>"
                                        + '\0' };

    auto request = HD::Communication::SimpleRequests::removeDirectory( fileRelativePath );

    auto expectedSize = expectedRequestContent.size();
    auto buildedSize = request->size();

    EXPECT_EQ( expectedSize, buildedSize );

    auto cmpResult = std::memcmp( expectedRequestContent.data(),
                                  request->data(),
                                  expectedSize );

    EXPECT_EQ( 0, cmpResult );
}

TEST( SimpleRequestsTests, newFile )
{
    std::string fileRelativePath{ "relativePath" };
    std::string fileContent{ "file content" };
    std::string expectedRequestContent{ "<request><type>NEW_FILE</type><path>"
                                        + fileRelativePath
                                        + "</path><file>"
                                        + fileContent
                                        + "</file></request>"
                                        + '\0' };

    auto request = HD::Communication::SimpleRequests::newFile( fileRelativePath, fileContent );

    auto expectedSize = expectedRequestContent.size();
    auto buildedSize = request->size();

    EXPECT_EQ( expectedSize, buildedSize );

    auto cmpResult = std::memcmp( expectedRequestContent.data(),
                                  request->data(),
                                  expectedSize );

    EXPECT_EQ( 0, cmpResult );
}

TEST( SimpleRequestsTests, newDirectory )
{
    std::string fileRelativePath{ "relativePath" };
    std::string expectedRequestContent{ "<request><type>NEW_DIR</type><path>"
                                        + fileRelativePath
                                        + "</path></request>"
                                        + '\0' };

    auto request = HD::Communication::SimpleRequests::newDirectory( fileRelativePath );

    auto expectedSize = expectedRequestContent.size();
    auto buildedSize = request->size();

    EXPECT_EQ( expectedSize, buildedSize );

    auto cmpResult = std::memcmp( expectedRequestContent.data(),
                                  request->data(),
                                  expectedSize );

    EXPECT_EQ( 0, cmpResult );
}