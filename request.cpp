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

#include "request.h"
#include "utils.h"

namespace Scrapp {
    Headers::Headers(
        const std::map<std::string, std::string, cpr::CaseInsensitiveCompare>&
            map) {
        for (const auto& [key, value] : map) {
            this->insert_or_assign(key, value);
        }
    }

    Request::Request() = default;

    Request::Request(Url url) : _url{std::move(url)}, _render{false} {}

    Request::Request(Url url, RequestParameters params)
        : _url{std::move(url)}, _parameters{std::move(params)}, _render{false} {
    }

    Request::Request(Url url, Headers headers)
        : _url{std::move(url)}, _headers{std::move(headers)}, _render{false} {}

    void Request::add_parameter(
        const std::pair<std::string, std::string>& param) noexcept {
        this->_parameters[param.first] = param.second;
    }

    void Request::add_header(
        const std::pair<std::string, std::string>& header) noexcept {
        this->_headers[header.first] = header.second;
    }

    Headers Request::headers() const noexcept { return this->_headers; }

    void Request::set_render(bool render) noexcept { this->_render = render; }

    std::string Request::full_url() const noexcept {
        std::string total = this->_url.str();
        total = total + "?";
        for (const auto& [key, value] : this->_parameters) {
            total += Scrapp::url_encode(key) + "=" + Scrapp::url_encode(value);
        }
        return total;
    }

    std::string Request::url() const noexcept { return this->_url.str(); }

    bool Request::operator==(const Request& other) const noexcept {
        return this->_url == other._url &&
               this->_parameters == other._parameters &&
               this->_headers == other._headers &&
               this->_render == other._render;
    }
} // namespace Scrapp
