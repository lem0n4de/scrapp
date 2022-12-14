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

#include "exceptions.h"
#include "request.h"
#include "response.h"
#include <catch2/catch_all.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <catch2/trompeloeil.hpp>
#include <iostream>
#include <spider.h>

class MockSpider : public trompeloeil::mock_interface<Scrapp::Spider> {
    IMPLEMENT_MOCK1(parse);
};

TEST_CASE("Request") {
    auto url = "https://example.org";
    SECTION("has url-only constructor") {
        auto req = Scrapp::Request(Scrapp::Url("https://example.org"));
    }

    SECTION("accepts parameters as map and url encodes them") {
        auto encoded_url = "https://example.org?some%20key=some%20value";
        auto params = Scrapp::RequestParameters{{"some key", "some value"}};
        auto req = Scrapp::Request(Scrapp::Url(url), params);
        REQUIRE(req.full_url() == encoded_url);
    }

    SECTION("accepts headers as map") {
        auto headers = Scrapp::Headers{{"key", "value"}};
        auto req = Scrapp::Request(Scrapp::Url(url), headers);
        REQUIRE(req.headers().at("key") == "value");
    }

    SECTION("headers and parameters can be added later on") {
        auto req = Scrapp::Request(Scrapp::Url(url));
        req.add_header({"key", "value"});
        REQUIRE(req.headers().at("key") == "value");

        req.add_parameter({"some key", "some value"});
        REQUIRE(req.parameters().at("some key") == "some value");
        REQUIRE(
            req.full_url() == "https://example.org?some%20key=some%20value");
    }
}

TEST_CASE("Response") {
    auto json_response = Scrapp::Response{};
    json_response.text =
        R"JSON({"key1": "value", "key2": ["value2", true, false], "key3": 42})JSON";
    json_response.headers["Content-Type"] = "application/json";
    auto html_response = Scrapp::Response{};
    html_response.text =
        R"HTML(<div><a src="example.com" /><div id="42">thisisdiv42</div></div>)HTML";
    html_response.headers["Content-Type"] = "text/html";

    SECTION("::html() throws invalid_content_type_exception if "
            "check-content-type enabled and response does not have valid html "
            "content-type") {
        REQUIRE_THROWS_AS(
            json_response.html<true>(), Scrapp::invalid_content_type_exception);
    }

    SECTION("::html<false>() does not throw even with invalid text") {
        REQUIRE_NOTHROW(json_response.html<false>());
    }

    SECTION("::html<true>() is the default") {
        REQUIRE_THROWS_AS(
            json_response.html(), Scrapp::invalid_content_type_exception);
    }
}

TEST_CASE("Spider class") {
    auto mock_spider = MockSpider();
    SECTION("requests added before start() are added to request_queue") {
        std::string url = "https://example.org";
        mock_spider.add_request(url);
        REQUIRE_THAT(
            mock_spider.request_queue(),
            Catch::Matchers::Contains(Scrapp::Request(Scrapp::Url(url))));
    }

    SECTION("parse gets proper response") {
        std::string url = "https://example.org";
        mock_spider.add_request(url);
        ALLOW_CALL(mock_spider, parse(ANY(Scrapp::Response)));
        mock_spider.start();
        mock_spider.wait();
    }

    SECTION("calls parse once for each url after started") {
        std::string url = "https://example.org";
        for (int i = 0; i < 3; i++) {
            mock_spider.add_request(url);
        }
        REQUIRE_CALL(mock_spider, parse(ANY(Scrapp::Response))).TIMES(3);
        mock_spider.start();
        mock_spider.wait();
    }

    SECTION("response.json() throws Scrapp::invalid_json_exception on false "
            "content-type") {
        std::string url = "https://example.org";
        mock_spider.add_request(url);
        Scrapp::Response res;
        REQUIRE_CALL(mock_spider, parse(trompeloeil::_))
            .LR_SIDE_EFFECT(res = _1);

        mock_spider.start();
        mock_spider.wait();
        REQUIRE_THROWS_AS(res.json(), Scrapp::invalid_json_exception);
    }

    SECTION("response.json() return boost::json::value on application/json "
            "responses") {
        std::string url = "https://www.httpbin.org/get";
        mock_spider.add_request(url);
        Scrapp::Response res;
        REQUIRE_CALL(mock_spider, parse(trompeloeil::_))
            .LR_SIDE_EFFECT(res = _1);
        mock_spider.start();
        mock_spider.wait();
        REQUIRE_NOTHROW(res.json());
        auto js = res.json();
        REQUIRE(js.at("url").get_string() == url);
    }

    SECTION("Spider sends parameters correctly") {
        std::string url = "https://www.httpbin.org/get";
        Scrapp::Request req{Scrapp::Url(url)};
        req.add_parameter({"key", "value"});
        mock_spider.add_request(req);

        Scrapp::Response res;
        REQUIRE_CALL(mock_spider, parse(trompeloeil::_))
            .LR_SIDE_EFFECT(res = _1);
        mock_spider.start();
        mock_spider.wait();
        REQUIRE_NOTHROW(res.json());
        auto js = res.json();
        REQUIRE(js.at("args").at("key").get_string() == "value");
    }

    SECTION("Spider sends headers correctly") {
        std::string url = "https://www.httpbin.org/get";
        Scrapp::Request req{Scrapp::Url(url)};
        req.add_header({"key", "value"});
        mock_spider.add_request(req);

        Scrapp::Response res;
        REQUIRE_CALL(mock_spider, parse(trompeloeil::_))
            .LR_SIDE_EFFECT(res = _1);
        mock_spider.start();
        mock_spider.wait();

        REQUIRE_NOTHROW(res.json());
        auto js = res.json();
        REQUIRE(js.at("headers").at("Key") == "value");
    }

    SECTION("Requests added after start are sent correctly") {
        std::string url = "https://www.httpbin.org/get";
        int count = 5;
        for (int i = 0; i < count; i++) {
            mock_spider.add_request(url);
        }
        std::vector<Scrapp::Response> responses;
        ALLOW_CALL(mock_spider, parse(trompeloeil::_))
            .LR_SIDE_EFFECT(responses.push_back(_1));
        mock_spider.start();

        for (int i = 0; i < count; i++) {
            mock_spider.add_request(url);
        }
        mock_spider.wait();
        REQUIRE(responses.size() == count * 2);
    }
}