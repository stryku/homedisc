#pragma once

#include <zmq.hpp>

namespace hd
{
    namespace communication
    {
        class Client
        {
        public:
            Client( const std::string &endpoint ) :
                endpoint( endpoint ),
                ctx( 1 ),
                socket( ctx, ZMQ_DEALER )
            {}

            void run()
            {
                socket.connect( endpoint );
            }

        private:
            std::string endpoint;
            zmq::context_t ctx;
            zmq::socket_t socket;
        };
    }
}