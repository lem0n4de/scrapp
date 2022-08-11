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

#include <catch2/catch_all.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <catch2/trompeloeil.hpp>
#include <spider.h>
#include "request.h"

class MySpider : public Scrapp::Spider {
public:
    void parse(const std::string& result) override {
    };
};


class MockSpider : public trompeloeil::mock_interface<MySpider> {
    IMPLEMENT_MOCK1 (parse);
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
        REQUIRE(req.url() == encoded_url);
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
        REQUIRE(req.url() == "https://example.org?some%20key=some%20value");
    }
}

TEST_CASE("Spider class")
{
    auto mock_spider = MockSpider();
    auto valid_html = trompeloeil::re(R"((?:\<!doctype html\>)*\s*<\/?[a-z][\s\S]*>)");
    SECTION("has start urls")
    {
        std::string url = "https://example.org";
        mock_spider.add_start_url(url);
        REQUIRE_THAT(mock_spider.start_urls(), Catch::Matchers::VectorContains(url));
    }

    SECTION("calls parse once for each url after started")
    {
        std::string url = "https://example.org";
        for (int i = 0; i < 3; i++) {
            mock_spider.add_start_url(url);
        }
        REQUIRE_CALL(mock_spider, parse(valid_html)).TIMES(3);
        mock_spider.start();
    }

    SECTION("parse gets valid html")
    {
        std::string url = "https://example.org";
        mock_spider.add_start_url(url);
        ALLOW_CALL(mock_spider, parse(valid_html));
        mock_spider.start();
    }

}