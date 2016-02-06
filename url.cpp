//
// url.cpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright ( c ) 2006-2016 Metasystems Technologies Inc. (www.mtihq.com)
//                           Nicholas Van Wyen
//
// Distributed under the Boost Software License, Version 1.0. ( See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt )
//

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>
#include "url.hpp"

namespace http {
namespace server {

namespace encoding_ref {

struct code
{
    const char* character;
    const char* seqeunce;

} codes[] = {
                { " ",  "%20" },
                { "!",  "%21" },
                { "\"", "%22" },
                { "#",  "%23" },
                { "$",  "%24" },
                { "%",  "%25" },
                { "&",  "%26" },
                { "'",  "%27" },
                { "(",  "%28" },
                { ")",  "%29" },
                { "*",  "%2A" },
                { "+",  "%2B" },
                { ",",  "%2C" },
                { "-",  "%2D" },
                { ".",  "%2E" },
                { "/",  "%2F" },
                { "0",  "%30" },
                { "1",  "%31" },
                { "2",  "%32" },
                { "3",  "%33" },
                { "4",  "%34" },
                { "5",  "%35" },
                { "6",  "%36" },
                { "7",  "%37" },
                { "8",  "%38" },
                { "9",  "%39" },
                { ":",  "%3A" },
                { ";",  "%3B" },
                { "<",  "%3C" },
                { "=",  "%3D" },
                { ">",  "%3E" },
                { "?",  "%3F" },
                { "@",  "%40" },
                { "A",  "%41" },
                { "B",  "%42" },
                { "C",  "%43" },
                { "D",  "%44" },
                { "E",  "%45" },
                { "F",  "%46" },
                { "G",  "%47" },
                { "H",  "%48" },
                { "I",  "%49" },
                { "J",  "%4A" },
                { "K",  "%4B" },
                { "L",  "%4C" },
                { "M",  "%4D" },
                { "N",  "%4E" },
                { "O",  "%4F" },
                { "P",  "%50" },
                { "Q",  "%51" },
                { "R",  "%52" },
                { "S",  "%53" },
                { "T",  "%54" },
                { "U",  "%55" },
                { "V",  "%56" },
                { "W",  "%57" },
                { "X",  "%58" },
                { "Y",  "%59" },
                { "Z",  "%5A" },
                { "[",  "%5B" },
                { "\\", "%5C" },
                { "]",  "%5D" },
                { "^",  "%5E" },
                { "`",  "%60" },
                { "a",  "%61" },
                { "b",  "%62" },
                { "c",  "%63" },
                { "d",  "%64" },
                { "e",  "%65" },
                { "f",  "%66" },
                { "g",  "%67" },
                { "h",  "%68" },
                { "i",  "%69" },
                { "j",  "%6A" },
                { "k",  "%6B" },
                { "l",  "%6C" },
                { "m",  "%6D" },
                { "n",  "%6E" },
                { "o",  "%6F" },
                { "p",  "%70" },
                { "q",  "%71" },
                { "r",  "%72" },
                { "s",  "%73" },
                { "t",  "%74" },
                { "u",  "%75" },
                { "v",  "%76" },
                { "w",  "%77" },
                { "x",  "%78" },
                { "y",  "%79" },
                { "z",  "%7A" },
                { "{",  "%7B" },
                { "|",  "%7C" },
                { "}",  "%7D" },
                { "~",  "%7E" },
                { 0, 0 }        // mark end of list.
            };
}

// syntax:
//  scheme://username:password@subdomain.domain.tld:port/path/file-name.suffix?query#fragment
std::string url::get()
{
    std::string uri;

    if ( user().length() )
    {
        uri = user();

        if ( passwd().length() > 0 )
            uri += ":" + passwd();
    }

    if ( host().length() > 0 )
    {
        if ( uri.length() > 0 )
            uri += "@" + uri;
        else
            uri = host();

        if ( port().length() > 0 )
            uri += ":" + port();
    }
    else
    {
        if ( uri.length() > 0 )
            uri += "@127.0.0.1";

        if ( port().length() > 0 )
            uri += ":" + port();
    }

    if ( uri.length() > 0 )
    {
        if ( scheme().length() > 0 )
            uri = scheme() + "://" + uri;
        else
            uri = "http://" + uri;
    }

    if ( resources().size() > 0 )
    {
        for ( std::vector<url::node>::const_iterator i = resources().begin(); i != resources().end(); ++i )
            uri += "/" + (*i);
    }
    else
        uri += "/";

    if ( queries().size() > 0 )
    {
         uri += "?";

        for ( std::vector<url::param>::const_iterator i = queries().begin(); i != queries().end(); ++i )
        {
            uri += (*i).name;

            if ( (*i).value.length() > 0 )
                uri += "=" + (*i).value;
        } 
    }

    if ( fragments().size() > 0 )
    {
         uri += "#";

        for ( std::vector<url::param>::const_iterator i = fragments().begin(); i != fragments().end(); ++i )
        {
            uri += (*i).name;

            if ( (*i).value.length() > 0 )
                uri += "=" + (*i).value;
        } 
    }

    return uri;
}

//
bool url::parse( std::string uri )
{
    if ( uri.length() > 0 )
    {
        uri_ = uri;
        return parse();
    }
    else
        return false;
}

//
bool url::parse()
{
    if ( uri_.length() > 0 )
    {
        size_t nxt = 0;
        size_t lst = 0;

        clear();

        if ( uri_ != "/" )
        {
            //
            if ( ( nxt = uri_.find( "://" ) ) != std::string::npos )
            {
                scheme_ = uri_.substr( lst, nxt );
                lst += nxt + 3;
            }
            else
                lst = 0;

            //
            if ( ( nxt = uri_.find_first_of( "/", lst ) ) != std::string::npos )
            {
                host_ = uri_.substr( lst, nxt - lst );
                path_ = uri_.substr( nxt + 1 );
            }
            else
                host_ = uri_.substr( lst );

            if ( host_.find_first_of( "@:" ) != std::string::npos )
            {
                size_t p = 0;

                if ( ( p = host_.find( "@" ) ) != std::string::npos )
                {
                    user_ = host_.substr( 0, host_.find( "@" ) );

                    if ( user_.find( ":" ) != std::string::npos )
                    {
                        passwd_ = user_.substr( user_.find( ":" ) + 1 );
                        user_ = user_.substr( 0, user_.find( ":" ) );
                    }

                    host_ = host_.substr( p + 1 );
                }

                if ( ( p = host_.find( ":" ) ) != std::string::npos )
                {
                    port_ = host_.substr( p + 1 );
                    host_ = host_.substr( 0, p );

                    if ( port_.find_first_of( "/?#" ) != std::string::npos )
                    {
                        path_ = port_.substr( port_.find_first_of( "/?#" ) + 1 );
                        port_ = port_.substr( 0, port_.find_first_of( "/?#" ) );
                    }
                }
            }

            if ( path_.length() > 0 )
            {
                size_t p = 0;

                if ( ( p = path_.find_first_of( "?#" ) ) != std::string::npos )
                {
                    std::string par;

                    if ( p == 0 )
                    {
                        par = path_;
                        path_.clear();
                    }
                    else
                    {
                        par = path_.substr( p );
                        path_ = path_.substr( 0, p );
                    }

                    if ( par.find( "?" ) != std::string::npos )
                    {
                        std::string q = par.substr( par.find( "?" ) + 1 );

                        if ( q.find( "#" ) != std::string::npos )
                        {
                            if ( q.find( "#" ) > 0 )
                                q = q.substr( 0, q.find( "#" ) );
                            else
                                q = q.substr( q.find( "#" ) + 1 );
                        }

                        parse_param( q, queries_ );
                    }

                    if ( par.find( "#" ) != std::string::npos )
                    {
                        std::string f = par.substr( par.find( "#" ) + 1 );

                        if ( f.find( "?" ) != std::string::npos )
                        {
                            if ( f.find( "?" ) > 0 )
                                f = f.substr( 0, f.find( "?" ) );
                            else
                                f = f.substr( f.find( "?" ) + 1 );
                        }

                        parse_param( f, fragment_ );
                    }
                }

                parse_path( path_, resources_ );
                path_ = "/" + path_;
            }

            decode( user_ );
            decode( passwd_ );
            decode( host_ );
            decode( port_ );
            decode( path_ );
        }
        else
            path_ = uri_;

        return ( ( scheme_.length()  > 0 )
              || ( user_.length()    > 0 )
              || ( passwd_.length()  > 0 )
              || ( host_.length()    > 0 )
              || ( port_.length()    > 0 )
              || ( path_.length()    > 0 )
              || ( resources_.size() > 0 )
              || ( queries_.size()   > 0 )
              || ( fragment_.size()  > 0 ) );
    }
    else
        return false;
}

//
void url::parse_path( std::string& res, std::vector<url::node>& vec )
{
    if ( res.length() > 0 )
    {
        boost::split( vec, res, boost::is_any_of( "/" ), boost::token_compress_on );

        for ( std::vector<url::node>::iterator i = vec.begin(); i != vec.end(); ++i )
            decode( *i );
    }
}

//
void url::parse_param( std::string& par, std::vector<url::param>& vec )
{
    if ( par.length() > 0 )
    {
        std::vector<std::string> ary;
        boost::split( ary, par, boost::is_any_of( "&" ), boost::token_compress_on );

        if ( ary.size() > 0 )
        {
            for ( std::vector<std::string>::iterator i = ary.begin(); i != ary.end(); ++i )
            {
                if ( (*i).length() > 0 )
                {
                    url::param p;

                    if ( (*i).find( "=" ) != std::string::npos )
                    {
                        p.name = (*i).substr( 0, (*i).find( "=" ) );
                        p.value = (*i).substr( (*i).find( "=" ) + 1 );
                    }
                    else
                        p.name = (*i);

                    if ( p.name.length() > 0 )
                    {
                        decode( p.name );
                        decode( p.value );

                        vec.push_back( p );
                    }
                }
            }
        }
    }
}

//
void url::clear()
{
    scheme_.clear();
    user_.clear();
    passwd_.clear();
    host_.clear();
    port_.clear();
    path_.clear();
    resources_.clear();
    queries_.clear();
    fragment_.clear();
}

//
void url::decode( std::string& val )
{
    for ( encoding_ref::code* code = encoding_ref::codes; code->character; ++code )
        boost::replace_all( val, code->seqeunce, code->character );
}

//
void url::encode( std::string& val )
{
}

} // namespace server
} // namespace http

