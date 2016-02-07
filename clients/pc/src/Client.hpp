#pragma once

#include "SimpleRequests.hpp"
#include "Communicator.hpp"
#include "ResponseHandler.hpp"

#include <zmq.hpp>

#include <chrono>
#include <thread>

namespace HD
{
    namespace Communication
    {
        class Client
        {
        public:
            Client( const std::string &endpoint ) :
                endpoint( endpoint )
            {}

            void run()
            {
                static const size_t delayBeetwenRequests = 10000;
                communicator.connect( endpoint );

                while( 1 )
                {
                    auto listRequest = SimpleRequests::fileList();
                    communicator.send( listRequest );

                    auto response = communicator.recv();

                    responseHandler.handle( response );

                    std::this_thread::sleep_for( std::chrono::milliseconds( delayBeetwenRequests ) );
                }
            }

        private:


            std::string endpoint;
            Communicator communicator;
            ResponseHandler responseHandler{ communicator };
        };
    }
}
