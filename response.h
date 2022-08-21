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

#ifndef SCRAPP_RESPONSE_H
#define SCRAPP_RESPONSE_H

#include "exceptions.h"
#include "html/document.h"
#include "request.h"
#include <boost/algorithm/string.hpp>
#include <boost/json.hpp>
#include <cpr/cpr.h>

namespace Scrapp {
    class Response {
      public:
        explicit Response(cpr::Response& res);

        explicit Response();
        ~Response();

        long status_code{};
        std::string text{};
        Headers headers{};
        Url url{};
        double elapsed{};
        cpr::Cookies cookies{};
        cpr::Error error{};
        std::string raw_header{};
        std::string status_line{};
        std::string reason{};
        cpr::cpr_off_t uploaded_bytes{};
        cpr::cpr_off_t downloaded_bytes{};
        long redirect_count{};

        boost::json::value json();

        template<bool check_content_type = true>
        Html::HtmlDocument html() const {
            if constexpr (check_content_type) {
                auto content_type = this->headers.at("Content-Type");
                if (!boost::algorithm::contains(content_type, "text/html") ||
                    !boost::algorithm::contains(
                        content_type, "multipart/related") ||
                    !boost::algorithm::contains(
                        content_type, "application/xhtml+xml")) {
                    throw invalid_content_type_exception(
                        "response to " + this->url.str() +
                        " does not have a valid HTML content-type. "
                        "Content-Type: " +
                        content_type);
                }
                return Html::HtmlDocument{this->text};
            } else {
                return Html::HtmlDocument{this->text};
            }
        }

      private:
    };
} // namespace Scrapp

#endif // SCRAPP_RESPONSE_H
