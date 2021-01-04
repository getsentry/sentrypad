#include "sentry_attachment.h"
#include "sentry_envelope.h"
#include "sentry_path.h"
#include "sentry_scope.h"
#include "sentry_string.h"
#include "sentry_testsupport.h"
#include <sentry.h>

typedef struct {
    uint64_t called;
    sentry_stringbuilder_t serialized_envelope;
} sentry_attachments_testdata_t;

static void
send_envelope(const sentry_envelope_t *envelope, void *_data)
{
    sentry_attachments_testdata_t *data = _data;
    data->called += 1;
    sentry__envelope_serialize_into_stringbuilder(
        envelope, &data->serialized_envelope);
}

#ifdef __ANDROID__
#    define PREFIX "/data/local/tmp/"
#else
#    define PREFIX ""
#endif

SENTRY_TEST(lazy_attachments)
{
    sentry_attachments_testdata_t testdata;
    testdata.called = 0;
    sentry__stringbuilder_init(&testdata.serialized_envelope);

    sentry_options_t *options = sentry_options_new();
    sentry_options_set_auto_session_tracking(options, false);
    sentry_options_set_dsn(options, "https://foo@sentry.invalid/42");
    sentry_options_set_transport(
        options, sentry_new_function_transport(send_envelope, &testdata));
    sentry_options_set_release(options, "prod");

    sentry_options_add_attachment(options, PREFIX ".existing-file-attachment");
    sentry_options_add_attachment(
        options, PREFIX ".non-existing-file-attachment");
    sentry_path_t *existing
        = sentry__path_from_str(PREFIX ".existing-file-attachment");
    sentry_path_t *non_existing
        = sentry__path_from_str(PREFIX ".non-existing-file-attachment");

    sentry_init(options);

    sentry__path_write_buffer(existing, "foo", 3);
    sentry_capture_event(sentry_value_new_message_event(
        SENTRY_LEVEL_INFO, "root", "Hello World!"));

    char *serialized
        = sentry_stringbuilder_take_string(&testdata.serialized_envelope);
    TEST_CHECK(strstr(serialized,
                   "{\"type\":\"attachment\",\"length\":3,"
                   "\"filename\":\".existing-file-attachment\"}\n"
                   "foo")
        != NULL);
    TEST_CHECK(
        strstr(serialized, "\"filename\":\".non-existing-file-attachment\"")
        == NULL);
    sentry_free(serialized);

    sentry__path_write_buffer(existing, "foobar", 6);
    sentry__path_write_buffer(non_existing, "it exists", 9);
    sentry_capture_event(sentry_value_new_message_event(
        SENTRY_LEVEL_INFO, "root", "Hello World!"));

    serialized
        = sentry_stringbuilder_take_string(&testdata.serialized_envelope);
    TEST_CHECK(strstr(serialized,
                   "{\"type\":\"attachment\",\"length\":6,"
                   "\"filename\":\".existing-file-attachment\"}\n"
                   "foobar")
        != NULL);
    TEST_CHECK(strstr(serialized,
                   "{\"type\":\"attachment\",\"length\":9,"
                   "\"filename\":\".non-existing-file-attachment\"}\n"
                   "it exists")
        != NULL);
    sentry_free(serialized);

    sentry_shutdown();

    sentry__path_remove(existing);
    sentry__path_remove(non_existing);
    sentry__path_free(existing);
    sentry__path_free(non_existing);

    TEST_CHECK_INT_EQUAL(testdata.called, 2);
}

