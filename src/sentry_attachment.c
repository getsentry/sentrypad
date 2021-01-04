#include "sentry_attachment.h"
#include "sentry_alloc.h"
#include "sentry_envelope.h"
#include "sentry_options.h"
#include "sentry_path.h"
#include "sentry_value.h"

static void
sentry__attachment_free(sentry_attachment_t *attachment)
{
    sentry__path_free(attachment->path);
    sentry_free(attachment);
}

void
sentry__attachments_free(sentry_attachment_t *attachments)
{
    sentry_attachment_t *next_attachment = attachments;
    while (next_attachment) {
        sentry_attachment_t *attachment = next_attachment;
        next_attachment = attachment->next;

        sentry__attachment_free(attachment);
    }
}

void
sentry__attachment_add(
    sentry_attachment_t **attachments_ptr, sentry_path_t *path)
{
    if (!path) {
        return;
    }
    sentry_attachment_t *attachment = SENTRY_MAKE(sentry_attachment_t);
    if (!attachment) {
        sentry__path_free(path);
        return;
    }
    attachment->path = path;
    attachment->next = NULL;

    sentry_attachment_t **next_ptr = attachments_ptr;

    for (sentry_attachment_t *last_attachment = *attachments_ptr;
         last_attachment; last_attachment = last_attachment->next) {
        if (sentry__path_eq(last_attachment->path, path)) {
            sentry__attachment_free(attachment);
            return;
        }

        next_ptr = &last_attachment->next;
    }

    *next_ptr = attachment;
}

void
sentry__attachment_remove(
    sentry_attachment_t **attachments_ptr, sentry_path_t *path)
{
    sentry_attachment_t **next_ptr = attachments_ptr;

    for (sentry_attachment_t *attachment = *attachments_ptr; attachment;
         attachment = attachment->next) {
        if (sentry__path_eq(attachment->path, path)) {
            *next_ptr = attachment->next;
            sentry__attachment_free(attachment);
            goto out;
        }

        next_ptr = &attachment->next;
    }

out:
    sentry__path_free(path);
}

/**
 * Reads the attachments from disk and adds them to the `envelope`.
 */
void
sentry__apply_attachments_to_envelope(
    sentry_envelope_t *envelope, const sentry_attachment_t *attachments)
{
    if (!attachments) {
        return;
    }

    SENTRY_TRACE("adding attachments to envelope");
    for (const sentry_attachment_t *attachment = attachments; attachment;
         attachment = attachment->next) {
        sentry_envelope_item_t *item = sentry__envelope_add_from_path(
            envelope, attachment->path, "attachment");
        if (!item) {
            continue;
        }
        sentry__envelope_item_set_header(item, "filename",
#ifdef SENTRY_PLATFORM_WINDOWS
            sentry__value_new_string_from_wstr(
#else
            sentry_value_new_string(
#endif
                sentry__path_filename(attachment->path)));
    }
}
