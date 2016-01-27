#pragma once

#include "SimpleRequests.hpp"

#include <zmq.hpp>

#include <memory>

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
                socket.connect( endpoint );
            }

            ZmqMessagePtr recv()
            {
                auto msg = std::make_shared<zmq::message_t>();

                socket.recv( msg.get() );

                return msg;
            }

            void send( ZmqMessagePtr msg )
            {
                socket.send( *msg.get() );
            }

        private:
            zmq::context_t ctx{ 1 };
            zmq::socket_t socket{ ctx, ZMQ_DEALER };
        };
    }
}