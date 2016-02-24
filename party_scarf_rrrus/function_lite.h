#pragma once

// C++11 lambdas are super awesome and wildly useful.  There's just one BIG problem
// with them.  Any lambda that captures context is a unique type that cannot be
// specified in code.  You can create a local auto var and assign it a capturing lambda,
// but there's no way to typedef it.  The only hope for storing and passing around
// capturing lambdas is with templates.  The STL has std::function to solve this issue.
// avr-g++ does not have STL.  So, we're in a quandry if we want to use this feature that
// otherwise the compiler supports.
//
// This file provides a light-ish weight (from a compiler perspective) implementation
// of std::function's lambda handling capability.  This implementation is not as fast
// as the full STL version in that it uses a virtual method for invocation of the
// lambda, but it is simple enough for the avr-g++ compiler to grok.  One other caveat
// is that the function class leaks its __function_ih member, since I haven't sorted
// out copy ownership yet, so don't create function objects in your render loop, only
// create them at setup.
//
// ---rrrus

template<class _Fp> class __function_ih;

template<class _Rp, class ..._ArgTypes>
class __function_ih<_Rp(_ArgTypes...)> {
public:
  virtual ~__function_ih() {}
  virtual _Rp operator()(_ArgTypes...) const = 0;
};

template<class _La, class _Rp, class ..._ArgTypes>
class __function_h : public __function_ih<_Rp(_ArgTypes...)> {
  _La _lamda;
public:
  __function_h(_La func) : _lamda(func) {}

  _Rp operator()(_ArgTypes... __arg) const override {
    return _lamda(__arg...);
  }
};

// function

template<class _Fp> class function;

template<class _Rp, class ..._ArgTypes>
class function<_Rp(_ArgTypes...)> {
  __function_ih<_Rp(_ArgTypes...)> *_fih;

public:
  function() : _fih(0) {}
  function(const function &other) : _fih(other._fih) {}
  ~function() {}

  // C++11 has no way of specifying the type of a lambda that captures
  // any context.  Each lambda is a unique type.  So the only way to
  // store them in a way that can be passed around is with templates.
  // This constructor template can take any lambda type and creates a
  // specialized container to hold it.  The function signature of the
  // lambda and the specialization of the function template must match
  // or it is a compile error.
  template<class _Fp>
    function(_Fp fp) {
      _fih = new __function_h<_Fp, _Rp, _ArgTypes...>(fp);
    }

  // function invocation:
  _Rp operator()(_ArgTypes... __arg) const {
    if (!_fih) return _Rp();
    return (*_fih)(__arg...);
  }
};
