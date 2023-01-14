/*
    SPDX-FileCopyrightText: 2015 Kai Uwe Broulik <kde@privat.broulik.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "kwindowsystemproxy.h"

#include <QMetaMethod>
#include <QWindow>

#include <kwindowsystem.h>

#include "config.h"
#if HAVE_X11
#include <KX11Extras>
#endif

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
}

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
#if HAVE_X11
    if (KWindowSystem::isPlatformX11()) {
        return KX11Extras::compositingActive();
    } else {
        return true;
    }
#else
    return true;
#endif
}

bool KWindowSystemProxy::isPlatformX11() const
{
    return KWindowSystem::isPlatformX11();
}

bool KWindowSystemProxy::isPlatformWayland() const
{
    return KWindowSystem::isPlatformWayland();
}

void KWindowSystemProxy::forceActivateWindow(QWindow *window, long time)
{
    if (window) {
#if HAVE_X11
        if (KWindowSystem::isPlatformX11()) {
            KX11Extras::forceActiveWindow(window->winId(), time);
        } else {
            KWindowSystem::activateWindow(window);
        }
#else
        KWindowSystem::activateWindow(window);
#endif
    }
}
