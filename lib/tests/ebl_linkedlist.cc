#include "doctest.h"
#include <iostream>
#include "ebl/linked_list.h"
#include "ebl/memory.h"
#include "ebl/util.h"

using list_type = ebl::IntrusiveList<int, 1>;
using list_node = list_type::node;
template<> struct ebl::LinkedRef<int> {
    typedef list_node type;
};

TEST_CASE("intrusive linked list test") {
    constexpr int data_size = 5;
    int data[data_size];
    for(int i = 0; i < data_size; i++)
        data[i] = i+1;
    
    list_type::list<0> list1;
    list_type::list<0> list2;

    SUBCASE("push_back / pop_back") {
        for(auto& v : data) list1.push_back(list_type::construct(v));
        int i = 0;
        for(auto& v : list1) {
            CHECK(v == data[i]);
            i++;
        }
        CHECK(list1.size() == data_size);
        while(!list1.empty()) {
            i--;
            auto v = list1.pop_back();
            CHECK(*v == data[i]);
        }
        CHECK(list1.size() == 0);
    }

    SUBCASE("push_front / pop_front") {
        for(auto& v : data) list1.push_front(list_type::construct(v));
        int i = data_size;
        for(auto& v : list1) {
            i--;
            CHECK(v == data[i]);
        }
        CHECK(list1.size() == data_size);
        i = data_size;
        while(!list1.empty()) {
            i--;
            auto v = list1.pop_front();
            CHECK(*v == data[i]);
        }
        CHECK(list1.size() == 0);
    }

    SUBCASE("move") {
        for(auto& v : data) list1.push_back(list_type::construct(v));
        while(!list1.empty()) {
            auto v = list1.pop_front();
            list2.push_back(ebl::move(v));
        }
        CHECK(list1.size() == 0);
        CHECK(list2.size() == data_size);
        int i = 0;
        for(auto& v : list2) {
            CHECK(v == data[i]);
            i++;
        }
        while(!list2.empty()) list2.pop_front();
    }
}
