/*
    This file is part of the KDE project

    SPDX-FileCopyrightText: 2014 Fredrik Höglund <fredrik@kde.org>
    SPDX-FileCopyrightText: 2014 Marco Martin <mart@kde.org>
    SPDX-FileCopyrightText: 2015 David Edmundson <davidedmundson@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "plotter.h"

#include <QOpenGLContext>
#include <QOpenGLShaderProgram>

#include <QPainterPath>
#include <QPolygonF>

#include <QMatrix4x4>
#include <QVector2D>

#include <QSGSimpleTextureNode>
#include <QSGTexture>

#include <QDebug>

#include <math.h>

// completely arbitrary
static int s_defaultSampleSize = 40;

PlotData::PlotData(QObject *parent)
    : QObject(parent)
    , m_min(std::numeric_limits<qreal>::max())
    , m_max(std::numeric_limits<qreal>::min())
    , m_sampleSize(s_defaultSampleSize)
{
    m_values.reserve(s_defaultSampleSize);
    for (int i = 0; i < s_defaultSampleSize; ++i) {
        m_values << 0.0;
    }
}

void PlotData::setColor(const QColor &color)
{
    if (m_color == color) {
        return;
    }

    m_color = color;

    Q_EMIT colorChanged();
}

QColor PlotData::color() const
{
    return m_color;
}

qreal PlotData::max() const
{
    return m_max;
}

qreal PlotData::min() const
{
    return m_min;
}

void PlotData::setSampleSize(int size)
{
    if (m_sampleSize == size) {
        return;
    }

    m_values.reserve(size);
    if (m_values.size() > size) {
        const int numberToRemove = (m_values.size() - size);
        for (int i = 0; i < numberToRemove; ++i) {
            m_values.removeFirst();
        }
    } else if (m_values.size() < size) {
        const int numberToAdd = (size - m_values.size());
        for (int i = 0; i < numberToAdd; ++i) {
            m_values.prepend(0.0);
        }
    }

    m_sampleSize = size;
}

QString PlotData::label() const
{
    return m_label;
}

void PlotData::setLabel(const QString &label)
{
    if (m_label == label) {
        return;
    }

    m_label = label;
    Q_EMIT labelChanged();
}

void PlotData::addSample(qreal value)
{
    // assume at this point we'll have to pop a single time to stay in size
    if (m_values.size() >= m_sampleSize) {
        m_values.removeFirst();
    }

    m_values.push_back(value);

    m_max = std::numeric_limits<qreal>::min();
    m_min = std::numeric_limits<qreal>::max();
    for (auto v : std::as_const(m_values)) {
        if (v > m_max) {
            m_max = v;
        } else if (v < m_min) {
            m_min = v;
        }
    }

    Q_EMIT valuesChanged();
}

QList<qreal> PlotData::values() const
{
    return m_values;
}

const char *vs_source =
    "attribute vec4 vertex;\n"
    "varying float gradient;\n"

    "uniform mat4 matrix;\n"
    "uniform float yMin;\n"
    "uniform float yMax;\n"

    "void main(void) {\n"
    "    gradient = (vertex.y - yMin) / (yMax - yMin);"
    "    gl_Position = matrix * vertex;\n"
    "}";

const char *fs_source =
    "uniform vec4 color1;\n"
    "uniform vec4 color2;\n"

    "varying float gradient;\n"

    "void main(void) {\n"
    "    gl_FragColor = mix(color1, color2, gradient);\n"
    "}";

// --------------------------------------------------

class PlotTexture : public QSGTexture
{
public:
    PlotTexture(QOpenGLContext *ctx);
    ~PlotTexture() override;

    void bind() override final;
    bool hasAlphaChannel() const override final
    {
        return true;
    }
    bool hasMipmaps() const override final
    {
        return false;
    }
    int textureId() const override final
    {
        return m_texture;
    }
    QSize textureSize() const override final
    {
        return m_size;
    }

    void recreate(const QSize &size);
    GLuint fbo() const
    {
        return m_fbo;
    }

private:
    GLuint m_texture = 0;
    GLuint m_fbo = 0;
    GLenum m_internalFormat;
    bool m_haveTexStorage;
    QSize m_size;
};

PlotTexture::PlotTexture(QOpenGLContext *ctx)
    : QSGTexture()
{
    QPair<int, int> version = ctx->format().version();

    if (ctx->isOpenGLES()) {
        m_haveTexStorage = version >= qMakePair(3, 0) || ctx->hasExtension("GL_EXT_texture_storage");
        m_internalFormat = m_haveTexStorage ? GL_RGBA8 : GL_RGBA;
    } else {
        m_haveTexStorage = version >= qMakePair(4, 2) || ctx->hasExtension("GL_ARB_texture_storage");
        m_internalFormat = GL_RGBA8;
    }

    glGenFramebuffers(1, &m_fbo);
}

PlotTexture::~PlotTexture()
{
    if (m_texture) {
        glDeleteTextures(1, &m_texture);
    }

    glDeleteFramebuffers(1, &m_fbo);
}

void PlotTexture::bind()
{
    glBindTexture(GL_TEXTURE_2D, m_texture);
}

void PlotTexture::recreate(const QSize &size)
{
    if (m_texture) {
        glDeleteTextures(1, &m_texture);
    }

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

    if (m_haveTexStorage) {
        glTexStorage2D(GL_TEXTURE_2D, 1, m_internalFormat, size.width(), size.height());
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, size.width(), size.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);

    m_size = size;
}

class PlotSGNode : public QSGSimpleTextureNode
{
public:
    PlotSGNode();
    void bind()
    {
        m_program->bind();
    }
    void setMatrix(const QMatrix4x4 &matrix)
    {
        m_program->setUniformValue(u_matrix, matrix);
    }
    void setColor1(const QColor &color)
    {
        m_program->setUniformValue(u_color1, color);
    }
    void setColor2(const QColor &color)
    {
        m_program->setUniformValue(u_color2, color);
    }
    void setYMin(float min)
    {
        m_program->setUniformValue(u_yMin, min);
    }
    void setYMax(float max)
    {
        m_program->setUniformValue(u_yMax, max);
    }
    ~PlotSGNode() override = default;

private:
    QScopedPointer<QOpenGLShaderProgram> m_program;
    int u_matrix;
    int u_color1;
    int u_color2;
    int u_yMin;
    int u_yMax;
};

PlotSGNode::PlotSGNode()
    : m_program(new QOpenGLShaderProgram)
{
    setOwnsTexture(true);
    m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, vs_source);
    m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, fs_source);
    m_program->bindAttributeLocation("vertex", 0);
    m_program->link();

    u_yMin = m_program->uniformLocation("yMin");
    u_yMax = m_program->uniformLocation("yMax");
    u_color1 = m_program->uniformLocation("color1");
    u_color2 = m_program->uniformLocation("color2");
    u_matrix = m_program->uniformLocation("matrix");
}

// ----------------------

Plotter::Plotter(QQuickItem *parent)
    : QQuickItem(parent)
    , m_min(0)
    , m_max(0)
    , m_rangeMax(0)
    , m_rangeMin(0)
    , m_sampleSize(s_defaultSampleSize)
    , m_horizontalLineCount(5)
    , m_stacked(true)
    , m_autoRange(true)
{
    setFlag(ItemHasContents);
    connect(this, &Plotter::windowChanged, this, [this]() {
        if (m_window) {
            disconnect(m_window.data(), &QQuickWindow::beforeRendering, this, &Plotter::render);
        }
        m_window.clear();
        // when the window changes, the node gets deleted
        m_node = nullptr;
    });
}

Plotter::~Plotter()
{
}

qreal Plotter::max() const
{
    return m_max;
}

qreal Plotter::min() const
{
    return m_min;
}

int Plotter::sampleSize() const
{
    return m_sampleSize;
}

void Plotter::setSampleSize(int size)
{
    if (m_sampleSize == size) {
        return;
    }

    m_sampleSize = size;

    m_mutex.lock();
    for (auto data : std::as_const(m_plotData)) {
        data->setSampleSize(size);
    }
    m_mutex.unlock();

    update();
    Q_EMIT sampleSizeChanged();
}

bool Plotter::isStacked() const
{
    return m_stacked;
}

void Plotter::setStacked(bool stacked)
{
    if (m_stacked == stacked) {
        return;
    }

    m_stacked = stacked;

    Q_EMIT stackedChanged();
    update();
}

bool Plotter::isAutoRange() const
{
    return m_autoRange;
}

void Plotter::setAutoRange(bool autoRange)
{
    if (m_autoRange == autoRange) {
        return;
    }

    m_autoRange = autoRange;

    Q_EMIT autoRangeChanged();
    normalizeData();
    update();
}

qreal Plotter::rangeMax() const
{
    if (m_autoRange) {
        return m_max;
    } else {
        return m_rangeMax;
    }
}

void Plotter::setRangeMax(qreal max)
{
    if (m_rangeMax == max) {
        return;
    }

    m_rangeMax = max;

    Q_EMIT rangeMaxChanged();
    normalizeData();
    update();
}

qreal Plotter::rangeMin() const
{
    if (m_autoRange) {
        return m_min;
    } else {
        return m_rangeMin;
    }
}

void Plotter::setRangeMin(qreal min)
{
    if (m_rangeMin == min) {
        return;
    }

    m_rangeMin = min;

    Q_EMIT rangeMinChanged();
    normalizeData();
    update();
}

void Plotter::setGridColor(const QColor &color)
{
    if (m_gridColor == color) {
        return;
    }

    m_gridColor = color;

    Q_EMIT gridColorChanged();
}

QColor Plotter::gridColor() const
{
    return m_gridColor;
}

int Plotter::horizontalGridLineCount()
{
    return m_horizontalLineCount;
}

void Plotter::setHorizontalGridLineCount(int count)
{
    if (m_horizontalLineCount == count) {
        return;
    }

    m_horizontalLineCount = count;
    Q_EMIT horizontalGridLineCountChanged();
}

void Plotter::addSample(qreal value)
{
    if (m_plotData.count() != 1) {
        qWarning() << "Must add a new value per data set, pass an array of values instead";
        return;
    }

    addSample(QList<qreal>() << value);
}

void Plotter::addSample(const QList<qreal> &value)
{
    if (value.count() != m_plotData.count()) {
        qWarning() << "Must add a new value per data set";
        return;
    }

    int i = 0;
    m_mutex.lock();
    for (auto data : std::as_const(m_plotData)) {
        data->addSample(value.value(i));
        ++i;
    }
    m_mutex.unlock();

    normalizeData();

    update();
}

void Plotter::dataSet_append(QQmlListProperty<PlotData> *list, PlotData *item)
{
    // encase all m_plotData access in a mutex, since rendering is usually done in another thread
    Plotter *p = static_cast<Plotter *>(list->object);
    p->m_mutex.lock();
    p->m_plotData.append(item);
    p->m_mutex.unlock();
}

int Plotter::dataSet_count(QQmlListProperty<PlotData> *list)
{
    Plotter *p = static_cast<Plotter *>(list->object);
    return p->m_plotData.count();
}

PlotData *Plotter::dataSet_at(QQmlListProperty<PlotData> *list, int index)
{
    Plotter *p = static_cast<Plotter *>(list->object);
    p->m_mutex.lock();
    PlotData *d = p->m_plotData.at(index);
    p->m_mutex.unlock();
    return d;
}

void Plotter::dataSet_clear(QQmlListProperty<PlotData> *list)
{
    Plotter *p = static_cast<Plotter *>(list->object);

    p->m_mutex.lock();
    p->m_plotData.clear();
    p->m_mutex.unlock();
}

QQmlListProperty<PlotData> Plotter::dataSets()
{
    return QQmlListProperty<PlotData>(this, nullptr, Plotter::dataSet_append, Plotter::dataSet_count, Plotter::dataSet_at, Plotter::dataSet_clear);
}

// Catmull-Rom interpolation
QPainterPath Plotter::interpolate(const QVector<qreal> &p, qreal x0, qreal x1) const
{
    QPainterPath path;

    /* clang-format off */
    const QMatrix4x4 matrix( 0,    1,    0,     0,
                            -1/6., 1,    1/6.,  0,
                             0,    1/6., 1,    -1/6.,
                             0,    0,    1,     0);
    /* clang-format on */

    const qreal xDelta = (x1 - x0) / (p.count() - 3);
    qreal x = x0 - xDelta;

    path.moveTo(x0, p[0]);

    for (int i = 1; i < p.count() - 2; i++) {
        /* clang-format off */
        const QMatrix4x4 points(x,              p[i-1], 0, 0,
                                x + xDelta * 1, p[i+0], 0, 0,
                                x + xDelta * 2, p[i+1], 0, 0,
                                x + xDelta * 3, p[i+2], 0, 0);

        const QMatrix4x4 res = matrix * points;

        path.cubicTo(res(1, 0), res(1, 1),
                     res(2, 0), res(2, 1),
                     res(3, 0), res(3, 1));
        /* clang-format on */

        x += xDelta;
    }

    return path;
}

