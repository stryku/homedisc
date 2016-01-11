#pragma once

#include "SimpleRequests.hpp"

#include <zmq.hpp>

#include <memory>

namespace hd
{
    namespace communication
    {
        class Communicator
        {
        public:
            typedef std::shared_ptr<zmq::message_t> ZmqMessagePtr;

            void connect( const std::string &newEndpoint )
            {
                socket.connect( newEndpoint );
                endpoint = newEndpoint;
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
            std::string endpoint;
            zmq::context_t ctx{ 1 };
            zmq::socket_t socket{ ctx, ZMQ_DEALER };
        };
    }
}