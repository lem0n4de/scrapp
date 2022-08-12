
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

#include "spider.h"
#include <future>
#include <cpr/cpr.h>

void Scrapp::Spider::start() {
    std::vector<std::future<cpr::Response>> futures;
    for (const auto& req: this->_requests) {
        auto params = cpr::Parameters{};
        for (const auto& [key, value]: req.parameters()) {
            params.Add({key, value});
        }
        auto headers = cpr::Header{};
        for (const auto& [key, value]: req.headers()) {
            headers[key] = value;
        }
        futures.emplace_back(cpr::GetAsync(cpr::Url(req.url()), params, headers));
    }

    for (auto& future: futures) {
        auto res = future.get();
        this->parse(static_cast<Scrapp::Response>(res));
    }
}
void Scrapp::Spider::add_request(const std::string& url) {
    this->_requests.emplace_back(Url(url));
}
void Scrapp::Spider::add_request(const Scrapp::Request& request) {
    this->_requests.push_back(request);

}
const std::vector<Scrapp::Request>& Scrapp::Spider::requests() {
    return this->_requests;
}