void Plotter::render()
{
    if (!window() || !m_node || !m_node->texture()) {
        return;
    }

    GLuint rb;

    if (m_haveMSAA && m_haveFramebufferBlit) {
        // Allocate a temporary MSAA renderbuffer
        glGenRenderbuffers(1, &rb);
        glBindRenderbuffer(GL_RENDERBUFFER, rb);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, m_internalFormat, width(), height());

        // Attach it to the framebuffer object
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rb);
    } else {
        // If we don't have MSAA support we render directly into the texture
        glBindFramebuffer(GL_FRAMEBUFFER, static_cast<PlotTexture *>(m_node->texture())->fbo());
    }

    glViewport(0, 0, width(), height());

    // Clear the color buffer
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Add horizontal lines
    qreal lineSpacing = height() / m_horizontalLineCount;

    QVector<QVector2D> vertices;

    // don't draw the bottom line that will come later
    for (int i = 0; i < m_horizontalLineCount; i++) {
        int lineY = ceil(i * lineSpacing) + 1; // floor +1 makes the entry at point 0 on pixel 1
        vertices << QVector2D(0, lineY) << QVector2D(width(), lineY);
    }
    // bottom line
    vertices << QVector2D(0, height() - 1) << QVector2D(width(), height() - 1);

    // Tessellate
    float min = height();
    float max = height();

    QHash<PlotData *, QPair<int, int>> verticesCounts;

    // encase all m_plotData access in a mutex, since rendering is usually done in another thread
    m_mutex.lock();
    int roundedHeight = qRound(height());
    int roundedWidth = qRound(width());

    for (auto data : std::as_const(m_plotData)) {
        // Interpolate the data set
        const QPainterPath path = interpolate(data->m_normalizedValues, 0, roundedWidth);

        // Flatten the path
        const QList<QPolygonF> polygons = path.toSubpathPolygons();

        for (const QPolygonF &p : polygons) {
            verticesCounts[data].first = 0;
            vertices << QVector2D(p.first().x(), roundedHeight);

            for (int i = 0; i < p.count() - 1; i++) {
                min = qMin<float>(min, roundedHeight - p[i].y());
                vertices << QVector2D(p[i].x(), roundedHeight - p[i].y());
                vertices << QVector2D((p[i].x() + p[i + 1].x()) / 2.0, roundedHeight);
                verticesCounts[data].first += 2;
            }

            min = qMin<float>(min, roundedHeight - p.last().y());
            vertices << QVector2D(p.last().x(), roundedHeight - p.last().y());
            vertices << QVector2D(p.last().x(), roundedHeight);
            verticesCounts[data].first += 3;
        }

        for (const QPolygonF &p : polygons) {
            verticesCounts[data].second = 0;

            for (int i = 0; i < p.count() - 1; i++) {
                min = qMin<float>(min, roundedHeight - p[i].y());
                vertices << QVector2D(p[i].x(), roundedHeight - p[i].y());
                verticesCounts[data].second += 1;
            }

            vertices << QVector2D(p.last().x(), roundedHeight - p.last().y());
            verticesCounts[data].second += 1;
            min = qMin<float>(min, roundedHeight - p.last().y());
        }
    }
    m_mutex.unlock();

    // Upload vertices
    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.count() * sizeof(QVector2D), vertices.constData(), GL_STATIC_DRAW);

    // Set up the array
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(QVector2D), nullptr);
    glEnableVertexAttribArray(0);

    // Bind the shader program
    m_node->bind();
    m_node->setMatrix(m_matrix);

    // Draw the lines
    QColor color1 = m_gridColor;
    QColor color2 = m_gridColor;
    color1.setAlphaF(0.10);
    color2.setAlphaF(0.40);
    m_node->setYMin((float)0.0);
    m_node->setYMax((float)height());
    m_node->setColor1(color1);
    m_node->setColor2(color2);

    glDrawArrays(GL_LINES, 0, (m_horizontalLineCount + 1) * 2);

    // Enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    QPair<int, int> oldCount;
    m_mutex.lock();
    for (auto data : std::as_const(m_plotData)) {
        color2 = data->color();
        color2.setAlphaF(0.60);
        // Draw the graph
        m_node->setYMin(min);
        m_node->setYMax(max);
        m_node->setColor1(data->color());
        m_node->setColor2(color2);

        //+2 is for the bottom line
        glDrawArrays(GL_TRIANGLE_STRIP, m_horizontalLineCount * 2 + 2 + oldCount.first + oldCount.second, verticesCounts[data].first);

        oldCount.first += verticesCounts[data].first;

        m_node->setColor1(data->color());
        m_node->setColor2(data->color());
        glDrawArrays(GL_LINE_STRIP, m_horizontalLineCount * 2 + 2 + oldCount.first + oldCount.second, verticesCounts[data].second);

        oldCount.second += verticesCounts[data].second;
    }
    m_mutex.unlock();

    glDisable(GL_BLEND);

    m_node->setColor1(m_gridColor);
    m_node->setColor2(m_gridColor);
    glDrawArrays(GL_LINES, vertices.count() - 2, 2);

    if (m_haveMSAA && m_haveFramebufferBlit) {
        // Resolve the MSAA buffer
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, static_cast<PlotTexture *>(m_node->texture())->fbo());
        glBlitFramebuffer(0, 0, width(), height(), 0, 0, width(), height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);

        // Delete the render buffer
        glDeleteRenderbuffers(1, &rb);
    }

    // Delete the VBO
    glDeleteBuffers(1, &vbo);
}

