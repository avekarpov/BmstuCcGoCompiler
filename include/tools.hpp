#pragma once

#define __DERIVED__ Derived

#define PARENT_CLASS template <class __DERIVED__> class

#define FROM_DERIVED static_cast<__DERIVED__ &>(*this)

#undef __DERIVED__
