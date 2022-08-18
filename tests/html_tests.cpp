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

#include "html/element.h"
#include "html/types.h"
#include <catch2/catch_test_macros.hpp>

const lxb_char_t* to_lxb_char(const std::string& str) {
    return reinterpret_cast<const lxb_char_t*>(str.c_str());
}

using namespace Scrapp;
using namespace Scrapp::Html;

TEST_CASE("HtmlElement") {

    SECTION("HtmlElement::tag returns lowercase tag name") {
        std::string html =
            "<html><div class=\"something another-class\"</div></html>";
        const lxb_char_t* lxb_html = to_lxb_char(html);
        unique_lxb_html_document document{lxb_html_document_create()};
        lxb_html_document_parse(
            document.get(), to_lxb_char(html), sizeof(lxb_html) - 1);
        auto body_el = lxb_dom_interface_element(document->body);
        HtmlElement element{body_el};
        REQUIRE(element.tag() == "body");
    }
}