QSGNode *Plotter::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *updatePaintNodeData)
{
    Q_UNUSED(updatePaintNodeData)
    if (!window()->openglContext()) {
        delete oldNode;
        return nullptr;
    }

    PlotSGNode *n = static_cast<PlotSGNode *>(oldNode);

    if (width() == 0 && height() == 0) {
        delete oldNode;
        return nullptr;
    }

    if (!n) {
        n = new PlotSGNode();
        n->setTexture(new PlotTexture(window()->openglContext()));
        n->setFiltering(QSGTexture::Linear);

        m_node = n;
        if (m_window) {
            disconnect(m_window.data(), &QQuickWindow::beforeRendering, this, &Plotter::render);
        }
        connect(window(), &QQuickWindow::beforeRendering, this, &Plotter::render, Qt::DirectConnection);
        m_window = window();
    }

    if (!m_initialized) {
        glGenFramebuffers(1, &m_fbo);

        QOpenGLContext *ctx = window()->openglContext();
        QPair<int, int> version = ctx->format().version();

        if (ctx->isOpenGLES()) {
            m_haveMSAA = version >= qMakePair(3, 0) || ctx->hasExtension("GL_NV_framebuffer_multisample");
            m_haveFramebufferBlit = version >= qMakePair(3, 0) || ctx->hasExtension("GL_NV_framebuffer_blit");
            m_haveInternalFormatQuery = version >= qMakePair(3, 0);
            m_internalFormat = version >= qMakePair(3, 0) ? GL_RGBA8 : GL_RGBA;
        } else {
            /* clang-format off */
            m_haveMSAA = version >= qMakePair(3, 2)
                         || ctx->hasExtension("GL_ARB_framebuffer_object")
                         || ctx->hasExtension("GL_EXT_framebuffer_multisample");

            m_haveFramebufferBlit = version >= qMakePair(3, 0)
                                    || ctx->hasExtension("GL_ARB_framebuffer_object")
                                    || ctx->hasExtension("GL_EXT_framebuffer_blit");
            /* clang-format on */

            m_haveInternalFormatQuery = version >= qMakePair(4, 2) || ctx->hasExtension("GL_ARB_internalformat_query");
            m_internalFormat = GL_RGBA8;
        }

        // Query the maximum sample count for the internal format
        if (m_haveInternalFormatQuery) {
            int count = 0;
            glGetInternalformativ(GL_RENDERBUFFER, m_internalFormat, GL_NUM_SAMPLE_COUNTS, 1, &count);

            if (count > 0) {
                QVector<int> samples(count);
                glGetInternalformativ(GL_RENDERBUFFER, m_internalFormat, GL_SAMPLES, count, samples.data());

                // The samples are returned in descending order. Choose the highest value.
                m_samples = samples.at(0);
            } else {
                m_samples = 0;
            }
        } else if (m_haveMSAA) {
            glGetIntegerv(GL_MAX_SAMPLES, &m_samples);
        } else {
            m_samples = 0;
        }

        m_initialized = true;
    }

    // we need a size always equal or smaller, size.toSize() won't do
    const QSize targetTextureSize(qRound(boundingRect().size().width()), qRound(boundingRect().size().height()));
    if (n->texture()->textureSize() != targetTextureSize) {
        static_cast<PlotTexture *>(n->texture())->recreate(targetTextureSize);
        m_matrix = QMatrix4x4();
        m_matrix.ortho(0, targetTextureSize.width(), 0, targetTextureSize.height(), -1, 1);
    }

    n->setRect(QRect(QPoint(0, 0), targetTextureSize));
    return n;
}

