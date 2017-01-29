# js::bind
`js::bind` makes any C++ function, member function or lambda as a Javascript callable callback for [Emscripten](http:://www.emscripten.org/).

## Easy to use

```cpp

  using namespace std;
  using std::placeholders;
  using emscripten::val;

  val::global("document")["body"].set("innerHTML", "<button id=\"clickme_btn\">Click me</button>");
  auto clickme_btn = val::global("document").call<val>("getElementById", string("clickme_btn"));

  auto onclick = [](val event){ cout << "hello world ! " << endl; };
  clickme_btn.set("onclick", js::bind(onclick, _1));
```

## Authors

 * [Damien Buhl alias daminetreg](mailto:damien.buhl@lecbna.org)
