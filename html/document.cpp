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

#include "document.h"

#include <utility>

namespace Scrapp::Html {
    HtmlDocument::HtmlDocument(std::string html)
        : html_{std::move(html)}, document_{lxb_html_document_create()} {
        auto lxb_html_ =
            reinterpret_cast<const lxb_char_t*>(this->html_.c_str());
        lxb_html_document_parse(
            this->document_.get(), lxb_html_, html_.length());
    }

    HtmlElement HtmlDocument::head() const noexcept {
        auto head = lxb_html_document_head_element(this->document_.get());
        return HtmlElement{lxb_dom_interface_element(head)};
    }
} // namespace Scrapp::Html