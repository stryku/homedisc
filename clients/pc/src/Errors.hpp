#pragma once

#include <log.h>

#include <string>

namespace HD
{
    namespace Error
    {
        enum class ErrorType
        {
            BROKEN_LIST_FROM_SERVER,

            UNDEF
        };

        std::string errorTypeToString( ErrorType type ) 
        {
            switch( type )
            {
                case ErrorType::BROKEN_LIST_FROM_SERVER: return "BROKEN_LIST_FROM_SERVER";
                default:
                break;
            }
        }

        ErrorType stringToErrorType( const std::string &s )
        {
            if( s == "BROKEN_LIST_FROM_SERVER" ) return ErrorType::BROKEN_LIST_FROM_SERVER;

            return ErrorType::UNDEF;
        }

        void reportError( ErrorType type, const std::string &info )
        {
            LOG( "[ ERROR ] Type: " << errorTypeToString( type ) << ", Info: " << info );
        }
    }
}
