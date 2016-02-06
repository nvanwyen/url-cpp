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
