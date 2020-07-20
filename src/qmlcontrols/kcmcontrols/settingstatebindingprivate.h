/*
 *    Copyright 2020 Kevin Ottens <kevin.ottens@enioka.com>
 *    Copyright 2020 Cyril Rossi <cyril.rossi@enioka.com>
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Library General Public
 *    License as published by the Free Software Foundation; either
 *    version 2 of the License, or (at your option) any later version.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Library General Public License for more details.
 *
 *    You should have received a copy of the GNU Library General Public License
 *    along with this library; see the file COPYING.LIB.  If not, write to
 *    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *    Boston, MA 02110-1301, USA.
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
