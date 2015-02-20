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

#include "kpackageview.h"

#include <QDebug>
#include <QQmlContext>
#include <QStandardPaths>

#include <kdeclarative/kdeclarative.h>
#include <KPackage/PackageLoader>


class KPackageViewPrivate
{
public:
    KPackageViewPrivate(const KPackage::Package& p)
        : package(p)
    {}
    KPackageViewPrivate()
    {}

    KPackage::Package package;
    QString packageName;
};

KPackageView::KPackageView(const KPackage::Package &package, QWindow *parent)
    : QQuickView(parent),
      d(new KPackageViewPrivate(package))
{

    KDeclarative::KDeclarative kdeclarative;
    kdeclarative.setDeclarativeEngine(engine());
    kdeclarative.setupBindings();

    if (d->package.isValid() && d->package.metadata().isValid()) {
        setTitle(d->package.metadata().name());
        setIcon(QIcon::fromTheme(d->package.metadata().iconName()));
        setSource(QUrl::fromLocalFile(d->package.filePath("mainscript")));
    } else {
        qWarning() << "Error: package not valid";
    }

    setResizeMode(QQuickView::SizeRootObjectToView);
}

KPackageView::KPackageView(const QString &packageName, QWindow *parent)
    : QQuickView(parent),
      d(new KPackageViewPrivate)
{
    d->packageName = packageName;
}

KPackageView::~KPackageView()
{
    delete d;
}

void KPackageView::init()
{
    KDeclarative::KDeclarative kdeclarative;
    kdeclarative.setDeclarativeEngine(engine());
    kdeclarative.setupBindings();

    if (!d->package.isValid()) {
        d->package = KPackage::PackageLoader::self()->loadPackage("KPackage/GenericQML");
        d->package.setPath(d->packageName);
    }

    if (d->package.isValid() && d->package.metadata().isValid()) {
        setTitle(d->package.metadata().name());
        setIcon(QIcon::fromTheme(d->package.metadata().iconName()));
        setSource(QUrl::fromLocalFile(d->package.filePath("mainscript")));
    } else {
        qWarning() << "Error: package not valid";
    }

    setResizeMode(QQuickView::SizeRootObjectToView);
}

void KPackageView::showEvent(QShowEvent *event)
{
    init();
    QQuickView::showEvent(event);
}

KPackage::Package KPackageView::package() const
{
    return d->package;
}

#include "moc_kpackageview.cpp"
