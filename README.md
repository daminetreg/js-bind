# js::bind
`js::bind` makes any C++ function, member function or lambda as a Javascript callable callback for [Emscripten](http:://www.emscripten.org/).

## Easy to use
It's header only and dependency-free, as described here : [INSTALL](./INSTALL.md).

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

## License
Please give copyright notice for this project : [Boost Software License](./LICENSE).

```
Copyright (c) 2017 Damien Buhl alias daminetreg (damien.buhl@lecbna.org)
```

 * [Damien Buhl alias daminetreg](mailto:damien.buhl@lecbna.org)
