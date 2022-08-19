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

#ifndef SCRAPP_TYPES_H
#define SCRAPP_TYPES_H

#include "../utils.h"
#include <lexbor/html/html.h>
#include <unordered_map>

namespace Scrapp::Html {
    using Attribute = std::pair<std::string, std::string>;
    using Attributes = std::unordered_map<std::string, std::string>;
    using unique_lxb_html_document =
        unique_ptr_with_deleter<lxb_html_document_t, lxb_html_document_destroy>;
    using unique_lxb_dom_collection = unique_ptr_with_deleter<
        lxb_dom_collection_t, lxb_dom_collection_destroy>;
} // namespace Scrapp::Html

#endif // SCRAPP_TYPES_H
