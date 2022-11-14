/*
    SPDX-FileCopyrightText: 2011 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef QICONITEM_H
#define QICONITEM_H

#include <QIcon>
#include <QQuickItem>
#include <QVariant>

#include "../../kdeclarative/kdeclarative_export.h"

#if KDECLARATIVE_BUILD_DEPRECATED_SINCE(5, 101)

/**
 * @deprecated since 5.101, use Kirigami.Icon or PlasmaCore.IconItem instead
 */
class QIconItem : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QVariant icon READ icon WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(bool smooth READ smooth WRITE setSmooth NOTIFY smoothChanged)
    Q_PROPERTY(int implicitWidth READ implicitWidth CONSTANT)
    Q_PROPERTY(int implicitHeight READ implicitHeight CONSTANT)
    Q_PROPERTY(State state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY stateChanged)

public:
    enum State {
        DefaultState, /// The default state.
        ActiveState, /// Icon is active.
        DisabledState, /// Icon is disabled.
        SelectedState, /// Icon is selected
    };
    Q_ENUM(State)

    explicit QIconItem(QQuickItem *parent = nullptr);
    ~QIconItem() override;

    void setIcon(const QVariant &icon);
    QIcon icon() const;

    QIconItem::State state() const;
    void setState(State state);

    int implicitWidth() const;
    int implicitHeight() const;

    void setSmooth(const bool smooth);
    bool smooth() const;

    void setEnabled(bool enabled = true);
    bool enabled() const;

    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *data) override;

Q_SIGNALS:
    void iconChanged();
    void smoothChanged();
    void stateChanged(State state);

protected:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
#else
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
#endif

private:
    QIcon m_icon;
    bool m_smooth;
    State m_state;
    bool m_changed;
};

#endif
#endif
