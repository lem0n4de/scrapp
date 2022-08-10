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
#include "utils.h"


namespace Scrapp {
    using Url = cpr::Url;

    class RequestParameter {
    public:
        RequestParameter(const std::string& key, const std::string& value) : _key(Scrapp::url_encode(key)),
                                                                             _value(Scrapp::url_encode(value)) {};
        std::string key() { return this->_key; }
        std::string value() { return this->_value; }
    private:
        const std::string _key;
        const std::string _value;
    };

    class Request {
    public:
        Request() = default;
        explicit Request(Url url) : _url(std::move(url)) {};
        template<typename ParameterContainer>
        Request(Url url, ParameterContainer params) : _url(url) {
            for (auto& param: params) {
                this->_parameters.push_back(param);
            }
        }
        std::string url() {
            std::string total = this->_url.str();
            total = total + "?";
            for (auto& param: this->_parameters) {
                total += param.key() + "=" + param.value();
            }
            return total;
        }
    private:
        Url _url;
        std::vector<RequestParameter> _parameters;
    };
}

#endif //SCRAPP_REQUEST_H
