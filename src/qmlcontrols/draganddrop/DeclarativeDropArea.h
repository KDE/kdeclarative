/*
    SPDX-FileCopyrightText: 2010 BetterInbox <contact@betterinbox.com>
    SPDX-FileContributor: Gregory Schlomoff <greg@betterinbox.com>

    SPDX-License-Identifier: MIT
*/

#ifndef DECLARATIVEDROPAREA_H
#define DECLARATIVEDROPAREA_H

#include <QQuickItem>

class DeclarativeDragDropEvent;

/*!
 * \qmltype DropArea
 * \inqmlmodule org.kde.draganddrop
 */
class DeclarativeDropArea : public QQuickItem
{
    Q_OBJECT

    /*!
     * \qmlproperty bool DropArea::enabled
     * If false the area will receive no drop events
     */
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)

    /*!
     * \qmlproperty bool DropArea::preventStealing
     */
    Q_PROPERTY(bool preventStealing READ preventStealing WRITE setPreventStealing NOTIFY preventStealingChanged)

    /*!
     * \qmlproperty bool DropArea::containsDrag
     */
    Q_PROPERTY(bool containsDrag READ containsDrag NOTIFY containsDragChanged)

public:
    DeclarativeDropArea(QQuickItem *parent = nullptr);
    bool isEnabled() const;
    void setEnabled(bool enabled);

    bool preventStealing() const;
    void setPreventStealing(bool prevent);
    bool containsDrag() const;

Q_SIGNALS:
    /*!
     * \qmlsignal DropArea::dragEnter(DeclarativeDragDropEvent *event)
     *
     * Emitted when the mouse cursor dragging something enters in the drag area
     *
     * \a event description of the dragged content
     * \sa DragDropEvent
     */
    void dragEnter(DeclarativeDragDropEvent *event);

    /*!
     * \qmlsignal DropArea::dragLeave(DeclarativeDragDropEvent *event)
     *
     * Emitted when the mouse cursor dragging something leaves the drag area
     *
     * \a event description of the dragged content
     * \sa DragDropEvent
     */
    void dragLeave(DeclarativeDragDropEvent *event);

    /*!
     * \qmlsignal DropArea::dragMove(DeclarativeDragDropEvent *event)
     *
     * Emitted when the mouse cursor dragging something moves over the drag area
     *
     * \a event description of the dragged content
     * \sa DragDropEvent
     */
    void dragMove(DeclarativeDragDropEvent *event);

    /*!
     * \qmlsignal DropArea::drop(DeclarativeDragDropEvent *event)
     *
     * Emitted when the user drops something in the area
     *
     * \a event description of the dragged content
     * \sa DragDropEvent
     */
    void drop(DeclarativeDragDropEvent *event);

    // Notifiers
    void enabledChanged();

    void preventStealingChanged();

    void containsDragChanged(bool contained);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private Q_SLOTS:
    void temporaryInhibitParent(bool inhibit);

private:
    void setContainsDrag(bool dragging);

    bool m_enabled : 1;
    bool m_preventStealing : 1;
    bool m_temporaryInhibition : 1;
    bool m_containsDrag : 1;
    QPoint m_oldDragMovePos;
};

#endif
