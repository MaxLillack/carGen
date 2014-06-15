#ifndef DSL_H_INCLUDED
#define DSL_H_INCLUDED

#include "generator.h"
#include "parts.h"
#include <type_traits>

class EcoLine{};
class StrictEcoLine{};

struct EndElement{};

template<class DSLClass = EndElement, class... DSLClasses>
struct DSL{
    //should be false every time, but only evaluated if the this Template is instantiated
    static_assert(is_same<DSLClass, bool>::value, "Unknown DSLClass handling. The is no DSL specialization for given class");
};

template<>
struct DSL<EndElement>{
    template<template<class>class... Parts>
    using Gen = Generator<Parts...>;
};

template<class... DSLClasses>
struct DSL<EcoLine, DSLClasses...>{
    template<template<class>class... Parts>
    using Gen = typename DSL<DSLClasses...>::template Gen<Parts..., parts::ElectroEngine>;
};


template<class... DSLClasses>
struct DSL<StrictEcoLine, DSLClasses...>{
    template<template<class>class... Parts>
    struct DefParts{
        using Gen2 = typename DSL<DSLClasses...>::template Gen<Parts..., parts::ElectroEngine>;
        static_assert(!(Gen2::template containsPart<parts::GasEngine>()), "Gas is not Eco");
    };

    template<template<class>class... Parts>
    using Gen = typename DefParts<Parts...>::Gen2;
};



#endif // DSL_H_INCLUDED
