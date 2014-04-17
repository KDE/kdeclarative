/*
    Copyright (C) 2010 by BetterInbox <contact@betterinbox.com>
    Original author: Gregory Schlomoff <greg@betterinbox.com>

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#ifndef DECLARATIVEDROPAREA_H
#define DECLARATIVEDROPAREA_H

#include <QQuickItem>

class DeclarativeDragDropEvent;

class DeclarativeDropArea : public QQuickItem
{
    Q_OBJECT

    /**
     * If false the area will receive no drop events
     */
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)

    /**
     * 
     */
    Q_PROPERTY(bool preventStealing READ preventStealing WRITE setPreventStealing NOTIFY preventStealingChanged)

public:
    DeclarativeDropArea(QQuickItem *parent=0);
    bool isEnabled() const;
    void setEnabled(bool enabled);

    bool preventStealing() const;
    void setPreventStealing(bool prevent);

Q_SIGNALS:
    /**
     * Emitted when the mouse cursor dragging something enters in the drag area
     * @arg DeclarativeDragDropEvent description of the dragged content
     * @see DeclarativeDragDropEvent
     */
    void dragEnter(DeclarativeDragDropEvent* event);

    /**
     * Emitted when the mouse cursor dragging something leaves the drag area
     * @arg DeclarativeDragDropEvent description of the dragged content
     * @see DeclarativeDragDropEvent
     */
    void dragLeave(DeclarativeDragDropEvent* event);

    /**
     * Emitted when the mouse cursor dragging something moves over the drag area
     * @arg DeclarativeDragDropEvent description of the dragged content
     * @see DeclarativeDragDropEvent
     */
    void dragMove(DeclarativeDragDropEvent *event);

    /**
     * Emitted when the user drops something in the area
     * @arg DeclarativeDragDropEvent description of the dragged content
     * @see DeclarativeDragDropEvent
     */
    void drop(DeclarativeDragDropEvent* event);

    //Notifiers
    void enabledChanged();

    void preventStealingChanged();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

private Q_SLOTS:
    void temporaryInhibitParent(bool inhibit);

private:
    bool m_enabled : 1;
    bool m_preventStealing : 1;
    bool m_temporaryInhibition : 1;
};

#endif

