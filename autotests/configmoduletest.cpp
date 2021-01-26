// SPDX-License-Identifier: LGPL-2.0-or-later
// SPDX-FileCopyrightText: 2021 Harald Sitter <sitter@kde.org>

#include <QTest>
#include <KAboutData>

#include "configmodule.h"

class ConfigModuleTest : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

private Q_SLOTS:
    void test_setAboutDataFromMetaData()
    {
        const KPluginMetaData data = KPluginMetaData::fromDesktopFile(QFINDTESTDATA("metadata.desktop"));
        KQuickAddons::ConfigModule module;
        module.setAboutData(data);
        const KAboutData *about = module.aboutData();
        QCOMPARE(about->componentName(), QStringLiteral("kcm_freespacenotifier"));
        QCOMPARE(about->displayName(), QStringLiteral("Free Space Notifier"));
        QCOMPARE(about->version(), QStringLiteral("1.0"));
        QCOMPARE(about->shortDescription(), QStringLiteral("Warns when running out of space on your home folder"));
        QCOMPARE(about->licenses().count(), 1);
        QCOMPARE(about->licenses().at(0).spdx(), QStringLiteral("GPL-2.0"));
        QCOMPARE(about->authors().count(), 1);
        QCOMPARE(about->authors().at(0).name(), QStringLiteral("Harald Sitter"));
    }
};

QTEST_GUILESS_MAIN(ConfigModuleTest)

#include "configmoduletest.moc"
