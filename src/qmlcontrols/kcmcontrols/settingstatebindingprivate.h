/*
    SPDX-FileCopyrightText: 2020 Kevin Ottens <kevin.ottens@enioka.com>
    SPDX-FileCopyrightText: 2020 Cyril Rossi <cyril.rossi@enioka.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef SETTINGSTATEBINDINGPRIVATE_H
#define SETTINGSTATEBINDINGPRIVATE_H

#include <QQuickItem>
#include <QPointer>

class SettingStateBindingPrivate : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem *target READ target WRITE setTarget NOTIFY targetChanged)
    Q_PROPERTY(bool highlight READ highlight WRITE setHighlight NOTIFY highlightChanged)
    Q_PROPERTY(bool defaultIndicatorVisible READ defaultIndicatorVisible WRITE setDefaultIndicatorVisible NOTIFY defaultIndicatorVisibleChanged)

public:
    using QObject::QObject;

    QQuickItem *target() const;
    void setTarget(QQuickItem *target);

    bool highlight() const;
    void setHighlight(bool highlight);

    bool defaultIndicatorVisible() const;
    void setDefaultIndicatorVisible(bool enabled);

Q_SIGNALS:
    void targetChanged();
    void highlightChanged();
    void defaultIndicatorVisibleChanged(bool enabled);

public Q_SLOTS:
    void updateTarget();

private:
    QPointer<QQuickItem> m_target = nullptr;
    QPointer<QQuickItem> m_styleTarget = nullptr;
    bool m_highlight = false;
    bool m_enabled = false;
};

#endif
