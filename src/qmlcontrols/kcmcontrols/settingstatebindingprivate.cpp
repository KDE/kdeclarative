/*
    SPDX-FileCopyrightText: 2020 Kevin Ottens <kevin.ottens@enioka.com>
    SPDX-FileCopyrightText: 2020 Cyril Rossi <cyril.rossi@enioka.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "settingstatebindingprivate.h"

#include <QGuiApplication>
#include <QQmlContext>

namespace {

QByteArray itemClassName(QQuickItem *item)
{
    // Split since some exported types will be of the form: Foo_QMLTYPE_XX
    const auto className = QByteArray(item->metaObject()->className()).split('_').first();
    return className;
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

QQuickItem *findStyleItem(QQuickItem *item)
{
    const auto className = itemClassName(item);

    auto descendant = findDescendantItems(item);
    for (auto child : qAsConst(descendant)) {
        if (className.contains("FontWidget") && itemClassName(child).contains("TextField")) {
            return child->property("background").value<QQuickItem*>();
        }
        if (itemClassName(child).contains("GridViewInternal")) {
            return child;
        }
        if (itemClassName(child).contains("GridView")) {
            return child->property("view").value<QQuickItem*>();
        }
        if (itemClassName(child).contains("CheckIndicator")
         || itemClassName(child).contains("KQuickStyleItem")) {
            return child;
        }
    }
    return nullptr;
}

} // namespace

QQuickItem *SettingStateBindingPrivate::target() const
{
    return m_target;
}

void SettingStateBindingPrivate::setTarget(QQuickItem *target)
{
    if (m_target == target) {
        return;
    }

    m_target = target;
    emit targetChanged();
}

bool SettingStateBindingPrivate::highlight() const
{
    return m_highlight;
}

void SettingStateBindingPrivate::setHighlight(bool highlight)
{
    if (m_highlight == highlight) {
        return;
    }

    m_highlight = highlight;
    updateTarget();
    emit highlightChanged();
}

bool SettingStateBindingPrivate::defaultIndicatorVisible() const
{
    return m_enabled;
}

void SettingStateBindingPrivate::setDefaultIndicatorVisible(bool enabled)
{
    if (m_enabled == enabled) {
        return;
    }

    m_enabled = enabled;
    updateTarget();

    emit defaultIndicatorVisibleChanged(m_enabled);
}

void SettingStateBindingPrivate::updateTarget()
{
    if (!m_styleTarget) {
        if (!m_target) {
            // parent is SettingStateBinding, use SettingStateBinding's visual parent as target item.
            const auto *parentItem = qobject_cast<QQuickItem*>(parent());
            if (parentItem) {
                setTarget(parentItem->parentItem());
            }
        }
        m_styleTarget = findStyleItem(m_target);
    }

    if (m_styleTarget) {
        if (itemClassName(m_styleTarget).contains("GridViewInternal")) {
            m_styleTarget->setProperty("neutralHighlight", m_highlight && m_enabled);
        } else {
            m_styleTarget->setProperty("_kde_highlight_neutral", m_highlight && m_enabled);
        }
        m_styleTarget->polish();
    }
}
