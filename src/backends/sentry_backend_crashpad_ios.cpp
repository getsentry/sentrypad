extern "C" {
#include "sentry_boot.h"

#include "sentry_alloc.h"
#include "sentry_backend.h"
#include "sentry_core.h"
}

#include <map>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#pragma GCC diagnostic ignored "-Wfour-char-constants"

#include "client/crashpad_client.h"

#pragma GCC diagnostic pop

extern "C" {

static int
sentry__crashpad_backend_startup(
    sentry_backend_t *UNUSED(backend), const sentry_options_t *UNUSED(options))
{
    SENTRY_TRACE("starting crashpad inprocess handler");

    crashpad::CrashpadClient client;
    client.StartCrashpadInProcessHandler();

    return 0;
}

sentry_backend_t *
sentry__backend_new(void)
{
    sentry_backend_t *backend = SENTRY_MAKE(sentry_backend_t);
    if (!backend) {
        return NULL;
    }
    memset(backend, 0, sizeof(sentry_backend_t));

    backend->startup_func = sentry__crashpad_backend_startup;
    backend->can_capture_after_shutdown = true;

    return backend;
}
}
