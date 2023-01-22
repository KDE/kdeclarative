/*
    SPDX-FileCopyrightText: 2015 Kai Uwe Broulik <kde@privat.broulik.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef KWINDOWSYSTEMPROXY_H
#define KWINDOWSYSTEMPROXY_H

#include <QObject>
#include <QString>

#include "kdeclarative/kdeclarative_export.h"

class QMetaMethod;
class QWindow;

/**
 * This item exposes various properties of KWindowSystem for use inside a QML environment.
 *
 * Example usage:
 * @code
 * import org.kde.kwindowsystem 1.0
 *
 * KWindowSystem {
 *     id: kwindowsystem
 * }
 *
 * Rectangle {
 *     width: 50
 *     height: 50
 *     color: "blue"
 *     opacity: kwindowsystem.compositingActive ? 0.5 : 1
 * }
 *
 * Label {
 *     text: i18n("You are currently on Desktop '%1'", kwindowsystem.currentDesktopName)
 * }
 * @endcode
 *
 * @note The features provided by this item are specific to the windowing system and might not
 * be supported on any platform other than X11
 */
class KWindowSystemProxy : public QObject
{
    Q_OBJECT

    /**
     * @brief Whether "show desktop" is currently active
     */
    Q_PROPERTY(bool showingDesktop READ showingDesktop WRITE setShowingDesktop NOTIFY showingDesktopChanged)
    /**
     * @brief Whether desktop compositing is active
     *
     * @note This property is updated delayed
     */
    Q_PROPERTY(bool compositingActive READ compositingActive NOTIFY compositingActiveChanged)

    /**
     * @brief Whether the Platform is X11.
     *
     * @see isPlatformWayland
     * @since 5.96
     */
    Q_PROPERTY(bool isPlatformX11 READ isPlatformX11 CONSTANT)

    /**
     * @brief Whether the Platform is Wayland.
     *
     * @see isPlatformX11
     * @since 5.96
     */
    Q_PROPERTY(bool isPlatformWayland READ isPlatformWayland CONSTANT)

public:
    explicit KWindowSystemProxy(QObject *parent = nullptr);
    ~KWindowSystemProxy() override;

    bool showingDesktop() const;
    void setShowingDesktop(bool showingDesktop);

    bool compositingActive() const;

    bool isPlatformX11() const;
    bool isPlatformWayland() const;

    /**
     * Sets window @p window to be the active window.
     *
     * @note This should be called only in special cases, applications
     * shouldn't force themselves or other windows to be the active
     * window. Generally, this call should used only by pagers
     * and similar tools.
     *
     * @param window the window to make active
     * @param time X server timestamp of the user activity that caused this request
     */
    Q_INVOKABLE void forceActivateWindow(QWindow *window, long time = 0);

Q_SIGNALS:
    void showingDesktopChanged(bool showingDesktop);
    void compositingActiveChanged(bool compositingActive);

protected:
    void connectNotify(const QMetaMethod &signal) override;

private:
    bool m_initialized;
};

#endif // KWINDOWSYSTEMPROXY_H
