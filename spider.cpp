
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

#include "spider.h"
#include <cpr/cpr.h>
#include <future>

void Scrapp::Spider::start() {
    if (!this->running()) {
        this->running_ = true;
        this->request_added_.connect([this](const Request& request) {
            this->on_request_added_(request);
        });
        this->request_finished_.connect(
            [this](const Request& request, const Response& response) {
                this->on_request_finished_(request, response);
            });
    }
    for (; !this->request_queue_.empty(); this->request_queue_.pop_front()) {
        auto front = this->request_queue_.front();
        this->request_added_(front);
    }
}

void Scrapp::Spider::add_request(const std::string& url) {
    auto req = Scrapp::Request(Scrapp::Url(url));
    this->add_request(req);
}

void Scrapp::Spider::add_request(const Scrapp::Request& request) {
    this->request_queue_.push_back(request);
    if (this->running()) {
        this->request_added_(request);
    }
}

const std::deque<Scrapp::Request>&
Scrapp::Spider::request_queue() const noexcept {
    return this->request_queue_;
}

void Scrapp::Spider::on_request_added_(const Scrapp::Request& request) {
    asio::post(this->thread_pool_, [this, request]() {
        auto params = cpr::Parameters{};
        for (const auto& [key, value] : request.parameters()) {
            params.Add({key, value});
        }
        auto headers = cpr::Header{};
        for (const auto& [key, value] : request.headers()) {
            headers[key] = value;
        }
        auto c_res = cpr::Get(cpr::Url{request.url()}, params, headers);
        this->request_finished_(request, Scrapp::Response(c_res));
    });
}

void Scrapp::Spider::on_request_finished_(
    const Scrapp::Request& request, const Scrapp::Response& response) {
    asio::post(this->thread_pool_, [this, request, response]() {
        this->parse(response);
    });
}

void Scrapp::Spider::wait() {
    this->work_guard_.reset();
    this->thread_pool_.join();
    this->running_ = false;
}

void Scrapp::Spider::stop() {
    this->thread_pool_.stop();
    this->running_ = false;
}

bool Scrapp::Spider::running() const { return this->running_; }