void Plotter::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
    normalizeData();
}

void Plotter::normalizeData()
{
    if (m_plotData.isEmpty()) {
        return;
    }
    // normalize data
    m_max = std::numeric_limits<qreal>::min();
    m_min = std::numeric_limits<qreal>::max();
    qreal adjustedMax = m_max;
    qreal adjustedMin = m_min;
    m_mutex.lock();
    if (m_stacked) {
        PlotData *previousData = nullptr;
        auto i = m_plotData.constEnd();
        do {
            --i;
            PlotData *data = *i;
            data->m_normalizedValues.clear();
            data->m_normalizedValues.resize(data->values().count());
            if (previousData) {
                for (int i = 0; i < data->values().count(); ++i) {
                    data->m_normalizedValues[i] = data->values().value(i) + previousData->m_normalizedValues.value(i);

                    if (data->m_normalizedValues[i] > adjustedMax) {
                        adjustedMax = data->m_normalizedValues[i];
                    }
                    if (data->m_normalizedValues[i] < adjustedMin) {
                        adjustedMin = data->m_normalizedValues[i];
                    }
                }
            } else {
                data->m_normalizedValues = data->values().toVector();
                if (data->max() > adjustedMax) {
                    adjustedMax = data->max();
                }
                if (data->min() < adjustedMin) {
                    adjustedMin = data->min();
                }
            }
            previousData = data;

            // global max and global min
            if (data->max() > m_max) {
                m_max = data->max();
            }
            if (data->min() < m_min) {
                m_min = data->min();
            }
        } while (i != m_plotData.constBegin());

    } else {
        for (auto data : std::as_const(m_plotData)) {
            data->m_normalizedValues.clear();
            data->m_normalizedValues = data->values().toVector();
            // global max and global min
            if (data->max() > m_max) {
                adjustedMax = m_max = data->max();
            }
            if (data->min() < m_min) {
                adjustedMin = m_min = data->min();
            }
        }
    }
    m_mutex.unlock();

    if (adjustedMin > 0.0 && adjustedMax > 0.0) {
        adjustedMin = 0.0;
    }

    if (adjustedMin < 0.0 && adjustedMax < 0.0) {
        adjustedMax = 0.0;
    }

    if (m_autoRange || m_rangeMax > m_rangeMin) {
        if (!m_autoRange) {
            adjustedMax = m_rangeMax;
            adjustedMin = m_rangeMin;
        }

        qreal adjust;
        // this should never happen, remove?
        if (qFuzzyCompare(adjustedMax - adjustedMin, std::numeric_limits<qreal>::min())) {
            adjust = 1;
        } else {
            adjust = (height() / (adjustedMax - adjustedMin));
        }

        // normalizebased on global max and min
        m_mutex.lock();
        for (auto data : std::as_const(m_plotData)) {
            for (int i = 0; i < data->values().count(); ++i) {
                data->m_normalizedValues[i] = (data->m_normalizedValues.value(i) - adjustedMin) * adjust;
            }
        }
        m_mutex.unlock();
    }
}
