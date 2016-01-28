#include "Client.hpp"

#include <zmq.hpp>
#include <string>
#include <iostream>

INITIALIZE_EASYLOGGINGPP

int main()
{
    HD::Communication::Client client( "tcp://localhost:5570" );

    client.run();
    
    //std::string str = "<msg><type>FILE_REQ</type></msg>";

    //zmq::message_t msg( const_cast<char*>( str.c_str() ), str.size() + 1, nullptr );

    //zmq::context_t ctx( 1 );
    //zmq::socket_t socket( ctx, ZMQ_DEALER );

    //socket.connect( "tcp://localhost:5570" );

    //socket.send( msg );
    ////socket.recv( &msg );
    //char b[10240];


    //zmq::pollitem_t items[] = { socket, 0, ZMQ_POLLIN, 0 };

    //while( true )
    //{
    //    //int r = rand( ) % 100;
    //    //for( int i = 0; i < 100; ++i )
    //    //{
    //    // 10 milliseconds
    //    zmq::poll( items, 1 );
    //    if( items[0].revents & ZMQ_POLLIN )
    //    {
    //        zmq::message_t m;
    //        socket.recv( &m );
    //        //s_dump( client_socket_ );
    //        //int more = 0;           //  Multipart detection
    //        //size_t more_size = sizeof ( more );
    //        //client_socket_.getsockopt( ZMQ_RCVMORE, &more, &more_size );
    //        std::cout << static_cast<char*>( m.data() );
    //    }
    //    //}

    //    //std::cout << "Client " << identity << " sending request\n";
    //    //char request_string[16] = {};
    //    //sprintf( request_string, "request #%d", ++request_nbr );
    //    //client_socket_.send( request_string, strlen( request_string ) );
    //}
   

    return 0;
}