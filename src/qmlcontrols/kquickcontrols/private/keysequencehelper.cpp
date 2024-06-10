/*
    SPDX-FileCopyrightText: 2020 David Redondo <kde@david-redondo.de>
    SPDX-FileCopyrightText: 2014 David Edmundson <davidedmundson@kde.org>
    SPDX-FileCopyrightText: 1998 Mark Donohoe <donohoe@kde.org>
    SPDX-FileCopyrightText: 2001 Ellis Whitehead <ellis@kde.org>
    SPDX-FileCopyrightText: 2007 Andreas Hartmetz <ahartmetz@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "keysequencehelper.h"

#include <QDebug>
#include <QHash>
#include <QPointer>
#include <QQmlEngine>
#include <QQuickRenderControl>
#include <QQuickWindow>

#include <KLocalizedString>
#include <KStandardShortcut>

#include <config-kdeclarative.h>
#if HAVE_KGLOBALACCEL
#include <KGlobalAccel>
#include <KGlobalShortcutInfo>
#endif

class KeySequenceHelperPrivate
{
public:
    KeySequenceHelperPrivate(KeySequenceHelper *qq);

    // members
    KeySequenceHelper *const q;

    //! Check the key sequence against KStandardShortcut::find()
    KeySequenceHelper::ShortcutTypes checkAgainstShortcutTypes;
};

KeySequenceHelperPrivate::KeySequenceHelperPrivate(KeySequenceHelper *qq)
    : q(qq)
    , checkAgainstShortcutTypes(KeySequenceHelper::StandardShortcuts | KeySequenceHelper::GlobalShortcuts)
{
}

KeySequenceHelper::KeySequenceHelper(QObject *parent)
    : KKeySequenceRecorder(nullptr, parent)
    , d(new KeySequenceHelperPrivate(this))
{
}

KeySequenceHelper::~KeySequenceHelper()
{
    delete d;
}

void KeySequenceHelper::updateKeySequence(const QKeySequence &keySequence)
{
#if HAVE_KGLOBALACCEL
    if (d->checkAgainstShortcutTypes.testFlag(GlobalShortcuts)) {
        KGlobalAccel::stealShortcutSystemwide(keySequence);
    }
#endif

    setCurrentKeySequence(keySequence);
}

KeySequenceHelper::ShortcutTypes KeySequenceHelper::checkAgainstShortcutTypes()
{
    return d->checkAgainstShortcutTypes;
}

void KeySequenceHelper::setCheckAgainstShortcutTypes(KeySequenceHelper::ShortcutTypes types)
{
    if (d->checkAgainstShortcutTypes != types) {
        d->checkAgainstShortcutTypes = types;
    }
    Q_EMIT checkAgainstShortcutTypesChanged();
}

bool KeySequenceHelper::keySequenceIsEmpty(const QKeySequence &keySequence)
{
    return keySequence.isEmpty();
}

QString KeySequenceHelper::keySequenceNativeText(const QKeySequence &keySequence)
{
    return keySequence.toString(QKeySequence::NativeText);
}

QWindow *KeySequenceHelper::renderWindow(QQuickWindow *quickWindow)
{
    QWindow *renderWindow = QQuickRenderControl::renderWindowFor(quickWindow);
    auto window = renderWindow ? renderWindow : quickWindow;
    // If we have CppOwnership, set it explicitly to prevent the engine taking ownership of the window
    // and crashing on teardown
    if (QQmlEngine::objectOwnership(window) == QQmlEngine::CppOwnership) {
        QQmlEngine::setObjectOwnership(window, QQmlEngine::CppOwnership);
    }
    return window;
}

#include "moc_keysequencehelper.cpp"
