/*
    SPDX-FileCopyrightText: 2011 Marco Martin <mart@kde.org>
    SPDX-FileCopyrightText: 2015 Luca Beltrame <lbeltrame@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef QPIXMAPITEM_H
#define QPIXMAPITEM_H

#include <QPixmap>
#include <QQuickPaintedItem>

/*!
 * \qmltype QPixMapItem
 * \inqmlmodule org.kde.kquickcontrols.addons
 */
class QPixmapItem : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT

    /*! \qmlproperty QPixMap QPixMapItem::pixmap */
    Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap NOTIFY pixmapChanged RESET resetPixmap)
    /*! \qmlproperty int QPixMapItem::nativeWidth */
    Q_PROPERTY(int nativeWidth READ nativeWidth NOTIFY nativeWidthChanged)
    /*! \qmlproperty int QPixMapItem::nativeHeight */
    Q_PROPERTY(int nativeHeight READ nativeHeight NOTIFY nativeHeightChanged)
    /*! \qmlproperty int QPixMapItem::paintedWidth */
    Q_PROPERTY(int paintedWidth READ paintedWidth NOTIFY paintedWidthChanged)
    /*! \qmlproperty int QPixMapItem::paintedHeight */
    Q_PROPERTY(int paintedHeight READ paintedHeight NOTIFY paintedHeightChanged)
    /*! \qmlproperty FillMode QPixMapItem::fillMode
     *  \value Stretch
     *         The image is scaled to fit.
     *  \value PreserveAspectFit
     *         the image is scaled uniformly to fit without cropping.
     *  \value PreserveAspectCrop
     *         The image is scaled uniformly to fill, cropping if necessary.
     *  \value Tile
     *         The image is duplicated horizontally and vertically.
     *  \value TileVertically
     *         The image is stretched horizontally and tiled vertically.
     *  \value TileHorizontally
     *         The image is stretched vertically and tiled horizontally.
     */
    Q_PROPERTY(FillMode fillMode READ fillMode WRITE setFillMode NOTIFY fillModeChanged)
    /*! \qmlproperty bool QPixMapItem::null */
    Q_PROPERTY(bool null READ isNull NOTIFY nullChanged)

public:
    enum FillMode {
        Stretch,
        PreserveAspectFit,
        PreserveAspectCrop,
        Tile,
        TileVertically,
        TileHorizontally,
    };
    Q_ENUM(FillMode)

    explicit QPixmapItem(QQuickItem *parent = nullptr);
    ~QPixmapItem() override;

    void setPixmap(const QPixmap &pixmap);
    QPixmap pixmap() const;
    void resetPixmap();

    int nativeWidth() const;
    int nativeHeight() const;

    int paintedWidth() const;
    int paintedHeight() const;

    FillMode fillMode() const;
    void setFillMode(FillMode mode);

    void paint(QPainter *painter) override;

    bool isNull() const;

Q_SIGNALS:
    /*! \qmlsignal QPixMapItem::nativeWidthChanged */
    void nativeWidthChanged();
    /*! \qmlsignal QPixMapItem::nativeHeightChanged */
    void nativeHeightChanged();
    /*! \qmlsignal QPixMapItem::fillModeChanged */
    void fillModeChanged();
    /*! \qmlsignal QPixMapItem::pixmapChanged */
    void pixmapChanged();
    /*! \qmlsignal QPixMapItem::nullChanged */
    void nullChanged();
    /*! \qmlsignal QPixMapItem::paintedWidthChanged */
    void paintedWidthChanged();
    /*! \qmlsignal QPixMapItem::paintedHeightChanged */
    void paintedHeightChanged();

protected:
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;

private:
    QPixmap m_pixmap;
    FillMode m_fillMode;
    QRect m_paintedRect;

private Q_SLOTS:
    void updatePaintedRect();
};

#endif
