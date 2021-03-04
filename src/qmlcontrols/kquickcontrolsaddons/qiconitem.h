/*
    SPDX-FileCopyrightText: 2011 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef QICONITEM_H
#define QICONITEM_H

#include <QIcon>
#include <QQuickItem>
#include <QVariant>

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
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;

private:
    QIcon m_icon;
    bool m_smooth;
    State m_state;
    bool m_changed;
};

#endif
