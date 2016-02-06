//
// test.cpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright ( c ) 2006-2016 Metasystems Technologies Inc. (www.mtihq.com)
//                           Nicholas Van Wyen
//
// Distributed under the Boost Software License, Version 1.0. ( See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt )
//

#include <iostream>
#include "url.hpp"

int main( int argc, char** argv )
{
    http::server::url url;

    if ( argc > 1 )
    {
        for ( int i = 1; i < argc; ++i )
        {
            if ( url.parse( argv[ i ] ) )
            {
                std::cout << "--- parsed --- " << std::endl;
                std::cout << url << std::endl;

                // derived the url, based on the parsed data (matches inout)
                std::cout << std::endl;
                std::cout << "--- derived ---" << std::endl;
                std::cout << url.get() << std::endl;
            }
            else
                std::cout << "Parsing failed for: " << argv[ i ] << std::endl;
        }
    }
    else
        std::cout << "Usage: " << argv[ 0 ] << "url url ..." << std::endl;

    return 0;
}

// g++ -o test test.cpp url.cpp && tail -n 12 test.cpp

// fully qualifed
//
// ./test "http://foo.com"
// ./test "http://foo.com/path.html"
// ./test "http://foo.com/very/long/path.html"
// ./test "http://foo.com/very/long/path.html?p1=v1"
// ./test "http://foo.com/very/long/path.html?p1=v1&p2=v2"
// ./test "http://foo.com/very/long/path.html#f1=x1"
// ./test "http://foo.com/very/long/path.html#f1=x1&f2=x2"
// ./test "http://jdoe:secret@foo.com/very/long/path.html?p1=v1&p2=v2#more-details"
// ./test "https://jdoe:secret@foo.com:8888/very/long/path.html?p1=v1&p2=v2#more-details=a:b"

// relative
//
// ./test "/"
// ./test "/path.html"
// ./test "/very/long/path.html"
// ./test "/very/long/path.html?p1=v1"
// ./test "/very/long/path.html?p1=v1&p2=v2"
// ./test "/very/long/path.html#f1=x1"
// ./test "/very/long/path.html#f1=x1&f2=x2"
