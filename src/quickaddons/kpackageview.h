/*
 *   Copyright 2015 Marco Martin <mart@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#ifndef VIEW_H
#define VIEW_H

#include <QQuickView>

#include <KPackage/Package>

#include "quickaddons_export.h"

class KPackageViewPrivate;

class QUICKADDONS_EXPORT KPackageView : public QQuickView {
    Q_OBJECT

public:
    KPackageView(const KPackage::Package &package, QWindow *parent = 0);
    KPackageView(const QString &packageName, QWindow *parent = 0);
    virtual ~KPackageView();

    KPackage::Package package() const;

private:
    KPackageViewPrivate *const d;
};

#endif // VIEW_H
