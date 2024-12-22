/*
    SPDX-FileCopyrightText: 2014 David Edmundson <davidedmundson@kde.org>
    SPDX-FileCopyrightText: 2020 David Redondo <kde@david-redondo.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef KEYSEQUENCEHELPER_H
#define KEYSEQUENCEHELPER_H

#include <KKeySequenceRecorder>

#include <QKeySequence>
#include <QQuickItem>

#include "enums.h"

class KeySequenceHelperPrivate;
class QQuickWindow;

class KeySequenceHelper : public KKeySequenceRecorder
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(KeySequenceEnums::ShortcutTypes checkAgainstShortcutTypes READ checkAgainstShortcutTypes WRITE setCheckAgainstShortcutTypes NOTIFY
                   checkAgainstShortcutTypesChanged)

public:
    /**
     * Constructor.
     */
    explicit KeySequenceHelper(QObject *parent = nullptr);

    /**
     * Destructs the widget.
     */
    ~KeySequenceHelper() override;

    Q_INVOKABLE void updateKeySequence(const QKeySequence &keySequence);

    Q_INVOKABLE void showQuestionDialog(const QString &title, const QString &text);
    Q_INVOKABLE void showErrorDialog(const QString &title, const QString &text);

    KeySequenceEnums::ShortcutTypes checkAgainstShortcutTypes();
    void setCheckAgainstShortcutTypes(KeySequenceEnums::ShortcutTypes types);

    Q_INVOKABLE static bool keySequenceIsEmpty(const QKeySequence &keySequence);
    Q_INVOKABLE static QString keySequenceNativeText(const QKeySequence &keySequence);
    Q_INVOKABLE static QWindow *renderWindow(QQuickWindow *quickWindow);

Q_SIGNALS:
    void checkAgainstShortcutTypesChanged();
    void questionDialogAccepted();
    void questionDialogRejected();

private:
    friend class KeySequenceHelperPrivate;
    KeySequenceHelperPrivate *const d;

    Q_DISABLE_COPY(KeySequenceHelper)
};

#endif // KEYSEQUENCEHELPER_H
