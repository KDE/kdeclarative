/*
 *    Copyright 2020 Kevin Ottens <kevin.ottens@enioka.com>
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
    Q_PROPERTY(QQuickItem *indicator READ indicator WRITE setIndicator NOTIFY indicatorChanged)
    Q_PROPERTY(bool indicatorAsOverlay READ isIndicatorAsOverlay WRITE setIndicatorAsOverlay NOTIFY indicatorAsOverlayChanged)
    Q_PROPERTY(qreal leftCoord READ leftCoord NOTIFY leftCoordChanged)
public:
    using QObject::QObject;

    QQuickItem *target() const;
    void setTarget(QQuickItem *target);

    QQuickItem *indicator() const;
    void setIndicator(QQuickItem *indicator);

    bool isIndicatorAsOverlay() const;
    void setIndicatorAsOverlay(bool asOverlay);

    qreal leftCoord() const;

Q_SIGNALS:
    void targetChanged();
    void indicatorChanged();
    void indicatorAsOverlayChanged();
    void leftCoordChanged();

private:
    qreal formLeftCoord() const;
    qreal overlayLeftCoord() const;

    void triggerCoordChanges();

    QPointer<QQuickItem> m_target;
    QPointer<QQuickItem> m_indicator;
    bool m_indicatorAsOverlay = false;
};

#endif
