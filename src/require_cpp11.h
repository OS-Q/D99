#ifndef REQUIRE_CPP11_H
#define REQUIRE_CPP11_H

#if __cplusplus < 201103L
#error "This library needs at least a C++11 compliant compiler, maybe compiler argument for C++11 support is missing or if you use Arduino IDE upgrade to version >=1.6.6"
#endif

#endif // REQUIRE_CPP11_H
