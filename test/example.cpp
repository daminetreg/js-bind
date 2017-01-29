#include <iostream>

#include <js/bind.hpp>

int main(int argc, char** argv) {

  using namespace std;
  using namespace std::placeholders;
  using emscripten::val;

  val::global("document")["body"].set("innerHTML", "<button id=\"clickme_btn\">Click me</button>");
  auto clickme_btn = val::global("document").call<val>("getElementById", string("clickme_btn"));

  auto onclick = [](val event){ cout << "hello world ! " << endl; };
  clickme_btn.set("onclick", js::bind(onclick, _1));

  return 0;
}
