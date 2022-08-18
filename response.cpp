
// MIT License
//
// Copyright (c) 2022 Yunus Emre ÖRCÜN
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "response.h"
#include "exceptions.h"
#include <boost/algorithm/string.hpp>
#include <boost/json/string_view.hpp>
#include <iostream>

namespace Scrapp {
    Response::Response(cpr::Response& res)
        : text{res.text}, headers{res.header}, url{res.url},
          elapsed{res.elapsed}, cookies{res.cookies}, error{res.error},
          raw_header{res.raw_header}, status_line{res.status_line},
          reason{res.reason}, uploaded_bytes{res.uploaded_bytes},
          downloaded_bytes{res.downloaded_bytes},
          redirect_count{res.redirect_count}, status_code{res.status_code} {}

    Response::Response() = default;
    Response::~Response() = default;

    boost::json::value Response::json() {
        // covers both application/json and application/ld+json
        if (!boost::algorithm::contains(
                this->headers.at("Content-Type"), "json")) {
            throw invalid_json_exception(
                "response to " + this->url.str() +
                " is not a valid json: " + this->text);
        }
        boost::json::parse_options opts;
        opts.allow_comments = true;
        opts.allow_trailing_commas = true;
        boost::json::error_code ec;

        unsigned char buf[4096];
        boost::json::static_resource res(buf);
        boost::json::string_view sw = this->text;
        auto j = boost::json::parse(sw, ec, &res, opts);
        return j;
    }
}; // namespace Scrapp
