/*
 *    Copyright 2011 Marco Martin <mart@kde.org>
 *    Copyright 2015 Luca Beltrame <lbeltrame@kde.org>
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

#ifndef QPIXMAPITEM_H
#define QPIXMAPITEM_H

#include <QQuickPaintedItem>
#include <QPixmap>

class QPixmapItem : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap NOTIFY pixmapChanged RESET resetPixmap)
    Q_PROPERTY(bool smooth READ smooth WRITE setSmooth)
    Q_PROPERTY(int nativeWidth READ nativeWidth NOTIFY nativeWidthChanged)
    Q_PROPERTY(int nativeHeight READ nativeHeight NOTIFY nativeHeightChanged)
    Q_PROPERTY(int paintedWidth READ paintedWidth NOTIFY paintedWidthChanged)
    Q_PROPERTY(int paintedHeight READ paintedHeight NOTIFY paintedHeightChanged)
    Q_PROPERTY(FillMode fillMode READ fillMode WRITE setFillMode NOTIFY fillModeChanged)
    Q_PROPERTY(bool null READ isNull NOTIFY nullChanged)

public:
    enum FillMode {
        Stretch, // the image is scaled to fit
        PreserveAspectFit, // the image is scaled uniformly to fit without cropping
        PreserveAspectCrop, // the image is scaled uniformly to fill, cropping if necessary
        Tile, // the image is duplicated horizontally and vertically
        TileVertically, // the image is stretched horizontally and tiled vertically
        TileHorizontally //the image is stretched vertically and tiled horizontally
    };
    Q_ENUM(FillMode)

    QPixmapItem(QQuickItem *parent=nullptr);
    ~QPixmapItem();

    void setPixmap(const QPixmap &pixmap);
    QPixmap pixmap() const;
    void resetPixmap();

    void setSmooth(const bool smooth);
    bool smooth() const;

    int nativeWidth() const;
    int nativeHeight() const;

    int paintedWidth() const;
    int paintedHeight() const;

    FillMode fillMode() const;
    void setFillMode(FillMode mode);

    void paint(QPainter *painter) Q_DECL_OVERRIDE;

    bool isNull() const;

Q_SIGNALS:
    void nativeWidthChanged();
    void nativeHeightChanged();
    void fillModeChanged();
    void pixmapChanged();
    void nullChanged();
    void paintedWidthChanged();
    void paintedHeightChanged();

protected:
    void geometryChanged(const QRectF & newGeometry, const QRectF & oldGeometry) Q_DECL_OVERRIDE;

private:
    QPixmap m_pixmap;
    bool m_smooth;
    FillMode m_fillMode;
    QRect m_paintedRect;

private Q_SLOTS:
    void updatePaintedRect();

};

#endif
