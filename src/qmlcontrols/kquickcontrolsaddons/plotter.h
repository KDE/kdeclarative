/*
    This file is part of the KDE project

    SPDX-FileCopyrightText: 2014 Fredrik Höglund <fredrik@kde.org>
    SPDX-FileCopyrightText: 2014 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef PLASMA_PLOTTER_H
#define PLASMA_PLOTTER_H

#include <epoxy/gl.h>

// qopengl.h declares GLdouble as a typedef of float when Qt is built
// with GLES support.  This conflicts with the epoxy/gl_generated.h
// declaration, so we have to prevent the Qt header from being #included.
#define QOPENGL_H

#ifndef QOPENGLF_APIENTRY
#define QOPENGLF_APIENTRY GLAPIENTRY
#endif

#ifndef QOPENGLF_APIENTRYP
#define QOPENGLF_APIENTRYP GLAPIENTRYP
#endif

#include <QMutex>
#include <QPointer>
#include <QQmlListProperty>
#include <QQuickItem>
#include <QQuickWindow>

class PlotSGNode;

/**
 * a Plotter can draw a graph of values arriving from an arbitrary number of data sources
 * to show their evolution in time.
 * an example can be a plot of the network transfer speed or CPU temperature over time.
 * Multiple plots can be fitted in the same graph, either stacked or intersected.
 */
class PlotData : public QObject
{
    Q_OBJECT
    /**
     * text Label of the data set: note this is purely a model, it will need a Label somewhere to be actually painted
     */
    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)

    /**
     * Color to plot this data set
     */
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

    /**
     * All the values currently in this data set
     */
    Q_PROPERTY(QList<qreal> values READ values NOTIFY valuesChanged)

    /**
     * Maximum value of this data set
     */
    Q_PROPERTY(qreal max READ max NOTIFY maxChanged)

    /**
     * Minimum value of this data set
     */
    Q_PROPERTY(qreal min READ min NOTIFY minChanged)

public:
    PlotData(QObject *parent = nullptr);

    void setColor(const QColor &color);
    QColor color() const;

    void addSample(qreal value);

    QList<qreal> values() const;

    QVector<qreal> m_normalizedValues;

    qreal max() const;
    qreal min() const;

    void setSampleSize(int size);

    QString label() const;
    void setLabel(const QString &label);

Q_SIGNALS:
    void colorChanged();
    void valuesChanged();
    void maxChanged();
    void minChanged();
    void labelChanged();

private:
    QString m_label;
    QColor m_color;
    QList<qreal> m_values;

    qreal m_min;
    qreal m_max;
    int m_sampleSize;
};

class Plotter : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<PlotData> dataSets READ dataSets)

    /**
     * maximum value among all graphs
     */
    Q_PROPERTY(qreal max READ max NOTIFY maxChanged)

    /**
     * minimum value among all graphs
     */
    Q_PROPERTY(qreal min READ min NOTIFY minChanged)

    /**
     * draw at most n samples, if new samples are pushed old values are started to be thrown away
     */
    Q_PROPERTY(int sampleSize READ sampleSize WRITE setSampleSize NOTIFY sampleSizeChanged)

    /**
     * if true stack the graphs one on top of each other instead of just painting one on top of each other
     */
    Q_PROPERTY(bool stacked READ isStacked WRITE setStacked NOTIFY stackedChanged)

    /**
     * If true, the graph is automatically scaled to always fit in the Plotter area
     */
    Q_PROPERTY(bool autoRange READ isAutoRange WRITE setAutoRange NOTIFY autoRangeChanged)

    /**
     * Always represents the values scaled between this and rangeMin
     * If autoRange is true, this property just holds the minimum value
     */
    Q_PROPERTY(qreal rangeMax READ rangeMax WRITE setRangeMax NOTIFY rangeMaxChanged)

    /**
     * Always represents the values scaled between this and rangeMax
     * If autoRange is true, this property just holds the maximum value
     */
    Q_PROPERTY(qreal rangeMin READ rangeMin WRITE setRangeMin NOTIFY rangeMinChanged)

    /**
     * Color of the grid lines
     */
    Q_PROPERTY(QColor gridColor READ gridColor WRITE setGridColor NOTIFY gridColorChanged)

    /**
     * The number of horizontal lines drawn across the view between 0 and rangeMax at the top of the plotter at rangeMax
     * This does not include the bottom line
     * Setting this to 0 will disable grid lines
     *
     * The default value is 5
     */
    Q_PROPERTY(int horizontalGridLineCount READ horizontalGridLineCount WRITE setHorizontalGridLineCount NOTIFY horizontalGridLineCountChanged)

    // Q_CLASSINFO("DefaultProperty", "dataSets")

public:
    Plotter(QQuickItem *parent = nullptr);
    ~Plotter() override;

    qreal max() const;
    qreal min() const;

    int sampleSize() const;
    void setSampleSize(int size);

    bool isStacked() const;
    void setStacked(bool stacked);

    bool isAutoRange() const;
    void setAutoRange(bool autorange);

    qreal rangeMax() const;
    void setRangeMax(qreal max);

    qreal rangeMin() const;
    void setRangeMin(qreal min);

    void setHorizontalGridLineCount(int count);
    int horizontalGridLineCount();

    void setGridColor(const QColor &color);
    QColor gridColor() const;

    QQmlListProperty<PlotData> dataSets();
    static void dataSet_append(QQmlListProperty<PlotData> *list, PlotData *item);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    static int dataSet_count(QQmlListProperty<PlotData> *list);
    static PlotData *dataSet_at(QQmlListProperty<PlotData> *list, int pos);
#else
    static qsizetype dataSet_count(QQmlListProperty<PlotData> *list);
    static PlotData *dataSet_at(QQmlListProperty<PlotData> *list, qsizetype pos);
#endif
    static void dataSet_clear(QQmlListProperty<PlotData> *list);

    Q_INVOKABLE void addSample(qreal value);
    Q_INVOKABLE void addSample(const QList<qreal> &value);

protected:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
#else
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
#endif

private:
    QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *updatePaintNodeData) override final;
    QPainterPath interpolate(const QVector<qreal> &p, qreal x0, qreal x1) const;
    void normalizeData();

Q_SIGNALS:
    void maxChanged();
    void minChanged();
    void sampleSizeChanged();
    void stackedChanged();
    void autoRangeChanged();
    void rangeMaxChanged();
    void rangeMinChanged();
    void gridColorChanged();
    void horizontalGridLineCountChanged();

private Q_SLOTS:
    void render();

private:
    QList<PlotData *> m_plotData;

    GLuint m_fbo = 0;
    PlotSGNode *m_node = nullptr;
    qreal m_min;
    qreal m_max;
    qreal m_rangeMax;
    qreal m_rangeMin;
    int m_sampleSize;
    int m_horizontalLineCount;
    bool m_stacked;
    bool m_autoRange;
    QColor m_gridColor;

    QMatrix4x4 m_matrix;
    bool m_initialized = false;
    bool m_haveMSAA;
    bool m_haveFramebufferBlit;
    bool m_haveInternalFormatQuery;
    GLenum m_internalFormat;
    int m_samples;
    QPointer<QQuickWindow> m_window;
    QMutex m_mutex;
};

#endif
