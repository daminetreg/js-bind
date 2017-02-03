# js::bind
`js::bind` makes any C++ function, member function or lambda as a Javascript callable callback for [Emscripten](http:://www.emscripten.org/).

## Easy to use

* Compile with : `emcc --bind -Ijs-bind/ -Ijs-bind/all-inclusive`

Try the [following example in your browser](https://daminetreg.github.io/js-bind/test/build/example.html) : 

```cpp

  using namespace std;
  using namespace std::placeholders;
  using emscripten::val;

  val::global("document")["body"].set("innerHTML", "<button id=\"clickme_btn\">Click me</button>");
  auto clickme_btn = val::global("document").call<val>("getElementById", string("clickme_btn"));

  auto onclick = [](val event){ cout << "hello world ! " << endl; };
  clickme_btn.set("onclick", js::bind(onclick, _1));
```

### Dependencies all-inclusive
If you don't care of managing the dependencies, you can simply use the shipped-with folder [all-inclusive/](all-inclusive/) to your compiler path, otherwise simply omit it and provide the following dependencies:

- [Boost.Preprocessor](https://github.com/boostorg/preprocessor/)
- [cpp-pre type_traits](https://github.com/cpp-pre/type_traits) 

## Authors

 * [Damien Buhl alias daminetreg](mailto:damien.buhl@lecbna.org)
