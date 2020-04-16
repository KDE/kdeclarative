/*
 *    Copyright 2020 Kevin Ottens <kevin.ottens@enioka.com>
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

#include "settingstatebindingprivate.h"

#include <QGuiApplication>
#include <QQmlContext>

namespace {
constexpr const char *bindingProperty = "__SettingStateBindingPrivate_binding";
constexpr const char *hasBindingProperty = "__SettingStateBindingPrivate_hasBinding";

QByteArray itemClassName(QQuickItem *item)
{
    // Split since some exported types will be of the form: Foo_QMLTYPE_XX
    const auto className = QByteArray(item->metaObject()->className()).split('_').first();
    return className;
}

bool isLayout(QQuickItem *item)
{
    const auto className = itemClassName(item);
    // We pretend StackLayout to not be a layout so that
    // later on the positioning of one tab isn't polluted
    // by the elemends from another tab.
    if (className.endsWith("StackLayout")) {
        return false;
    } else {
        return className.endsWith("Layout")
            || className.endsWith("Column")
            || className.endsWith("Row")
            || className.endsWith("Grid");
    }
}

bool isView(QQuickItem *item)
{
    const auto className = itemClassName(item);
    return className.contains("GridView")
        || className.endsWith("ListView");
}

bool hasNonOverlayIndicator(QQuickItem *item)
{
    if (!item->property(hasBindingProperty).toBool()) {
        return false;
    }

    const auto binding = item->property(bindingProperty).value<SettingStateBindingPrivate *>();
    return !binding->isIndicatorAsOverlay();
}

QList<QQuickItem *> findDescendantItems(QQuickItem *item)
{
    const auto children = item->childItems();
    auto result = children;
    for (auto child : children) {
        result += findDescendantItems(child);
    }
    return result;
}

QQuickItem *findOuterLayout(QQuickItem *item)
{
    if (!item->parentItem()) {
        return item;
    } else if (isLayout(item) && !isLayout(item->parentItem())) {
        return item;
    } else {
        return findOuterLayout(item->parentItem());
    }
}

QQuickItem *findOuterView(QQuickItem *item)
{
    // This is mostly useful for situations where
    // the target is a GridViewInternal for some
    // reason but really what we need is finding the GridView
    if (!isView(item)) {
        return nullptr;
    } else {
        const auto parentView = findOuterView(item->parentItem());
        if (!parentView) {
            return item;
        } else {
            return parentView;
        }
    }
}

}

QQuickItem *SettingStateBindingPrivate::target() const
{
    return m_target;
}

void SettingStateBindingPrivate::setTarget(QQuickItem *target)
{
    if (m_target == target) {
        return;
    }

    if (m_target) {
        m_target->setProperty(hasBindingProperty, QVariant());
        m_target->setProperty(bindingProperty, QVariant());
        disconnect(m_target);

        const auto outerView = findOuterView(m_target);
        if (outerView) {
            disconnect(outerView);
        }
    }

    m_target = target;
    if (m_target) {
        m_target->setProperty(hasBindingProperty, true);
        m_target->setProperty(bindingProperty, QVariant::fromValue(this));
        connect(m_target, &QQuickItem::parentChanged, this, &SettingStateBindingPrivate::triggerCoordChanges);
        connect(m_target, &QQuickItem::widthChanged, this, &SettingStateBindingPrivate::triggerCoordChanges);
        connect(m_target, &QQuickItem::heightChanged, this, &SettingStateBindingPrivate::triggerCoordChanges);
        connect(m_target, &QQuickItem::xChanged, this, &SettingStateBindingPrivate::triggerCoordChanges);
        connect(m_target, &QQuickItem::yChanged, this, &SettingStateBindingPrivate::triggerCoordChanges);

        // For indicators which are in overlay mode, they need to change position
        // when scrollbars are shown/hidden
        const auto outerView = findOuterView(m_target);
        const auto view = outerView? outerView : m_target.data();
        connect(view, &QQuickItem::visibleChildrenChanged, this, &SettingStateBindingPrivate::triggerCoordChanges);
    }

    emit targetChanged();
    triggerCoordChanges();
}

QQuickItem *SettingStateBindingPrivate::indicator() const
{
    return m_indicator;
}

void SettingStateBindingPrivate::setIndicator(QQuickItem *indicator)
{
    if (m_indicator == indicator) {
        return;
    }

    m_indicator = indicator;

    emit indicatorChanged();
    triggerCoordChanges();
}

bool SettingStateBindingPrivate::isIndicatorAsOverlay() const
{
    return m_indicatorAsOverlay;
}

void SettingStateBindingPrivate::setIndicatorAsOverlay(bool asOverlay)
{
    if (m_indicatorAsOverlay == asOverlay) {
        return;
    }

    m_indicatorAsOverlay = asOverlay;

    emit indicatorAsOverlayChanged();
    triggerCoordChanges();
}

qreal SettingStateBindingPrivate::leftCoord() const
{
    if (!m_target || !m_indicator) {
        return 0.0;
    }

    if (m_indicatorAsOverlay) {
        return overlayLeftCoord();
    } else {
        return formLeftCoord();
    }
}

qreal SettingStateBindingPrivate::formLeftCoord() const
{
    const auto parent = m_indicator->parentItem();
    const auto outerLayout = findOuterLayout(m_target);

    const auto siblings = [=] {
        auto result = findDescendantItems(outerLayout);
        result.erase(std::remove_if(result.begin(), result.end(),
                                    [=](QQuickItem *item) {
                                        return !hasNonOverlayIndicator(item);
                                    }),
                     result.end());
        return result;
    }();

    const auto leftToRight = QGuiApplication::layoutDirection() == Qt::LeftToRight;
    const auto xValues = [=] {
        auto result = QVector<qreal>();
        result.reserve(siblings.size());
        std::transform(siblings.cbegin(), siblings.cend(),
                       std::back_inserter(result),
                       [=](QQuickItem *item) {
                           const auto localX = leftToRight ? item->width() : -m_indicator->width();
                           return parent->mapFromItem(item, {localX, 0.0}).x();
                       });
        return result;
    }();
    if (leftToRight) {
        return *std::max_element(xValues.cbegin(), xValues.cend());
    } else {
        return *std::min_element(xValues.cbegin(), xValues.cend());
    }
}

qreal SettingStateBindingPrivate::overlayLeftCoord() const
{
    const auto parent = m_indicator->parentItem();
    const auto leftToRight = QGuiApplication::layoutDirection() == Qt::LeftToRight;

    const auto localX = leftToRight ? m_target->width() : -m_indicator->width();
    const auto parentX = parent->mapFromItem(m_target, {localX, 0.0}).x();

    const auto outerView = findOuterView(m_target);

    // Heuristic to find the vertical scroll bars which are visible and which
    // might overlap with the indicator if left on the board of a view
    const auto verticalScrollbars = [=] {
        auto result = findDescendantItems(outerView? outerView : m_target.data());
        result.erase(std::remove_if(result.begin(), result.end(),
                                    [=](QQuickItem *item) {
                                        if (!item->isVisible()
                                         || itemClassName(item) != "ScrollBar"
                                         || item->width() > item->height()) {
                                            return true;
                                        }

                                        const auto localSbX = leftToRight ? item->width() : -m_indicator->width();
                                        const auto parentSbX = parent->mapFromItem(item, {localSbX, 0.0}).x();
                                        return qAbs(parentSbX - parentX) > m_indicator->width();
                                    }),
                     result.end());
        return result;
    }();
    Q_ASSERT(verticalScrollbars.size() <= 1);

    const auto scrollbarWidth = verticalScrollbars.isEmpty() ? 0.0 : verticalScrollbars.first()->width();
    return parentX + (leftToRight? -1.0 : 1.0) * scrollbarWidth;
}

void SettingStateBindingPrivate::triggerCoordChanges()
{
    const auto outerLayout = findOuterLayout(m_target);
    const auto items = findDescendantItems(outerLayout);

    for (auto item : items) {
        const auto binding = item->property(bindingProperty).value<SettingStateBindingPrivate *>();
        if (binding) {
            emit binding->leftCoordChanged();
        }
    }
}