//
std::ostream& operator<<( std::ostream& os, const http::server::url& url )
{
    using namespace http::server;

    os << "uri:       " << url.uri()    << std::endl;
    os << "schema:    " << url.scheme() << std::endl;
    os << "user:      " << url.user()   << std::endl;
    os << "passwd:    " << url.passwd() << std::endl;
    os << "host:      " << url.host()   << std::endl;
    os << "port:      " << url.port()   << std::endl;
    os << "path:      " << url.path()   << std::endl;

    if ( url.resources().size() > 0 )
    {
        for ( std::vector<url::node>::const_iterator i = url.resources().begin(); i != url.resources().end(); ++i )
            os << "resource:  " << (*i) << std::endl;
    }
    else
        os << "resources: {empty}" << std::endl;

    if ( url.queries().size() > 0 )
    {
        for ( std::vector<url::param>::const_iterator i = url.queries().begin(); i != url.queries().end(); ++i )
            os << "query:     " << (*i).name << " = " << (*i).value << std::endl;
    }
    else
        os << "queries:   {empty}" << std::endl;

    if ( url.fragments().size() > 0 )
    {
        for ( std::vector<url::param>::const_iterator i = url.fragments().begin(); i != url.fragments().end(); ++i )
            os << "fragment:  " << (*i).name << " = " << (*i).value << std::endl;
    }
    else
        os << "fragments: {empty}" << std::endl;

    return os;
}
