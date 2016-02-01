/*
 * Copyright 2015 Kai Uwe Broulik <kde@privat.broulik.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <QMetaMethod>
#include <QWindow>

#include <kwindowsystem.h>

#include "kwindowsystemproxy.h"

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

    if (signal == QMetaMethod::fromSignal(&KWindowSystemProxy::currentDesktopChanged) ||
        signal == QMetaMethod::fromSignal(&KWindowSystemProxy::desktopNamesChanged) ||
        signal == QMetaMethod::fromSignal(&KWindowSystemProxy::numberOfDesktopsChanged) ||
        signal == QMetaMethod::fromSignal(&KWindowSystemProxy::showingDesktopChanged) ||
        signal == QMetaMethod::fromSignal(&KWindowSystemProxy::compositingActiveChanged)) {

        connect(KWindowSystem::self(), &KWindowSystem::currentDesktopChanged, this, [this](int currentDesktop) {
            Q_EMIT currentDesktopChanged(currentDesktop);
            Q_EMIT currentDesktopNameChanged();
        });

        connect(KWindowSystem::self(), &KWindowSystem::desktopNamesChanged, this, [this]() {
            Q_EMIT desktopNamesChanged();
            Q_EMIT currentDesktopNameChanged();
        });

        connect(KWindowSystem::self(), &KWindowSystem::numberOfDesktopsChanged,
                this, &KWindowSystemProxy::numberOfDesktopsChanged);
        connect(KWindowSystem::self(), &KWindowSystem::showingDesktopChanged,
                this, &KWindowSystemProxy::showingDesktopChanged);
        connect(KWindowSystem::self(), &KWindowSystem::compositingChanged,
                this, &KWindowSystemProxy::compositingActiveChanged);

        m_initialized = true;
    }
}

int KWindowSystemProxy::currentDesktop() const
{
    return KWindowSystem::currentDesktop();
}

void KWindowSystemProxy::setCurrentDesktop(int desktop)
{
    KWindowSystem::setCurrentDesktop(desktop);
}

QString KWindowSystemProxy::currentDesktopName() const
{
    return KWindowSystem::desktopName(KWindowSystem::currentDesktop());
}

int KWindowSystemProxy::numberOfDesktops() const
{
    return KWindowSystem::numberOfDesktops();
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
    return KWindowSystem::compositingActive();
}

bool KWindowSystemProxy::hasWindow(QWindow *window) const
{
    if (!window) {
        return false;
    }

    return KWindowSystem::hasWId(window->winId());
}

void KWindowSystemProxy::forceActivateWindow(QWindow *window, long time)
{
    if (window) {
        KWindowSystem::forceActiveWindow(window->winId(), time);
    }
}

void KWindowSystemProxy::setOnAllDesktops(QWindow *window, bool onAllDesktops)
{
    if (window) {
        KWindowSystem::setOnAllDesktops(window->winId(), onAllDesktops);
    }
}

void KWindowSystemProxy::setOnDesktop(QWindow *window, int desktop)
{
    if (window) {
        KWindowSystem::setOnDesktop(window->winId(), desktop);
    }
}

void KWindowSystemProxy::setOnActivities(QWindow *window, const QStringList &activities)
{
    if (window) {
        KWindowSystem::setOnActivities(window->winId(), activities);
    }
}

QString KWindowSystemProxy::desktopName(int desktop) const
{
    return KWindowSystem::desktopName(desktop);
}

void KWindowSystemProxy::setDesktopName(int desktop, const QString &name)
{
    KWindowSystem::setDesktopName(desktop, name);
}
