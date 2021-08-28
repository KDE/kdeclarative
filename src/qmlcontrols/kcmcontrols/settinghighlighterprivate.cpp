/*
    SPDX-FileCopyrightText: 2020 Kevin Ottens <kevin.ottens@enioka.com>
    SPDX-FileCopyrightText: 2020 Cyril Rossi <cyril.rossi@enioka.com>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "settinghighlighterprivate.h"

#include <QGuiApplication>
#include <QQmlContext>

namespace
{
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
    for (auto child : std::as_const(descendant)) {
        if (className.contains("FontWidget") && itemClassName(child).contains("TextField")) {
            return child->property("background").value<QQuickItem *>();
        }
        if (itemClassName(child).contains("GridViewInternal")) {
            return child;
        }
        if (itemClassName(child).contains("GridView")) {
            return child->property("view").value<QQuickItem *>();
        }
        if (itemClassName(child).contains("CheckIndicator") //
            || itemClassName(child).contains("KQuickStyleItem")) {
            return child;
        }
    }
    return nullptr;
}

} // namespace

QQuickItem *SettingHighlighterPrivate::target() const
{
    return m_target;
}

void SettingHighlighterPrivate::setTarget(QQuickItem *target)
{
    if (m_target == target) {
        return;
    }

    m_target = target;
    Q_EMIT targetChanged();
}

bool SettingHighlighterPrivate::highlight() const
{
    return m_highlight;
}

void SettingHighlighterPrivate::setHighlight(bool highlight)
{
    if (m_highlight == highlight) {
        return;
    }

    m_highlight = highlight;
    updateTarget();
    Q_EMIT highlightChanged();
}

bool SettingHighlighterPrivate::defaultIndicatorVisible() const
{
    return m_enabled;
}

void SettingHighlighterPrivate::setDefaultIndicatorVisible(bool enabled)
{
    if (m_enabled == enabled) {
        return;
    }

    m_enabled = enabled;
    updateTarget();

    Q_EMIT defaultIndicatorVisibleChanged(m_enabled);
}

void SettingHighlighterPrivate::updateTarget()
{
    if (!m_styleTarget) {
        if (!m_target) {
            // parent is SettingStateBinding/SettingHighlighter, use its visual parent as target item.
            const auto *parentItem = qobject_cast<QQuickItem *>(parent());
            if (parentItem) {
                setTarget(parentItem->parentItem());
            }
        }
        if (m_target) {
            m_styleTarget = findStyleItem(m_target);
        }
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
