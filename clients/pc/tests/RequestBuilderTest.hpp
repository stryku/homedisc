#pragma once

#include "../src/RequestBuilder.hpp"

#include <gtest/gtest.h>

#include <string>

TEST( RequestBuilderTests, EmptyMsgBuild )
{
    HD::Communication::RequestBuilder builder;

    EXPECT_THROW( builder.build(), HD::Communication::UndefMsgTypeRequestBuild );
}

TEST( RequestBuilderTests, UndefMsgTypeRequestBuild )
{
    HD::Communication::RequestBuilder builder;

    builder.setFile( "file" );
    EXPECT_THROW( builder.build(), HD::Communication::UndefMsgTypeRequestBuild );

    builder.setPath( "path" );
    EXPECT_THROW( builder.build(), HD::Communication::UndefMsgTypeRequestBuild );

    builder.setType( HD::Communication::MessageType::UNDEF );

    EXPECT_THROW( builder.build(), HD::Communication::UndefMsgTypeRequestBuild );
}

TEST( RequestBuilderTests, ValidTypesNoThrow )
{
    auto types = {
        HD::Communication::MessageType::FILE_REQ,
        HD::Communication::MessageType::LIST_REQ,
        HD::Communication::MessageType::NEW_DIR,
        HD::Communication::MessageType::NEW_FILE,
        HD::Communication::MessageType::REMOVE_DIR,
        HD::Communication::MessageType::REMOVE_FILE
    };

    HD::Communication::RequestBuilder builder;

    for( auto type : types )
    {
        builder.setType( type );

        EXPECT_NO_THROW( builder.build() );
    }
}

TEST( RequestBuilderTests, ValidTypesCorrectMsgContent )
{
    auto types = {
        HD::Communication::MessageType::FILE_REQ,
        HD::Communication::MessageType::LIST_REQ,
        HD::Communication::MessageType::NEW_DIR,
        HD::Communication::MessageType::NEW_FILE,
        HD::Communication::MessageType::REMOVE_DIR,
        HD::Communication::MessageType::REMOVE_FILE
    };
    std::string prefix{ "<request><type>" },
                sufix{ "</type></request>" };

    HD::Communication::RequestBuilder builder;

    for( auto type : types )
    {
        auto expectedRequestContent{ prefix
                                     + HD::Communication::messageTypeToString( type )
                                     + sufix
                                     + '\0' };

        builder.setType( type );

        auto buildedRequest = builder.build();

        auto expectedSize = expectedRequestContent.size();
        auto buildedSize = buildedRequest->size();

        EXPECT_EQ( expectedSize, buildedSize );

        auto cmpResult = std::memcmp( expectedRequestContent.data(),
                                      buildedRequest->data(),
                                      expectedSize );

        EXPECT_EQ( 0, cmpResult );
    }
}

TEST( RequestBuilderTests, TypeAndFileCorrectMsgContent )
{
    HD::Communication::RequestBuilder builder;

    auto type = HD::Communication::MessageType::NEW_FILE;
    std::string fileContent{ "file" };
    std::string prefix{ "<request><type>" };
    std::string sufix{ "</type><file>" + fileContent + "</file></request>" + '\0' };

    auto expectedRequestContent{ prefix 
                                 + HD::Communication::messageTypeToString( type ) 
                                 + sufix };

    builder.setType( type ).setFile( fileContent );

    auto buildedRequest = builder.build();

    auto expectedSize = expectedRequestContent.size();
    auto buildedSize = buildedRequest->size();

    EXPECT_EQ( expectedSize, buildedSize );

    auto cmpResult = std::memcmp( expectedRequestContent.data(),
                                  buildedRequest->data(),
                                  expectedSize );

    EXPECT_EQ( 0, cmpResult );
}

TEST( RequestBuilderTests, TypeAndPathCorrectMsgContent )
{
    HD::Communication::RequestBuilder builder;

    auto type = HD::Communication::MessageType::NEW_FILE;
    std::string pathContent{ "path" };
    std::string prefix{ "<request><type>" };
    std::string sufix{ "</type><path>" + pathContent + "</path></request>" + '\0' };

    auto expectedRequestContent{ prefix
                                 + HD::Communication::messageTypeToString( type )
                                 + sufix };

    builder.setType( type ).setPath( pathContent );

    auto buildedRequest = builder.build();

    auto expectedSize = expectedRequestContent.size();
    auto buildedSize = buildedRequest->size();

    EXPECT_EQ( expectedSize, buildedSize );

    auto cmpResult = std::memcmp( expectedRequestContent.data(),
                                  buildedRequest->data(),
                                  expectedSize );

    EXPECT_EQ( 0, cmpResult );
}

TEST( RequestBuilderTests, TypeFileAndPathCorrectMsgContent )
{
    HD::Communication::RequestBuilder builder;

    auto type = HD::Communication::MessageType::NEW_FILE;
    std::string fileContent{ "file" };
    std::string pathContent{ "path" };
    std::string expectedRequestContent{ "<request><type>"
                                        + HD::Communication::messageTypeToString( type )
                                        + "</type><path>"
                                        + pathContent
                                        + "</path><file>"
                                        + fileContent
                                        + "</file></request>" 
                                        + '\0' };

    builder.setType( type ).setPath( pathContent ).setFile( fileContent );

    auto buildedRequest = builder.build();

    auto expectedSize = expectedRequestContent.size();
    auto buildedSize = buildedRequest->size();

    EXPECT_EQ( expectedSize, buildedSize );

    auto cmpResult = std::memcmp( expectedRequestContent.data(),
                                  buildedRequest->data(),
                                  expectedSize );

    EXPECT_EQ( 0, cmpResult );
}