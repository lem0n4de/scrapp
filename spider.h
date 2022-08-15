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

#ifndef SCRAPP_SPIDER_H
#define SCRAPP_SPIDER_H

#include <string>
#include <vector>
#include <boost/signals2.hpp>
#include <boost/asio.hpp>
#include <deque>
#include "request.h"
#include "response.h"

namespace asio = boost::asio;
namespace signals = boost::signals2;

namespace Scrapp {
    class Spider {
    private:
        bool running_ = false;
        std::vector<Request> _requests;
        std::deque<Request> request_queue_;
        signals::signal<void(const Request&)> request_added_;
        void on_request_added_(const Request& request);
        signals::signal<void(const Request&, const Response&)> request_finished_;
        void on_request_finished_(const Request& request, const Response& response);
        std::size_t thread_count_;
        asio::thread_pool thread_pool_;
        asio::executor_work_guard <asio::thread_pool::executor_type> work_guard_;
    public:
        explicit Spider(std::size_t thread_count = 8) : thread_count_{thread_count}, thread_pool_{thread_count_},
                                                        work_guard_{asio::make_work_guard(thread_pool_)} {}
        virtual ~Spider() = default;

        void add_request(const std::string& url);
        void add_request(const Request& request); // TODO Maybe change const ref
        const std::deque<Request>& request_queue() const noexcept;

        virtual void parse(Scrapp::Response result) = 0;

        void start();
        void wait();
        void stop();

        bool running() const;
    };
}

#endif //SCRAPP_SPIDER_H
