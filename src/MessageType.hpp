#pragma once

#include <string>

namespace HD
{
    namespace Communication
    {
        enum class MessageType
        {
            LIST_REQ,
            FILE_REQ,
            NEW_FILE,
            NEW_DIR,
            REMOVE_FILE,
            REMOVE_DIR,

            UNDEF
        };

        std::string messageTypeToString( MessageType mt )
        {
            switch( mt )
            {
                case MessageType::LIST_REQ: return "LIST_REQ";
                case MessageType::FILE_REQ: return "FILE_REQ";
                case MessageType::NEW_FILE: return "NEW_FILE";
                case MessageType::NEW_DIR: return "NEW_DIR";
                case MessageType::REMOVE_FILE: return "REMOVE_FILE";
                case MessageType::REMOVE_DIR: return "REMOVE_DIR";

                default: return "UNDEF";
            }
        }

        MessageType stringToMessageType( const std::string &str )
        {
            if( str == "LIST_REQ" ) return MessageType::LIST_REQ;
            if( str == "FILE_REQ" ) return MessageType::FILE_REQ;
            if( str == "NEW_FILE" ) return MessageType::NEW_FILE;
            if( str == "NEW_DIR" ) return MessageType::NEW_DIR;
            if( str == "REMOVE_FILE" ) return MessageType::REMOVE_FILE;
            if( str == "REMOVE_DIR" ) return MessageType::REMOVE_DIR;
            
            return MessageType::UNDEF;
        }
    }
}