# js::bind
`js::bind` makes any C++ function, member function or lambda as a Javascript callable callback for [Emscripten](http:://www.emscripten.org/).

## Easy to use
It's header only. You can set-it-up in 0-time with : [`nxxm .`](https://nxxm.github.io/), add this to `.nxxm/deps`

```
{
  "daminetreg/js-bind" : { "@" : "v0.0.2" } 
}
```

* [Simple Example in Browser](https://daminetreg.github.io/js-bind/example.html) : [src: ./test/example.html](./test/example.html)
* [HTTP Request Example](https://daminetreg.github.io/js-bind/bind.html) : [src: ./test/bind.html](./test/bind.html)

```cpp

  using namespace std;
  using namespace std::placeholders;
  using emscripten::val;

  auto clickme_btn = val::global("document").call<val>("getElementById", "clickme_btn"s);

  auto onclick = [](val event){ 
    std::cout << "hello world ! " << std::endl; 
  };

  clickme_btn.set("onclick", js::bind(onclick, _1));
```

## License
Please give copyright notice for this project : [Boost Software License](./LICENSE).

```
Copyright (c) 2017 Damien Buhl alias daminetreg (damien.buhl@lecbna.org)
```

 * [Damien Buhl alias daminetreg](mailto:damien.buhl@lecbna.org)
