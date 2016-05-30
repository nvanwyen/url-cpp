#
# Makefile
# ~~~~~~~~~~~~~~~~~~~
#
# Copyright ( c ) 2006-2016 Metasystems Technologies Inc. (www.mtihq.com)
#                           Nicholas Van Wyen
#
# Distributed under the Boost Software License, Version 1.0. ( See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt )
#

all: test
debug: testd

test: test.cpp url.hpp url.cpp
	@g++ -o test test.cpp url.cpp

testd: test.cpp url.hpp url.cpp
	@g++ -g -o testd test.cpp url.cpp

clean:
	@rm -f test *.o
