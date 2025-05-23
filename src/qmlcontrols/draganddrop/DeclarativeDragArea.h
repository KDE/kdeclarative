/*
    SPDX-FileCopyrightText: 2010 BetterInbox <contact@betterinbox.com>
    SPDX-FileContributor: Gregory Schlomoff <greg@betterinbox.com>

    SPDX-License-Identifier: MIT
*/

#ifndef DECLARATIVEDRAGAREA_H
#define DECLARATIVEDRAGAREA_H

#include "DeclarativeMimeData.h"

#include <QImage>
#include <QQuickItem>
#include <QSharedPointer>

class QQmlComponent;
class QQuickItemGrabResult;

/*!
 * \qmltype DragArea
 * \inqmlmodule org.kde.draganddrop
 */
class DeclarativeDragArea : public QQuickItem
{
    Q_OBJECT
    QML_NAMED_ELEMENT(DragArea)

    /*!
     * \qmlproperty Item DragArea::delegate
     * The delegate is the item that will be displayed next to the mouse cursor during the drag and drop operation.
     * It usually consists of a large, semi-transparent icon representing the data being dragged.
     */
    Q_PROPERTY(QQuickItem *delegate READ delegate WRITE setDelegate NOTIFY delegateChanged RESET resetDelegate)

    /*!
     * \qmlproperty Item DragArea::source
     * The QML element that is the source of the resulting drag and drop operation. This can be defined to any item, and will
     * be available to the DropArea as event.data.source
     */
    Q_PROPERTY(QQuickItem *source READ source WRITE setSource NOTIFY sourceChanged RESET resetSource)

    // TODO: to be implemented
    /*!
     * \qmlproperty Item DragArea::target
     */
    Q_PROPERTY(QQuickItem *target READ source NOTIFY targetChanged)

    /*!
     * \qmlproperty MimeData DragArea::mimedata
     * the mime data of the drag operation
     * \sa MimeData
     */
    Q_PROPERTY(DeclarativeMimeData *mimeData READ mimeData CONSTANT)

    /*!
     * \qmlproperty bool DragArea::enabled
     * If false no drag operation will be generate
     */
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged) // TODO: Should call setAcceptDrops()

    /*!
     * \qmlproperty Qt::DropActions DragArea::supportedActions
     * Supported operations, a combination of:
     * \list
     * \li Qt.CopyAction
     * \li Qt.MoveAction
     * \li Qt.LinkAction
     * \li Qt.ActionMask
     * \li Qt.IgnoreAction
     * \li Qt.TargetMoveAction
     * \endlist
     */
    Q_PROPERTY(Qt::DropActions supportedActions READ supportedActions WRITE setSupportedActions NOTIFY supportedActionsChanged)

    /*!
     * \qmlproperty Qt::DropAction DragArea::defaultAction
     *
     * The default action will be performed during a drag when no modificators are pressed.
     */
    Q_PROPERTY(Qt::DropAction defaultAction READ defaultAction WRITE setDefaultAction NOTIFY defaultActionChanged)

    /*!
     * \qmlproperty int DragArea::startDragDistance
     * distance in pixel after which a drag event will get started
     */
    Q_PROPERTY(int startDragDistance READ startDragDistance WRITE setStartDragDistance NOTIFY startDragDistanceChanged)

    /*!
     * \qmlproperty var DragArea::delegateImage
     * \brief An image to be used as delegate.
     *
     * If present, it overrides the delegate property.
     *
     * It can be either a QImage or a QIcon.
     */
    Q_PROPERTY(QVariant delegateImage READ delegateImage WRITE setDelegateImage NOTIFY delegateImageChanged)

    /*!
     * \qmlproperty bool DragArea::dragActive
     * Whether a drag currently originates from this drag area.
     *
     * \since 5.19
     */
    Q_PROPERTY(bool dragActive READ dragActive NOTIFY dragActiveChanged)

public:
    DeclarativeDragArea(QQuickItem *parent = nullptr);
    ~DeclarativeDragArea() override;

    QQuickItem *delegate() const;
    void setDelegate(QQuickItem *delegate);
    void resetDelegate();

    QVariant delegateImage() const;
    void setDelegateImage(const QVariant &image);
    QQuickItem *target() const;
    QQuickItem *source() const;
    void setSource(QQuickItem *source);
    void resetSource();

    bool dragActive() const;

    bool isEnabled() const;
    void setEnabled(bool enabled);

    int startDragDistance() const;
    void setStartDragDistance(int distance);

    // supported actions
    Qt::DropActions supportedActions() const;
    void setSupportedActions(Qt::DropActions actions);

    // default action
    Qt::DropAction defaultAction() const;
    void setDefaultAction(Qt::DropAction action);

    DeclarativeMimeData *mimeData() const;

Q_SIGNALS:
    /*! \qmlsignal DragArea::dragStarted() */
    void dragStarted();
    /*! \qmlsignal DragArea::delegateChanged() */
    void delegateChanged();
    /*! \qmlsignal DragArea::dragActiveChanged() */
    void dragActiveChanged();
    /*! \qmlsignal DragArea::sourceChanged() */
    void sourceChanged();
    /*! \qmlsignal DragArea::targetChanged() */
    void targetChanged();
    /*! \qmlsignal DragArea::dataChanged() */
    void dataChanged();
    /*! \qmlsignal DragArea::enabledChanged() */
    void enabledChanged();
    /*! \qmlsignal DragArea::drop(int action) */
    void drop(int action);
    /*! \qmlsignal DragArea::supportedActionsChanged() */
    void supportedActionsChanged();
    /*! \qmlsignal DragArea::defaultActionChanged() */
    void defaultActionChanged();
    /*! \qmlsignal DragArea::startDragDistanceChanged() */
    void startDragDistanceChanged();
    /*! \qmlsignal DragArea::delegateImageChanged() */
    void delegateImageChanged();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void timerEvent(QTimerEvent *event) override;
    bool childMouseEventFilter(QQuickItem *item, QEvent *event) override;

private:
    void startDrag(const QImage &image);

    QQuickItem *m_delegate;
    QQuickItem *m_source;
    QQuickItem *m_target;
    QSharedPointer<QQuickItemGrabResult> m_grabResult;
    bool m_enabled;
    bool m_draggingJustStarted;
    bool m_dragActive;
    Qt::DropActions m_supportedActions;
    Qt::DropAction m_defaultAction;
    DeclarativeMimeData *const m_data;
    QImage m_delegateImage;
    int m_startDragDistance;
    QPointF m_buttonDownPos;
    int m_pressAndHoldTimerId;
};

#endif // DECLARATIVEDRAGAREA_H
