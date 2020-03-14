#ifndef SENTRY_SESSIONS_H_INCLUDED
#define SENTRY_SESSIONS_H_INCLUDED

#include "sentry_boot.h"

#include "sentry_utils.h"

struct sentry_jsonwriter_s;

typedef enum {
    SENTRY_SESSION_STATUS_OK,
    SENTRY_SESSION_STATUS_CRASHED,
    SENTRY_SESSION_STATUS_ABNORMAL,
    SENTRY_SESSION_STATUS_EXITED,
} sentry_session_status_t;

typedef struct sentry_session_s {
    sentry_uuid_t session_id;
    char *distinct_id;
    struct tm started;
    uint64_t started_ms;
    bool init;
    uint64_t errors;
    sentry_session_status_t status;
} sentry_session_t;

sentry_session_t *sentry__session_new(void);
void sentry__session_free(sentry_session_t *session);
void sentry__session_to_json(
    const sentry_session_t *session, struct sentry_jsonwriter_s *jw);

void sentry__end_current_session_with_status(sentry_session_status_t status);
void sentry__record_errors_on_current_session(uint32_t error_count);

void sentry__add_current_session_to_envelope(sentry_envelope_t *envelope);

#endif
