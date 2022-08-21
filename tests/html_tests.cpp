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

#include "html/document.h"
#include "html/element.h"
#include "html/html_exceptions.h"
#include "html/types.h"
#include <catch2/catch_test_macros.hpp>

using namespace Scrapp;
using namespace Scrapp::Html;

TEST_CASE("HtmlElement") {

    SECTION("HtmlElement::tag returns lowercase tag name") {
        std::string html = "<html><body id=\"42\"><div class=\"something "
                           "another-class\"</div></body></html>";
        auto lxb_html = (const lxb_char_t*)html.c_str();
        unique_lxb_html_document document{lxb_html_document_create()};
        lxb_html_document_parse(document.get(), lxb_html, html.size());
        auto body_el = lxb_dom_interface_element(document->body);
        HtmlElement element{body_el};
        REQUIRE(element.tag() == "body");
    }

    SECTION("::attributes returns all attributes of element ") {
        std::string s_html =
            "<html><div class=\"something another-class\">hello</div><html>";
        auto html = (const lxb_char_t*)s_html.c_str();
        size_t html_size = s_html.size();
        unique_lxb_html_document document{lxb_html_document_create()};
        lxb_html_document_parse(document.get(), html, html_size);

        unique_lxb_dom_collection collection{
            lxb_dom_collection_make(&document->dom_document, 16)};

        lxb_status_t status;
        status = lxb_dom_elements_by_tag_name(
            lxb_dom_interface_element(document->body), collection.get(),
            (const lxb_char_t*)"div", 3);
        auto el_p = lxb_dom_collection_element(collection.get(), 0);
        HtmlElement element{el_p};
        auto attrs = element.attributes();
        REQUIRE(attrs["class"] == "something another-class");
    }

    SECTION("::get_attribute returns associated attribute value") {
        std::string html_s = "<html><div id=\"42\">hey</div></html>";
        auto html = (const lxb_char_t*)html_s.c_str();
        size_t html_size = html_s.size();
        unique_lxb_html_document document{lxb_html_document_create()};
        lxb_html_document_parse(document.get(), html, html_size);
        unique_lxb_dom_collection collection{
            lxb_dom_collection_make(&document->dom_document, 16)};
        lxb_dom_elements_by_tag_name(
            lxb_dom_interface_element(
                lxb_html_document_body_element(document.get())),
            collection.get(), (const lxb_char_t*)"div", 3);
        auto el_p = lxb_dom_collection_element(collection.get(), 0);
        HtmlElement element{el_p};
        auto id_attr = element.get_attribute("id");
        REQUIRE(id_attr == "42");
    }

    SECTION(
        "::get_attribute throws attribute_error if given key does not exists") {
        std::string html_s = "<html><div id=\"42\">hey</div></html>";
        auto html = (const lxb_char_t*)html_s.c_str();
        size_t html_size = html_s.size();
        unique_lxb_html_document document{lxb_html_document_create()};
        lxb_html_document_parse(document.get(), html, html_size);
        unique_lxb_dom_collection collection{
            lxb_dom_collection_make(&document->dom_document, 16)};
        lxb_dom_elements_by_tag_name(
            lxb_dom_interface_element(
                lxb_html_document_body_element(document.get())),
            collection.get(), (const lxb_char_t*)"div", 3);
        auto el_p = lxb_dom_collection_element(collection.get(), 0);
        HtmlElement element{el_p};
        REQUIRE_THROWS_AS(
            element.get_attribute("idd"), Scrapp::Html::attribute_error);
    }

    SECTION("::text returns textContent of element") {
        std::string html_s = "<html><div id=\"42\">hey</div></html>";
        auto html = (const lxb_char_t*)html_s.c_str();
        size_t html_size = html_s.size();
        unique_lxb_html_document document{lxb_html_document_create()};
        lxb_html_document_parse(document.get(), html, html_size);
        unique_lxb_dom_collection collection{
            lxb_dom_collection_make(&document->dom_document, 16)};
        lxb_dom_elements_by_tag_name(
            lxb_dom_interface_element(
                lxb_html_document_body_element(document.get())),
            collection.get(), (const lxb_char_t*)"div", 3);
        auto el_p = lxb_dom_collection_element(collection.get(), 0);
        HtmlElement element{el_p};
        REQUIRE(element.text() == "hey");
    }

    SECTION("::css finds elements that matches selectors") {
        std::string html_s =
            R"(<html><div id="42">hey<div id="hello">hello</div></div></html>)";
        auto html = (const lxb_char_t*)html_s.c_str();
        size_t html_size = html_s.size();
        unique_lxb_html_document document{lxb_html_document_create()};
        lxb_html_document_parse(document.get(), html, html_size);
        unique_lxb_dom_collection collection{
            lxb_dom_collection_make(&document->dom_document, 16)};
        lxb_dom_elements_by_tag_name(
            lxb_dom_interface_element(
                lxb_html_document_body_element(document.get())),
            collection.get(), (const lxb_char_t*)"div", 3);
        auto el_p = lxb_dom_collection_element(collection.get(), 0);
        HtmlElement element{el_p};
        auto selected = element.css("#hello");
        REQUIRE(selected.size() == 1);
        REQUIRE(selected[0].text() == "hello");
    }
    SECTION("::css return empty list if no children found") {
        std::string html_s =
            R"(<html><div id="42">hey<div id="hello">hello</div></div></html>)";
        auto html = (const lxb_char_t*)html_s.c_str();
        size_t html_size = html_s.size();
        unique_lxb_html_document document{lxb_html_document_create()};
        lxb_html_document_parse(document.get(), html, html_size);
        unique_lxb_dom_collection collection{
            lxb_dom_collection_make(&document->dom_document, 16)};
        lxb_dom_elements_by_tag_name(
            lxb_dom_interface_element(
                lxb_html_document_body_element(document.get())),
            collection.get(), (const lxb_char_t*)"div", 3);
        auto el_p = lxb_dom_collection_element(collection.get(), 0);
        HtmlElement element{el_p};
        auto selected = element.css("#no-item-with-this-id");
        REQUIRE(selected.empty());
    }
    SECTION("::css return empty list if no child exists") {
        std::string html_s =
            R"(<html><div id="42">hey<div id="hello">hello</div></div></html>)";
        auto html = (const lxb_char_t*)html_s.c_str();
        size_t html_size = html_s.size();
        unique_lxb_html_document document{lxb_html_document_create()};
        lxb_html_document_parse(document.get(), html, html_size);
        unique_lxb_dom_collection collection{
            lxb_dom_collection_make(&document->dom_document, 16)};
        lxb_dom_elements_by_tag_name(
            lxb_dom_interface_element(
                lxb_html_document_body_element(document.get())),
            collection.get(), (const lxb_char_t*)"div", 3);
        auto el_p = lxb_dom_collection_element(collection.get(), 0);
        HtmlElement element{el_p};
        auto selected = element.css("#hello");
        REQUIRE(selected[0].text() == "hello");
        auto el = selected[0];
        auto found = el.css("div");
        REQUIRE(found.empty());
    }
}

TEST_CASE("HtmlDocument") {
    SECTION("::head returns <head> HtmlElement") {
        std::string html = "<html><head id=\"42\"></head></html>";
        HtmlDocument document{html};
        auto head = document.head();
        REQUIRE(head.tag() == "head");
        REQUIRE(head.get_attribute("id") == "42");
    }

    SECTION("::body returns <body> HtmlElement") {
        std::string html =
            "<html><head></head><body autocomplete id=\"42\"></body></html>";
        HtmlDocument document{html};
        auto body = document.body();
        REQUIRE(body.tag() == "body");
        REQUIRE(body.get_attribute("id") == "42");
    }
}