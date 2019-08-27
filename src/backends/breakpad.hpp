#ifndef SENTRY_BACKENDS_BREAKPAD_HPP_INCLUDED
#define SENTRY_BACKENDS_BREAKPAD_HPP_INCLUDED

#include "../internal.hpp"
#include "../scope.hpp"
#include "base.hpp"

namespace sentry {
namespace backends {

class BreakpadBackendImpl;

class BreakpadBackend : public Backend {
   public:
    BreakpadBackend();
    ~BreakpadBackend();
    void start();
    void flush_scope_state(const sentry::Scope &scope);
    void add_breadcrumb(sentry::Value breadcrumb);

   private:
    BreakpadBackendImpl *m_impl;
};
}  // namespace backends
}  // namespace sentry

#endif
