#include <iostream>
#include <functional>
#include <memory>

#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>

#include <js/bind.hpp>

class MiniXhr : public std::enable_shared_from_this<MiniXhr> {
  using val = emscripten::val;
  using url_t = std::string;

  public:

    void set_url(const url_t& url) { url_ = url; }

    void GET();

    /**
     *
     * The member function to be called from javascript.
     */
    void on_readystate(val event) {
      std::cout << "ready " << std::endl;
      std::cout << "xxhr:: " 
          << event["type"].as<std::string>() << ". "
          << xhr["readyState"].as<size_t>() << " - " << url_ << " :: "
          << xhr["status"].as<size_t>() << ": " 
          << xhr["statusText"].as<std::string>() << std::endl;
    }

  private:
    url_t url_;
    val xhr = val::global("XMLHttpRequest").new_();
};
   

 void MiniXhr::GET() { 
 
  auto callback = js::bind(&MiniXhr::on_readystate, shared_from_this(), std::placeholders::_1);
  xhr.set("onreadystatechange", callback);

  auto lamb = [](val e) { 
    std::cout << "Im a lambda to handle event : " << e["type"].as<std::string>() << std::endl; 
  };

  xhr.set("onload", js::bind(lamb, std::placeholders::_1));
 
  xhr.call<val>("open", std::string("GET"), url_, true);
  xhr.call<val>("send");
}


int main(int argc, char** argv) {
  
  
  auto x = std::make_shared<MiniXhr>(); //shared_ptr needed for callback to work after main exit, due to NO_EXIT_RUNTIME.
  x->set_url("CMakeCache.txt");
  x->GET();

  return 0;
}
