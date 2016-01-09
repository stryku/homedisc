#pragma once

#include <router/log.h>

#include <zmq.hpp>

#include <functional>

namespace hd
{
    namespace communication
    {
        class RequestHandler
        {
        public:
            auto getHandleFunction()
            {
                return std::bind( &RequestHandler::handle, this, std::placeholders::_1 );
            }

            zmq::message_t handle( const zmq::message_t &msg )
            {
                LOG( "handluje" );

                return zmq::message_t();
            }

        };
    }
}