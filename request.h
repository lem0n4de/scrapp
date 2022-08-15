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

#ifndef SCRAPP_REQUEST_H
#define SCRAPP_REQUEST_H

#include <cpr/cpr.h>

#include "utils.h"
#include <utility>

namespace Scrapp {
    using Url = cpr::Url;

    class RequestParameters
        : public std::unordered_map<std::string, std::string> {
        using unordered_map::unordered_map;
    };

    class Headers : public std::unordered_map<std::string, std::string> {
        using unordered_map::unordered_map;

      public:
        explicit Headers(
            const std::map<
                std::string, std::string, cpr::CaseInsensitiveCompare>& map) {
            for (const auto& [key, value] : map) {
                this->insert_or_assign(key, value);
            }
        }
    };

    class Request {
      public:
        Request() = default;
        explicit Request(Url url) : _url(std::move(url)){};

        Request(Url url, RequestParameters params)
            : _url(std::move(url)), _parameters(std::move(params)) {}

        Request(Url url, Headers headers)
            : _url(std::move(url)), _headers(std::move(headers)) {}

        void add_parameter(
            const std::pair<std::string, std::string>& param) noexcept {
            this->_parameters[param.first] = param.second;
        }

        RequestParameters parameters() const noexcept {
            return this->_parameters;
        }

        void
        add_header(const std::pair<std::string, std::string>& header) noexcept {
            this->_headers[header.first] = header.second;
        }

        Headers headers() const noexcept { return this->_headers; }

        void set_render(bool render) noexcept { this->_render = render; }

        std::string full_url() const noexcept {
            std::string total = this->_url.str();
            total = total + "?";
            for (const auto& [key, value] : this->_parameters) {
                total +=
                    Scrapp::url_encode(key) + "=" + Scrapp::url_encode(value);
            }
            return total;
        }

        std::string url() const noexcept { return this->_url.str(); }

        bool operator==(const Request& other) const noexcept {
            return this->_url == other._url &&
                   this->_parameters == other._parameters &&
                   this->_headers == other._headers &&
                   this->_render == other._render;
        }

      private:
        Url _url;
        RequestParameters _parameters;
        Headers _headers;
        bool _render = false;
    };
} // namespace Scrapp

#endif // SCRAPP_REQUEST_H
