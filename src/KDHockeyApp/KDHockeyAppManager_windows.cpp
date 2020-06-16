//
// Copyright (C) 2018 F.X.MEILLER GmbH & Co. KG
// All rights reserved.
//
// This file is part of the KD HockeyApp library.
//
// This library is free software; you can redistribute it and/or modify
// it under the terms of either:
//
//   The GNU Lesser General Public License version 2.1 and version 3
//   as published by the Free Software Foundation and appearing in the
//   file LICENSE.LGPL.txt included.
//
// Or:
//
//   The Mozilla Public License Version 2.0 as published by the Mozilla
//   Foundation and appearing in the file LICENSE.MPL2.txt included.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Contact info@kdab.com if any conditions of this licensing is not clear to you.
//

#include "KDHockeyAppManager_p.h"

#include "KDHockeyAppLiterals_p.h"
#include "KDHockeyAppSoftAssert_p.h"

#include <client/windows/handler/exception_handler.h>

#include <QStandardPaths>

namespace KDHockeyApp {

using google_breakpad::CustomClientInfo;
using google_breakpad::ExceptionHandler;

namespace {

class PlatformData
{
public:
    explicit PlatformData(const QString &path, ExceptionHandler::MinidumpCallback callback, void *context)
        : eh{path.toStdWString(), NoFilter, callback, context, ExceptionHandler::HANDLER_ALL,
             MiniDumpNormal, NoPipeName, NoClientInfo}
    {}

    constexpr static ExceptionHandler::FilterCallback NoFilter = nullptr;
    constexpr static const wchar_t *NoPipeName = nullptr;
    constexpr static const CustomClientInfo *NoClientInfo = nullptr;

    const ExceptionHandler eh;
};

} // namespace

// use multi-inheritance to ensure that the exception handler is initialized before all of the Private fields
class HockeyAppManager::PlatformPrivate : public PlatformData, public Private
{
public:
    explicit PlatformPrivate(const QString &appId, HockeyAppManager *q)
        : PlatformData{dataDirPath(), &PlatformPrivate::onException, this}
        , Private{appId, q}
    {}

private:
    static bool onException(const wchar_t *, const wchar_t *, void *context,
                            EXCEPTION_POINTERS *, MDRawAssertionInfo *, bool succeeded)
    {
        // NOTICE: This context is compromised. Complex operations, allocations must be avoided!
        return static_cast<const PlatformPrivate *>(context)->writeCrashReport(succeeded);
    }
};

HockeyAppManager::Private *HockeyAppManager::Private::create(const QString &appId, HockeyAppManager *q)
{
    return new PlatformPrivate{appId, q};
}

QDir HockeyAppManager::Private::cacheLocation()
{
    return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
}

void HockeyAppManager::Private::fillAppInfo(AppInfo *)
{
}

std::string HockeyAppManager::Private::nextMiniDumpFileName() const
{
    const auto fileName = static_cast<const PlatformPrivate *>(this)->eh.next_minidump_path();
    return QString::fromStdWString(fileName).toStdString();
}

QString HockeyAppManager::Private::requestParameterDeviceId()
{
    return "udid"_l1;
}

bool HockeyAppManager::Private::installedFromMarket()
{
    return false;
}

} // namespace KDHockeyApp
