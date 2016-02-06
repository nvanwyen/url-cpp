//
// url.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright ( c ) 2006-2016 Metasystems Technologies Inc. (www.mtihq.com)
//                           Nicholas Van Wyen
//
// Distributed under the Boost Software License, Version 1.0. ( See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt )
//

#ifndef HTTP_SERVER_URL_HPP
#define HTTP_SERVER_URL_HPP

#include <iostream>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

namespace http {
namespace server {

//
class url
{
public:
    //
    typedef std::string node;

    //
    struct param
    {
        std::string name;
        std::string value;

        //
        param() {}
        param( std::string n, std::string v )
            : name( n ), value( v ) {}

        //
        inline param& operator=( const param& p )
        {
            name  = p.name;
            value = p.value;
            return *this;
        }

        //
        inline bool operator==( std::string n ) const
        {
            return name == n;
        }

        //
        inline bool operator==( const param& p ) const
        {
            return ( ( name == p.name ) && ( value == p.value ) );
        }
    };

    // Construct a new authorization handler
    url() {}
    url( std::string uri ) : uri_( uri ) { parse(); }

    //
    bool parse();
    bool parse( std::string uri );

    //
    inline const std::string& uri() const { return uri_; }
    std::string get(); /// buikd a URI from provided data

    //
    inline const std::string& scheme() const { return scheme_; }
    inline const std::string& user()   const { return user_; }
    inline const std::string& passwd() const { return passwd_; }
    inline const std::string& host()   const { return host_; }
    inline const std::string& port()   const { return port_; }
    inline const std::string& path()   const { return path_; }

    inline const std::vector<node>&  resources() const { return resources_; }
    inline const std::vector<param>& queries()   const { return queries_; }
    inline const std::vector<param>& fragments() const { return fragment_; }

    inline void scheme( std::string value ) { scheme_ = value; }
    inline void user( std::string value )   { user_ = value; }
    inline void passwd( std::string value ) { passwd_ = value; }
    inline void host( std::string value )   { host_ = value; }
    inline void port( std::string value )   { port_ = value; }
    inline void path( std::string value )   { path_ = value; }

    inline void resource( std::string value )
    { resources_.push_back( value ); }

    inline void query( std::string name, std::string value )
    { queries_.push_back( param( name, value ) ); }

    inline void fragment( std::string name, std::string value )
    { fragment_.push_back( param( name, value ) ); }

    //
    void decode( std::string& val );
    void encode( std::string& val );

private:
    //
    std::string uri_;

    std::string scheme_;
    std::string user_;
    std::string passwd_;
    std::string host_;
    std::string port_;
    std::string path_;

    std::vector<node>  resources_;
    std::vector<param> queries_;
    std::vector<param> fragment_;

    //
    void parse_path( std::string& res, std::vector<node>& vec );
    void parse_param( std::string& par, std::vector<param>& vec );
    void clear();
};

typedef boost::shared_ptr<url> url_ptr;

} // namespace server
} // namespace http

//
std::ostream& operator<<( std::ostream& os, const http::server::url& url );

#endif // HTTP_SERVER_URL_HPP
