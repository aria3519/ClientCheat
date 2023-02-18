//
// request_handler.cpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "request_handler.h"
#include <fstream>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include "mime_types.h"
#include "reply.h"
#include "request.h"


namespace http {
    namespace server3 {

        request_handler::request_handler(const std::string& doc_root)
            : doc_root_(doc_root)
        {
        }

        void request_handler::handle_request(const request& req, reply& rep)
        {
            // Decode url to path.
            std::string request_path;
            if (!url_decode(req.uri, request_path))
            {
                rep = reply::stock_reply(reply::bad_request);
                return;
            }

            // Request path must be absolute and not contain "..".
            if (request_path.empty() || request_path[0] != '/'
                || request_path.find("..") != std::string::npos)
            {
                rep = reply::stock_reply(reply::bad_request);
                return;
            }

            // adress.html 를 넣어줘야함 
            // google.html 를 넣어줘야함 

            int Random = rand() % 2;
            // sIf path ends in slash (i.e. is a directory) then add "index.html".
            if (request_path[request_path.size() - 1] == '/')
            {
                switch (Random)
                {
                case 0:
                    request_path += "adress.html";
                    break;
                case 1:
                    request_path += "google.html";
                    break;
                default:
                    request_path += "adress.html";
                    break;
                }

            }



            // Determine the file extension.
            std::size_t last_slash_pos = request_path.find_last_of("/");
            std::size_t last_dot_pos = request_path.find_last_of(".");
            std::string extension;
            if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
            {
                extension = request_path.substr(last_dot_pos + 1);
            }

            // Open the file to send back.
            std::string full_path = doc_root_ + request_path;
            std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
            if (!is)
            {
                rep = reply::stock_reply(reply::not_found);
                return;
            }

            // Fill out the reply to be sent to the client.
            rep.status = reply::ok;
            char buf[512];
            while (is.read(buf, sizeof(buf)).gcount() > 0)
            {
                // 구글
                //src = "/images/branding/googlelogo/2x/googlelogo_color_272x92dp.png" 
                // 네이버
                // content = "https://s.pstatic.net/static/www/mobile/edit/2016/0705/mobile_212852414260.png"

               // L"구글";
                
                for (int i = 0;i < 512;i++)
                {
                     // 네 -> 구 
                    if(buf[i] == -21 && count == 0)
                        count++;
                    else if (buf[i] == -124 && count == 1)
                        count++;
                    else if (buf[i] == -92 && count == 2)
                        count++;
                    // 이 -> " "
                    else if (buf[i] == -20 && count == 3)
                        count++;
                    else if (buf[i] == -99 && count == 4)
                        count++;
                    else if (buf[i] == -76 && count == 5)
                        count++;
                    // 버 -> 글 
                    else if (buf[i] == -21 && count == 6)
                    {
                        count++;
                    }
                    else if (buf[i] == -78 && count == 7)
                        count++;
                    else if (buf[i] == -124 && count == 8)
                    {
                        buf[i - 8] = 'g';
                        buf[i - 7] = 'o';
                        buf[i - 6] = 'o';
                        buf[i - 5] = 'l';
                        buf[i - 4] = 'e';
                        buf[i - 3] = 'n';
                        buf[i - 2] = 'o';
                        buf[i - 1] = 'w';
                        buf[i] = 'a';
                        count = 0;
                    }
                    else
                    {
                        count = 0;
                    }



                   /* if (buf[i] == 'N' && count==0)
                    {
                        count++;
                    }
                    else if (buf[i] == 'A' && count == 1)
                    {
                        count++;
                    }
                    else if (buf[i] == 'V' && count == 2)
                    {
                        count++;
                    }
                    else if (buf[i] == 'E' && count == 3)
                    {
                        count++;
                    }
                    else if (buf[i] == 'R' && count == 4)
                    {
                        buf[i - 4] = 'g';
                        buf[i - 3] = 'o';
                        buf[i - 2] = 'g';
                        buf[i - 1] = 'l';
                        buf[i] = 'e';

                        count = 0;
                    }
                    else
                    {
                        count = 0;
                    }*/
                }

                
                
                rep.content.append(buf, is.gcount());
            }
            rep.headers.resize(2);
            rep.headers[0].name = "Content-Length";
            rep.headers[0].value = boost::lexical_cast<std::string>(rep.content.size());
            rep.headers[1].name = "Content-Type";
            rep.headers[1].value = mime_types::extension_to_type(extension);
        }

        bool request_handler::url_decode(const std::string& in, std::string& out)
        {
            out.clear();
            out.reserve(in.size());
            for (std::size_t i = 0; i < in.size(); ++i)
            {
                if (in[i] == '%')
                {
                    if (i + 3 <= in.size())
                    {
                        int value = 0;
                        std::istringstream is(in.substr(i + 1, 2));
                        if (is >> std::hex >> value)
                        {
                            out += static_cast<char>(value);
                            i += 2;
                        }
                        else
                        {
                            return false;
                        }
                    }
                    else
                    {
                        return false;
                    }
                }
                else if (in[i] == '+')
                {
                    out += ' ';
                }
                else
                {
                    out += in[i];
                }
            }
            return true;
        }

    } // namespace server3
} // namespace http