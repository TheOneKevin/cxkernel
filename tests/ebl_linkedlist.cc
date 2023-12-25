// TODO: These tests are incomplete and need to be finished.

#include <iostream>

#include "doctest.h"
#include "ebl/linked_list.h"
#include "ebl/memory.h"
#include "ebl/util.h"

struct TestStruct : ebl::RefCountable<TestStruct>, ebl::IntrusiveListNode<TestStruct> {
   int data;
   TestStruct(int data) : data(data) {}
   operator int&() { return data; }
};

TEST_CASE("intrusive linked list") {
   constexpr int data_size = 5;
   int data[data_size];
   for(int i = 0; i < data_size; i++) data[i] = i + 1;

   ebl::IntrusiveList<TestStruct> list1;
   ebl::IntrusiveList<TestStruct> list2;

   SUBCASE("push_back / pop_back") {
      for(auto& v : data) list1.push_back(new TestStruct(v));
      int i = 0;
      for(auto v : list1) {
         CHECK(*v == data[i]);
         i++;
      }
      CHECK(list1.size() == data_size);
      while(!list1.empty()) {
         i--;
         auto v = list1.pop_back();
         CHECK(*v == data[i]);
         delete v;
      }
      CHECK(list1.size() == 0);
   }

   SUBCASE("push_front / pop_front") {
      for(auto& v : data) list1.push_front(new TestStruct(v));
      int i = data_size;
      for(auto v : list1) {
         i--;
         CHECK(*v == data[i]);
      }
      CHECK(list1.size() == data_size);
      i = data_size;
      while(!list1.empty()) {
         i--;
         auto v = list1.pop_front();
         CHECK(*v == data[i]);
         delete v;
      }
      CHECK(list1.size() == 0);
   }

   SUBCASE("move") {
      for(auto& v : data) list1.push_back(new TestStruct(v));
      while(!list1.empty()) {
         auto v = list1.pop_front();
         list2.push_back(ebl::move(v));
      }
      CHECK(list1.size() == 0);
      CHECK(list2.size() == data_size);
      int i = 0;
      for(auto v : list2) {
         CHECK(*v == data[i]);
         i++;
      }
      while(!list2.empty()) delete list2.pop_front();
   }
}

TEST_CASE("intrusive linked list with refptr") {
   constexpr int data_size = 100;
   int data[data_size];
   for(int i = 0; i < data_size; i++) data[i] = i + 1;

   ebl::IntrusiveList<ebl::RefPtr<TestStruct>> list1;
   ebl::IntrusiveList<ebl::RefPtr<TestStruct>> list2;

   SUBCASE("push_back / pop_back") {
      for(auto& v : data) list1.push_back(ebl::AdoptRef(new TestStruct(v)));
      int i = 0;
      for(auto v : list1) {
         CHECK(*v == data[i]);
         CHECK(v->ref_count() == 1);
         i++;
      }
      CHECK(list1.size() == data_size);
      while(!list1.empty()) {
         i--;
         auto v = list1.pop_back();
         CHECK(*v == data[i]);
         CHECK(v->ref_count() == 1);
      }
      CHECK(list1.size() == 0);
   }
}
