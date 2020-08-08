/*
    SPDX-FileCopyrightText: 2014 Bhushan Shah <bhush94@gmail.com>

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#ifndef FORMATS_H
#define FORMATS_H

#include <KFormat>

class Formats : public QObject
{
    Q_OBJECT

    public:

	explicit Formats (QObject* parent = nullptr);

	/**
	* Converts size from bytes to the appropriate string representation
	*/
	Q_INVOKABLE QString formatByteSize(double size, int precision = 1) const;

	/**
	 * Given a number of milliseconds, converts that to a string containing
	 * the localized equivalent, e.g. 1:23:45
	 */
    Q_INVOKABLE QString formatDuration(quint64 msecs, KFormat::DurationFormatOptions options = KFormat::DefaultDuration) const;

    Q_DECLARE_FLAGS(DurationFormatOptions, KFormat::DurationFormatOption)

    /**
     * This overload exists so it can be called from QML, which does
     * not support calling Q_INVOKABLEs with Q_ENUMS from different classes
     *
     * This is mentioned in the docs and also in https://bugreports.qt.io/browse/QTBUG-20639
     * Until that bug is fixed, we'll need this
     */
    Q_INVOKABLE QString formatDuration(quint64 msecs, int options) const;

	/**
	 * Given a number of milliseconds, converts that to a string containing
	 * the localized equivalent to the requested decimal places.
	 *
	 * e.g. given formatDuration(60000), returns "1.0 minutes"
	 */
	Q_INVOKABLE QString formatDecimalDuration(quint64 msecs, int decimalPlaces = 2) const;

	/**
	 * Given a number of milliseconds, converts that to a spell-out string containing
	 * the localized equivalent.
	 *
	 * e.g. given formatSpelloutDuration(60001) returns "1 minute"
	 *      given formatSpelloutDuration(62005) returns "1 minute and 2 seconds"
	 *      given formatSpelloutDuration(90060000) returns "1 day and 1 hour"
	 *
	 * Units not interesting to the user, for example seconds or minutes when the first
	 * unit is day, are not returned because they are irrelevant. The same applies for
	 * seconds when the first unit is hour.
	 *
	 */
	Q_INVOKABLE QString formatSpelloutDuration(quint64 msecs) const;

	/**
	 * Returns a string formatted to a relative date style.
	 *
	 * If the date falls within one week before or after the current date
	 * then a relative date string will be returned, such as:
	 * * Yesterday
	 * * Today
	 * * Tomorrow
	 * * Last Tuesday
	 * * Next Wednesday
	 *
	 * If the date falls outside this period then the format is used
	 */
	Q_INVOKABLE QString formatRelativeDate(const QDate &date, QLocale::FormatType format) const;

	/**
	 * Returns a string formatted to a relative datetime style.
	 *
	 * If the dateTime falls within one week before or after the current date
	 * then a relative date string will be returned, such as:
	 * * Yesterday, 3:00pm
	 * * Today, 3:00pm
	 * * Tomorrow, 3:00pm
	 * * Last Tuesday, 3:00pm
	 * * Next Wednesday, 3:00pm
	 *
	 * If the datetime falls outside this period then the format is used
	 */
	Q_INVOKABLE QString formatRelativeDateTime(const QDateTime &dateTime, QLocale::FormatType format) const;

    private:

	KFormat m_format;
};

Q_DECLARE_METATYPE(QLocale::FormatType)

#endif
