/*
    SPDX-FileCopyrightText: 2015 Kai Uwe Broulik <kde@privat.broulik.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "kwindowsystemproxy.h"

#include <QMetaMethod>
#include <QWindow>

#include <kwindowsystem.h>

KWindowSystemProxy::KWindowSystemProxy(QObject *parent)
    : QObject(parent)
    , m_initialized(false)
{
}

KWindowSystemProxy::~KWindowSystemProxy()
{
}

void KWindowSystemProxy::connectNotify(const QMetaMethod &signal)
{
    if (m_initialized) {
        return;
    }

    if (signal == QMetaMethod::fromSignal(&KWindowSystemProxy::showingDesktopChanged)) {
        connect(KWindowSystem::self(), &KWindowSystem::showingDesktopChanged, this, &KWindowSystemProxy::showingDesktopChanged);
        m_initialized = true;
    }

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)
    if (signal == QMetaMethod::fromSignal(&KWindowSystemProxy::currentDesktopChanged)
        || signal == QMetaMethod::fromSignal(&KWindowSystemProxy::desktopNamesChanged)
        || signal == QMetaMethod::fromSignal(&KWindowSystemProxy::numberOfDesktopsChanged)
        || signal == QMetaMethod::fromSignal(&KWindowSystemProxy::compositingActiveChanged)) {
        connect(KWindowSystem::self(), &KWindowSystem::currentDesktopChanged, this, [this](int currentDesktop) {
            Q_EMIT currentDesktopChanged(currentDesktop);
            Q_EMIT currentDesktopNameChanged();
        });

        connect(KWindowSystem::self(), &KWindowSystem::desktopNamesChanged, this, [this]() {
            Q_EMIT desktopNamesChanged();
            Q_EMIT currentDesktopNameChanged();
        });

        connect(KWindowSystem::self(), &KWindowSystem::numberOfDesktopsChanged, this, &KWindowSystemProxy::numberOfDesktopsChanged);
        connect(KWindowSystem::self(), &KWindowSystem::compositingChanged, this, &KWindowSystemProxy::compositingActiveChanged);

        m_initialized = true;
    }
#endif
}

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)
int KWindowSystemProxy::currentDesktop() const
{
    return KWindowSystem::currentDesktop();
}
#endif

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)
void KWindowSystemProxy::setCurrentDesktop(int desktop)
{
    KWindowSystem::setCurrentDesktop(desktop);
}
#endif

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)
QString KWindowSystemProxy::currentDesktopName() const
{
    return KWindowSystem::desktopName(KWindowSystem::currentDesktop());
}
#endif

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)
int KWindowSystemProxy::numberOfDesktops() const
{
    return KWindowSystem::numberOfDesktops();
}
#endif

bool KWindowSystemProxy::showingDesktop() const
{
    return KWindowSystem::showingDesktop();
}

void KWindowSystemProxy::setShowingDesktop(bool showingDesktop)
{
    KWindowSystem::setShowingDesktop(showingDesktop);
}

bool KWindowSystemProxy::compositingActive() const
{
    return KWindowSystem::compositingActive();
}

bool KWindowSystemProxy::isPlatformX11() const
{
    return KWindowSystem::isPlatformX11();
}

bool KWindowSystemProxy::isPlatformWayland() const
{
    return KWindowSystem::isPlatformWayland();
}

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)
bool KWindowSystemProxy::hasWindow(QWindow *window) const
{
    if (!window) {
        return false;
    }

    return KWindowSystem::hasWId(window->winId());
}
#endif

void KWindowSystemProxy::forceActivateWindow(QWindow *window, long time)
{
    if (window) {
        KWindowSystem::forceActiveWindow(window->winId(), time);
    }
}

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)
void KWindowSystemProxy::setOnAllDesktops(QWindow *window, bool onAllDesktops)
{
    if (window) {
        KWindowSystem::setOnAllDesktops(window->winId(), onAllDesktops);
    }
}
#endif

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)
void KWindowSystemProxy::setOnDesktop(QWindow *window, int desktop)
{
    if (window) {
        KWindowSystem::setOnDesktop(window->winId(), desktop);
    }
}
#endif

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)
void KWindowSystemProxy::setOnActivities(QWindow *window, const QStringList &activities)
{
    if (window) {
        KWindowSystem::setOnActivities(window->winId(), activities);
    }
}
#endif

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)
QString KWindowSystemProxy::desktopName(int desktop) const
{
    return KWindowSystem::desktopName(desktop);
}
#endif

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)
void KWindowSystemProxy::setDesktopName(int desktop, const QString &name)
{
    KWindowSystem::setDesktopName(desktop, name);
}
#endif
