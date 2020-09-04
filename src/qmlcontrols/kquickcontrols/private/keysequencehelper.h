/*
    SPDX-FileCopyrightText: 2014 David Edmundson <davidedmundson@kde.org>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef KEYSEQUENCEHELPER_H
#define KEYSEQUENCEHELPER_H

#include <QQuickItem>
#include <QKeySequence>

/**
 * This class is a clone of Key from XMLGUI
 * It performs only the logic of building shortcuts
 * It is a private class to be used by KeySequenceItem
 *
 */

class KeySequenceHelperPrivate;

class KeySequenceHelper : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(
        QKeySequence keySequence
        READ keySequence
        WRITE setKeySequence
        NOTIFY keySequenceChanged)

    Q_PROPERTY(
        bool multiKeyShortcutsAllowed
        READ multiKeyShortcutsAllowed
        WRITE setMultiKeyShortcutsAllowed)

    Q_PROPERTY(
        QString shortcutDisplay
        READ shortcutDisplay
        NOTIFY shortcutDisplayChanged)

    Q_PROPERTY(
        bool modifierlessAllowed
        READ isModifierlessAllowed
        WRITE setModifierlessAllowed)

    Q_PROPERTY(
        ShortcutTypes checkAgainstShortcutTypes
        READ checkAgainstShortcutTypes
        WRITE setCheckAgainstShortcutTypes
        NOTIFY checkAgainstShortcutTypesChanged
    )

public:

    enum ShortcutType {
        None           = 0x00,      //!< No checking for conflicts
        StandardShortcuts = 0x01,   //!< Check against standard shortcuts. @see KStandardShortcut
        GlobalShortcuts = 0x02      //!< Check against global shortcuts. @see KGlobalAccel
    };
    Q_DECLARE_FLAGS(ShortcutTypes, ShortcutType)
    Q_FLAG(ShortcutTypes)

    /**
    * Constructor.
    */
    explicit KeySequenceHelper(QQuickItem* parent = nullptr);

    /**
    * Destructs the widget.
    */
    virtual ~KeySequenceHelper();

    /**
     * Allow multikey shortcuts?
     */
    void setMultiKeyShortcutsAllowed(bool);
    bool multiKeyShortcutsAllowed() const;

    /**
     * This only applies to user input, not to setShortcut().
     * Set whether to accept "plain" keys without modifiers (like Ctrl, Alt, Meta).
     * Plain keys by our definition include letter and symbol keys and
     * text editing keys (Return, Space, Tab, Backspace, Delete).
     * "Special" keys like F1, Cursor keys, Insert, PageDown will always work.
     */
    void setModifierlessAllowed(bool allow);

    /**
     * @see setModifierlessAllowed()
     */
    bool isModifierlessAllowed();


    bool isRecording() const;
    void setShortcut(bool recording);

    /**
     * Set the default key sequence from a string
     */
    void setKeySequence(const QKeySequence &sequence);

    /**
     * Return the currently selected key sequence as a string
     */
    QKeySequence keySequence() const;
    QString shortcutDisplay() const;

    bool isKeySequenceAvailable(const QKeySequence &keySequence) const;

    ShortcutTypes checkAgainstShortcutTypes();
    void setCheckAgainstShortcutTypes(ShortcutTypes types);

Q_SIGNALS:
    void keySequenceChanged(const QKeySequence &seq);
    void shortcutDisplayChanged(const QString &string);
    void captureFinished();
    void checkAgainstShortcutTypesChanged();

public Q_SLOTS:
    void captureKeySequence();
    void keyPressed(int key, int modifiers);
    void keyReleased(int key, int modifiers);

   /**
    * Clear the key sequence.
    */
    void clearKeySequence();

private Q_SLOTS:
    void doneRecording();

private:
    friend class KeySequenceHelperPrivate;
    KeySequenceHelperPrivate *const d;

    Q_DISABLE_COPY(KeySequenceHelper)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(KeySequenceHelper::ShortcutTypes)


#endif // KEYSEQUENCEHELPER_H
