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

#include <lexbor/css/css.h>
#include <lexbor/html/html.h>
#include <lexbor/selectors/selectors.h>
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

    HtmlElement HtmlDocument::body() const noexcept {
        auto body = lxb_html_document_body_element(this->document_.get());
        return HtmlElement{lxb_dom_interface_element(body)};
    }

    static lxb_status_t callback(
        lxb_dom_node_t* node, lxb_css_selector_specificity_t* spec, void* ctx) {
        auto* pointers = static_cast<std::vector<HtmlElement>*>(ctx);
        pointers->push_back(HtmlElement{lxb_dom_interface_element(node)});
        return LXB_STATUS_OK;
    }

    std::vector<HtmlElement>
    HtmlDocument::css(const std::string& s) const noexcept {
        lxb_status_t status;
        auto selectors_string = (const lxb_char_t*)s.c_str();
        unique_lxb_css_parser parser{lxb_css_parser_create()};
        status = lxb_css_parser_init(parser.get(), nullptr, nullptr);
        if (status != LXB_STATUS_OK) {
            return {};
        }
        unique_lxb_selectors selectors{lxb_selectors_create()};
        status = lxb_selectors_init(selectors.get());
        if (status != LXB_STATUS_OK) {
            return {};
        }

        unique_lxb_css_selector_list selector_list{lxb_css_selectors_parse(
            parser.get(), selectors_string, s.length())};
        if (parser->status != LXB_STATUS_OK) {
            return {};
        }
        std::vector<HtmlElement> found;
        lxb_selectors_find(
            selectors.get(), &this->document_->dom_document.node,
            selector_list.get(), callback, &found);
        return found;
    }
} // namespace Scrapp::Html