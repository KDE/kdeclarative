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
 * \qmltype DeclarativeDropArea
 * \inqmlmodule org.kde.draganddrop
 */
class DeclarativeDropArea : public QQuickItem
{
    Q_OBJECT

    /*!
     * \qmlproperty bool DeclarativeDropArea::enabled
     * If false the area will receive no drop events
     */
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)

    /*!
     * \qmlproperty bool DeclarativeDropArea::preventStealing
     */
    Q_PROPERTY(bool preventStealing READ preventStealing WRITE setPreventStealing NOTIFY preventStealingChanged)

    /*!
     * \qmlproperty bool DeclarativeDropArea::containsDrag
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
     * \qmlsignal DeclarativeDropArea::dragEnter(DeclarativeDragDropEvent *event)
     * Emitted when the mouse cursor dragging something enters in the drag area
     * \a event description of the dragged content
     * \sa DeclarativeDragDropEvent
     */
    void dragEnter(DeclarativeDragDropEvent *event);

    /*!
     * \qmlsignal DeclarativeDropArea::dragLeave(DeclarativeDragDropEvent *event)
     * Emitted when the mouse cursor dragging something leaves the drag area
     * \a event description of the dragged content
     * \sa DeclarativeDragDropEvent
     */
    void dragLeave(DeclarativeDragDropEvent *event);

    /*!
     * \qmlsignal DeclarativeDropArea::dragMove(DeclarativeDragDropEvent *event)
     * Emitted when the mouse cursor dragging something moves over the drag area
     * \a event description of the dragged content
     * \sa DeclarativeDragDropEvent
     */
    void dragMove(DeclarativeDragDropEvent *event);

    /*!
     * \qmlsignal DeclarativeDropArea::drop(DeclarativeDragDropEvent *event)
     * Emitted when the user drops something in the area
     * \a event description of the dragged content
     * \sa DeclarativeDragDropEvent
     */
    void drop(DeclarativeDragDropEvent *event);

    // Notifiers
    /*! \qmlsignal DeclarativeDropArea::enabledChanged() */
    void enabledChanged();

    /*! \qmlsignal DeclarativeDropArea::preventStealingChanged() */
    void preventStealingChanged();

    /*! \qmlsignal DeclarativeDropArea::containsDragChanged(bool contained) */
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
