#pragma once

#include "SimpleRequests.hpp"
#include "log.h"

#include <zmq.hpp>

#include <memory>
#include <iostream>

namespace HD
{
    namespace Communication
    {
        class Communicator
        {
        public:
            typedef std::shared_ptr<zmq::message_t> ZmqMessagePtr;

            void connect( const std::string &endpoint )
            {
                socket.connect( endpoint.c_str() );
            }

            ZmqMessagePtr recv()
            {
                auto msg = std::make_shared<zmq::message_t>();

                socket.recv( msg.get() );
                socket.recv( msg.get() );

                LOG( "Received: " << std::string( static_cast<const char*>( msg->data() ), msg->size() ) );

                return msg;
            }

            void send( ZmqMessagePtr msg )
            {
                LOG( "Sending: " << std::string( static_cast<const char*>( msg->data() ), msg->size() ) );
                socket.send( *msg.get() );
            }

        private:
            zmq::context_t ctx{ 1 };
            zmq::socket_t socket{ ctx, ZMQ_DEALER };
        };
    }
}
