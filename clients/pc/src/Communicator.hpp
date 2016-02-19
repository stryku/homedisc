#pragma once

#include "SimpleRequests.hpp"

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

                std::cout<<"Received: "<<static_cast<const char*>( msg->data() )<<"\n";

                return msg;
            }

            void send( ZmqMessagePtr msg )
            {
                std::cout<<"Sending: "<<static_cast<const char*>( msg->data() )<<"\n";
                socket.send( *msg.get() );
            }

        private:
            zmq::context_t ctx{ 1 };
            zmq::socket_t socket{ ctx, ZMQ_DEALER };
        };
    }
}
