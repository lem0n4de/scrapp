
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


void Scrapp::Spider::add_start_url(const std::string& url) {
    this->_start_urls.push_back(url);
}

const std::vector<std::string>& Scrapp::Spider::start_urls() {
    return this->_start_urls;
}

void Scrapp::Spider::start() {
    std::vector<std::future<cpr::Response>> futures;
    for (const auto& url: this->_start_urls) {
        futures.emplace_back(cpr::GetAsync(cpr::Url{url}));
    }

    for (auto& future: futures) {
        auto res = future.get();
        this->parse(res.text);
    }
}
