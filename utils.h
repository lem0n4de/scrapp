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

#ifndef SCRAPP_UTILS_H
#define SCRAPP_UTILS_H

#include <memory>
#include <string>

namespace Scrapp {
    // Copied from: https://stackoverflow.com/a/51274008/9105459
    template<auto fn> struct deleter_from_fn {
        template<class T> constexpr void operator()(T* pointer) const {
            fn(pointer);
        }
    };
    template<class Type, auto DeleterFunction>
    using unique_ptr_with_deleter =
        std::unique_ptr<Type, deleter_from_fn<DeleterFunction>>;

    std::string url_encode(const std::string& value);
    std::string url_decode(std::string text);
    char from_hex(char ch);
} // namespace Scrapp

#endif // SCRAPP_UTILS_H