SENTRY_TEST(attachments_add_dedupe)
{
    sentry_options_t *options = sentry_options_new();
    sentry_options_add_attachment(options, PREFIX ".a.txt");
    sentry_options_add_attachment(options, PREFIX ".b.txt");

    sentry_init(options);

    sentry_add_attachment(PREFIX ".a.txt");
    sentry_add_attachment(PREFIX ".b.txt");
    sentry_add_attachment(PREFIX ".c.txt");
#ifdef SENTRY_PLATFORM_WINDOWS
    sentry_add_attachmentw(L".a.txt");
    sentry_add_attachmentw(L".b.txt");
    sentry_add_attachmentw(L".c.txt");
#endif

    sentry_path_t *path_a = sentry__path_from_str(PREFIX ".a.txt");
    sentry_path_t *path_b = sentry__path_from_str(PREFIX ".b.txt");
    sentry_path_t *path_c = sentry__path_from_str(PREFIX ".c.txt");

    sentry__path_write_buffer(path_a, "aaa", 3);
    sentry__path_write_buffer(path_b, "bbb", 3);
    sentry__path_write_buffer(path_c, "ccc", 3);

    sentry_envelope_t *envelope = sentry__envelope_new();
    SENTRY_WITH_SCOPE (scope) {
        sentry__apply_attachments_to_envelope(envelope, scope->attachments);
    }
    char *serialized = sentry_envelope_serialize(envelope, NULL);
    sentry_envelope_free(envelope);

    TEST_CHECK_STRING_EQUAL(serialized,
        "{}\n"
        "{\"type\":\"attachment\",\"length\":3,\"filename\":\".a.txt\"}\naaa\n"
        "{\"type\":\"attachment\",\"length\":3,\"filename\":\".b.txt\"}\nbbb\n"
        "{\"type\":\"attachment\",\"length\":3,\"filename\":\".c.txt\"}"
        "\nccc");

    sentry_free(serialized);

    sentry_shutdown();

    sentry__path_remove(path_a);
    sentry__path_remove(path_b);
    sentry__path_remove(path_c);

    sentry__path_free(path_a);
    sentry__path_free(path_b);
    sentry__path_free(path_c);
}

SENTRY_TEST(attachments_add_remove)
{
    sentry_options_t *options = sentry_options_new();
    sentry_options_add_attachment(options, PREFIX ".a.txt");
    sentry_options_add_attachment(options, PREFIX ".c.txt");
    sentry_options_add_attachment(options, PREFIX ".b.txt");

    sentry_init(options);

    sentry_add_attachment(PREFIX ".c.txt");
    sentry_add_attachment(PREFIX ".d.txt");
#ifdef SENTRY_PLATFORM_WINDOWS
    sentry_add_attachmentw(L".e.txt");
    sentry_add_attachmentw(L".d.txt");
#endif

    sentry_remove_attachment(PREFIX ".c.txt");
    sentry_remove_attachment(PREFIX ".d.txt");
#ifdef SENTRY_PLATFORM_WINDOWS
    sentry_remove_attachmentw(L".e.txt");
    sentry_remove_attachmentw(L".d.txt");
#endif

    sentry_path_t *path_a = sentry__path_from_str(PREFIX ".a.txt");
    sentry_path_t *path_b = sentry__path_from_str(PREFIX ".b.txt");
    sentry_path_t *path_c = sentry__path_from_str(PREFIX ".c.txt");

    sentry__path_write_buffer(path_a, "aaa", 3);
    sentry__path_write_buffer(path_b, "bbb", 3);
    sentry__path_write_buffer(path_c, "ccc", 3);

    sentry_envelope_t *envelope;
    char *serialized;

    envelope = sentry__envelope_new();
    SENTRY_WITH_SCOPE (scope) {
        sentry__apply_attachments_to_envelope(envelope, scope->attachments);
    }
    serialized = sentry_envelope_serialize(envelope, NULL);
    sentry_envelope_free(envelope);

    TEST_CHECK_STRING_EQUAL(serialized,
        "{}\n"
        "{\"type\":\"attachment\",\"length\":3,\"filename\":\".a.txt\"}\naaa\n"
        "{\"type\":\"attachment\",\"length\":3,\"filename\":\".b.txt\"}"
        "\nbbb");

    sentry_free(serialized);

    sentry_remove_attachment(PREFIX ".a.txt");
    sentry_remove_attachment(PREFIX ".b.txt");
#ifdef SENTRY_PLATFORM_WINDOWS
    sentry_remove_attachmentw(L".b.txt");
    sentry_remove_attachmentw(L".a.txt");
#endif

    envelope = sentry__envelope_new();
    SENTRY_WITH_SCOPE (scope) {
        sentry__apply_attachments_to_envelope(envelope, scope->attachments);
    }
    serialized = sentry_envelope_serialize(envelope, NULL);
    sentry_envelope_free(envelope);

    TEST_CHECK_STRING_EQUAL(serialized, "{}");

    sentry_free(serialized);

    sentry_shutdown();

    sentry__path_remove(path_a);
    sentry__path_remove(path_b);
    sentry__path_remove(path_c);

    sentry__path_free(path_a);
    sentry__path_free(path_b);
    sentry__path_free(path_c);
}
