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
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef SCRAPP_REQUEST_H
#define SCRAPP_REQUEST_H

#include <cpr/cpr.h>

#include <utility>
#include "utils.h"


namespace Scrapp {
    using Url = cpr::Url;

    class RequestParameters : public std::unordered_map<std::string, std::string> {
        using unordered_map::unordered_map;
    };

    class Headers : public std::unordered_map<std::string, std::string> {
        using unordered_map::unordered_map;
    };

    class Request {
    public:
        Request() = default;
        explicit Request(Url url) : _url(std::move(url)) {};
        Request(Url url, RequestParameters params) : _url(std::move(url)), _parameters(std::move(params)) {}
        Request(Url url, Headers headers) : _url(std::move(url)), _headers(std::move(headers)) {}

        void add_parameter(const std::pair<std::string, std::string>& param) noexcept {
            this->_parameters[param.first] = param.second;
        }

        RequestParameters parameters() {
            return this->_parameters;
        }

        void add_header(const std::pair<std::string, std::string>& header) noexcept {
            this->_headers[header.first] = header.second;
        }

        Headers headers() {
            return this->_headers;
        }

        std::string url() {
            std::string total = this->_url.str();
            total = total + "?";
            for (const auto& [key, value]: this->_parameters) {
                total += Scrapp::url_encode(key) + "=" + Scrapp::url_encode(value);
            }
            return total;
        }
    private:
        Url _url;
        RequestParameters _parameters;
        Headers _headers;
    };
}

#endif //SCRAPP_REQUEST_H
