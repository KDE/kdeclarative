/*
    SPDX-FileCopyrightText: 2015 Kai Uwe Broulik <kde@privat.broulik.de>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef KCMSHELL_H
#define KCMSHELL_H

#include <QObject>

class KCMShell : public QObject
{
    Q_OBJECT

public:
    explicit KCMShell(QObject *parent = nullptr);
    virtual ~KCMShell();

public Q_SLOTS:
    void open(const QStringList &names) const;

    /**
     * Opens the specified module in System Settings. Only a single KCM name may
     * be provided.
     *
     * @code
     * onClicked: KCMShell.openSystemSettings("kcm_kscreen")
     * @endcode
     *
     * @param name A single kcm name to open in System Settings. Opening multiple
     * KCMs using this function is not supported; to do that, use kcmshell.open.
     * @param args Additional arguments to pass to the module.
     *
     * @since 5.71
     */
    void openSystemSettings(const QString &name, const QStringList &args = QStringList()) const;

    /**
     * Opens the specified module in InfCenter. Only a single KCM name may
     * be provided.
     *
     * @code
     * onClicked: KCMShell.openInfoCenter("kcm_energy")
     * @endcode
     *
     * @param name A single kcm name to open in Info Center. Opening multiple
     * KCMs using this function is not supported; to do that, use kcmshell.open.
     *
     * @since 5.71
     */
    void openInfoCenter(const QString &name) const;

    /**
     * Check which of the given control modules the user is
     * allowed to access
     *
     * This can be used for example to hide context menu options
     * that would do nothing if the user wasn't authorized.
     *
     * @code
     * visible: KCMShell.authorize(["org.kde.fooconfig.desktop"]).length > 0
     * @endcode
     *
     * @param menuIds A list of control module menu IDs
     * @return The entries in @p menuIds that the user is
     * authorized to access, may be empty
     * @sa KAuthorized::authorizeControlModules
     * @since 5.23
     */
    QStringList authorize(const QStringList &menuIds) const;
};

#endif // KCMSHELL_H
