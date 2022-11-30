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

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)
    /**
     * @brief The number of the current desktop
     */
    Q_PROPERTY(int currentDesktop READ currentDesktop WRITE setCurrentDesktop NOTIFY currentDesktopChanged)
#endif

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)
    /**
     * @brief The name of the current desktop
     */
    Q_PROPERTY(QString currentDesktopName READ currentDesktopName NOTIFY currentDesktopNameChanged)
#endif

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)
    /**
     * @brief The number of desktops
     */
    Q_PROPERTY(int numberOfDesktops READ numberOfDesktops NOTIFY numberOfDesktopsChanged)
#endif

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

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)
    int currentDesktop() const;
    void setCurrentDesktop(int desktop);
#endif

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)
    QString currentDesktopName() const;
    int numberOfDesktops() const;
#endif

    bool showingDesktop() const;
    void setShowingDesktop(bool showingDesktop);

    bool compositingActive() const;

    bool isPlatformX11() const;
    bool isPlatformWayland() const;

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)
    /**
     * Test to see if @p window still managed at present.
     * @param window the window to test
     */
    Q_INVOKABLE bool hasWindow(QWindow *window) const;
#endif

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

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)
    /**
     * Sets window @p window to be present on all virtual desktops if @p
     * onAllDesktops true. Otherwise the window lives only on one single desktop.
     *
     * @param window the window
     * @param onAllDesktops true to show the window on all desktops, false otherwise
     */
    Q_INVOKABLE void setOnAllDesktops(QWindow *window, bool onAllDesktops);
#endif

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)
    /**
     * Moves window @p window to desktop @p desktop.
     *
     * @param window the window
     * @param desktop the number of the new desktop
     */
    Q_INVOKABLE void setOnDesktop(QWindow *window, int desktop);
#endif

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)
    /**
     * Moves window @p window to activities @p activities.
     *
     * @param window the window
     * @param activities the list of activity UUIDs
     */
    Q_INVOKABLE void setOnActivities(QWindow *window, const QStringList &activities);
#endif

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)
    /**
     * Returns the name of the specified desktop.
     * @see currentDesktopName
     * @param desktop the number of the desktop
     * @return the name of the desktop
     **/
    Q_INVOKABLE QString desktopName(int desktop) const;
#endif

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)
    /**
     * Sets the name of the specified desktop.
     * @param desktop the number of the desktop
     * @param name the new name for the desktop
     **/
    Q_INVOKABLE void setDesktopName(int desktop, const QString &name);
#endif

Q_SIGNALS:
#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)
    void currentDesktopChanged(int desktop);
#endif

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)
    void currentDesktopNameChanged();
#endif

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)
    void desktopNamesChanged();
#endif

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)
    void numberOfDesktopsChanged(int numberOfDesktops);
#endif

    void showingDesktopChanged(bool showingDesktop);
    void compositingActiveChanged(bool compositingActive);

protected:
    void connectNotify(const QMetaMethod &signal) override;

private:
    bool m_initialized;
};

#endif // KWINDOWSYSTEMPROXY_H
