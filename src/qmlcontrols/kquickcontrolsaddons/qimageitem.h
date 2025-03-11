/*
    SPDX-FileCopyrightText: 2011 Marco Martin <mart@kde.org>
    SPDX-FileCopyrightText: 2015 Luca Beltrame <lbeltrame@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef QIMAGEITEM_H
#define QIMAGEITEM_H

#include <QImage>
#include <QQuickPaintedItem>

/*!
 * \qmltype QImageItem
 * \inqmlmodule org.kde.kquickcontrols.addons
 */
class QImageItem : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT

    /*! \qmlproperty QImage QImageItem::image */
    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged RESET resetImage)
    /*! \qmlproperty int QImageItem::nativeWidth */
    Q_PROPERTY(int nativeWidth READ nativeWidth NOTIFY nativeWidthChanged)
    /*! \qmlproperty int QImageItem::nativeHeight */
    Q_PROPERTY(int nativeHeight READ nativeHeight NOTIFY nativeHeightChanged)
    /*! \qmlproperty int QImageItem::paintedWidth */
    Q_PROPERTY(int paintedWidth READ paintedWidth NOTIFY paintedWidthChanged)
    /*! \qmlproperty int QImageItem::paintedHeight */
    Q_PROPERTY(int paintedHeight READ paintedHeight NOTIFY paintedHeightChanged)
    /*! \qmlproperty FillMode QImageItem::fillMode
     *
     * \value Stretch
     *        The image is scaled to fit.
     * \value PreserveAspectFit
     *        the image is scaled uniformly to fit without cropping.
     * \value PreserveAspectCrop
     *        The image is scaled uniformly to fill, cropping if necessary.
     * \value Tile
     *        The image is duplicated horizontally and vertically.
     * \value TileVertically
     *        The image is stretched horizontally and tiled vertically.
     * \value TileHorizontally
     *        The image is stretched vertically and tiled horizontally.
     * \value [since 5.96] Pad
     *        The image is not transformed.
     *
     */
    Q_PROPERTY(FillMode fillMode READ fillMode WRITE setFillMode NOTIFY fillModeChanged)
    /*! \qmlproperty bool QImageItem::null */
    Q_PROPERTY(bool null READ isNull NOTIFY nullChanged)

public:
    enum FillMode {
        Stretch,
        PreserveAspectFit,
        PreserveAspectCrop,
        Tile,
        TileVertically,
        TileHorizontally,
        Pad,
    };
    Q_ENUM(FillMode)

    explicit QImageItem(QQuickItem *parent = nullptr);
    ~QImageItem() override;

    void setImage(const QImage &image);
    QImage image() const;
    void resetImage();

    int nativeWidth() const;
    int nativeHeight() const;

    int paintedWidth() const;
    int paintedHeight() const;

    FillMode fillMode() const;
    void setFillMode(FillMode mode);

    void paint(QPainter *painter) override;

    bool isNull() const;

Q_SIGNALS:
    /*! \qmlsignal QImageItem::nativeWidthChanged() */
    void nativeWidthChanged();
    /*! \qmlsignal QImageItem::nativeHeightChanged() */
    void nativeHeightChanged();
    /*! \qmlsignal QImageItem::fillModeChanged() */
    void fillModeChanged();
    /*! \qmlsignal QImageItem::imageChanged() */
    void imageChanged();
    /*! \qmlsignal QImageItem::nullChanged() */
    void nullChanged();
    /*! \qmlsignal QImageItem::paintedWidthChanged() */
    void paintedWidthChanged();
    /*! \qmlsignal QImageItem::paintedHeightChanged() */
    void paintedHeightChanged();

protected:
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;

private:
    QImage m_image;
    FillMode m_fillMode;
    QRect m_paintedRect;

private Q_SLOTS:
    void updatePaintedRect();
};

#endif
