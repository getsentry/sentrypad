#include <stdio.h>
#include <atomic>
#include <map>
#include <string>
#include <vector>

#include "../../attachment.hpp"
#include "../../backend.hpp"
#include "../../internal.hpp"
#include "../../options.hpp"
#include "../../path.hpp"

#include "client/crash_report_database.h"
#include "client/crashpad_client.h"
#include "client/crashpad_info.h"
#include "client/settings.h"

using namespace crashpad;

static SimpleStringDictionary simple_annotations;

namespace sentry {

void init_backend() {
    const sentry_options_t *options = sentry_get_options();

    base::FilePath database(options->database_path.as_osstr());
    base::FilePath handler(options->handler_path.as_osstr());
    std::map<std::string, std::string> annotations;
    std::map<std::string, base::FilePath> file_attachments;

    std::vector<sentry::Attachment>::const_iterator iter;
    for (iter = options->attachments.begin();
         iter != options->attachments.end(); ++iter) {
        file_attachments.insert(std::make_pair(
            iter->name(), base::FilePath(iter->path().as_osstr())));
    }

    std::vector<std::string> arguments;
    arguments.push_back("--no-rate-limit");

    CrashpadClient client;
    std::string url = options->dsn.get_minidump_url();
    bool success = client.StartHandlerWithAttachments(
        handler, database, database, url, annotations, file_attachments,
        arguments,
        /* restartable */ true,
        /* asynchronous_start */ false);

    if (success) {
        SENTRY_PRINT_DEBUG("Started client handler.");
    } else {
        SENTRY_PRINT_ERROR("Failed to start client handler.");
        return;
    }

    std::unique_ptr<CrashReportDatabase> db =
        CrashReportDatabase::Initialize(database);

    if (db != nullptr && db->GetSettings() != nullptr) {
        db->GetSettings()->SetUploadsEnabled(true);
    }

    CrashpadInfo *crashpad_info = CrashpadInfo::GetCrashpadInfo();
    crashpad_info->set_simple_annotations(&simple_annotations);
}

}  // namespace sentry