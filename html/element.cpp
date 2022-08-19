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

#include "element.h"
#include "html_exceptions.h"

namespace Scrapp::Html {

    HtmlElement::HtmlElement(lxb_dom_element_t* dom_el) : element_p{dom_el} {
        lxb_dom_attr_t* attr = this->element_p->first_attr;
        while (attr != nullptr) {
            size_t name_len, value_len;
            auto name = lxb_dom_attr_local_name(attr, &name_len);
            auto value = lxb_dom_attr_value(attr, &value_len);
            auto s_name = std::string(reinterpret_cast<const char*>(name));
            auto s_value = std::string(reinterpret_cast<const char*>(value));
            this->attributes_[s_name] = s_value;
            attr = lxb_dom_element_next_attribute(attr);
        }

        size_t len;
        auto text_content =
            lxb_dom_node_text_content(&this->element_p->node, &len);
        this->text_ = std::string(reinterpret_cast<const char*>(text_content));
    }

    std::string HtmlElement::tag() const noexcept {
        size_t len;
        auto lxb_tag = lxb_dom_element_tag_name(this->element_p, &len);
        auto tag = std::string(reinterpret_cast<const char*>(lxb_tag));
        return Scrapp::to_lower(tag);
    }

    const Attributes HtmlElement::attributes() const noexcept {
        return this->attributes_;
    }

    std::string HtmlElement::get_attribute(const std::string& attr) const {
        auto it = this->attributes_.find(attr);
        if (it == this->attributes_.end()) {
            throw attribute_error("element does not have attribute: " + attr);
        }
        return (*it).second;
    }

    std::string HtmlElement::text() const noexcept { return this->text_; }

} // namespace Scrapp::Html
