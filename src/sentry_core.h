#ifndef SENTRY_CORE_H_INCLUDED
#define SENTRY_CORE_H_INCLUDED

#include "sentry_boot.h"
#include "sentry_logger.h"

#define SENTRY_BREADCRUMBS_MAX 100

#if defined(__GNUC__) && (__GNUC__ >= 4)
#    define MUST_USE __attribute__((warn_unused_result))
#elif defined(_MSC_VER) && (_MSC_VER >= 1700)
#    define MUST_USE _Check_return_
#else
#    define MUST_USE
#endif

#if defined(__GNUC__)
#    define UNUSED(x) UNUSED_##x __attribute__((__unused__))
#elif defined(_MSC_VER)
#    define UNUSED(x) UNUSED_##x __pragma(warning(suppress : 4100))
#else
#    define UNUSED(x) UNUSED_##x
#endif

/**
 * This function will check the user consent, and return `true` if uploads
 * should *not* be sent to the sentry server, and be discarded instead.
 */
bool sentry__should_skip_upload(void);

/**
 * This function is essential to capture reports in the case of a hard crash.
 * It will set a special transport that will dump events to disk.
 * See `sentry__run_write_envelope`.
 */
sentry_transport_t *sentry__swap_disk_transport(sentry_options_t *options);

/**
 * This function will submit the given `envelope` to the configured transport.
 */
void sentry__capture_envelope(sentry_envelope_t *envelope);

/**
 * Generates a new random UUID for events.
 */
sentry_uuid_t sentry__new_event_id(void);

/**
 * This will ensure that the given `event` has a UUID, generating a new one on
 * demand. It will return a serialized UUID as `sentry_value_t` and also write
 * it into the `uuid_out` parameter.
 */
sentry_value_t sentry__ensure_event_id(
    sentry_value_t event, sentry_uuid_t *uuid_out);

/**
 * This will acquire a lock on the global options and return them.
 */
sentry_options_t *sentry__options_lock(void);

/**
 * Release the lock on the global options.
 */
void sentry__options_unlock(void);

#define SENTRY_WITH_OPTIONS(Options)                                           \
    for (sentry_options_t *Options = sentry__options_lock();                   \
         Options || (sentry__options_unlock(), false); Options = NULL)

#endif
