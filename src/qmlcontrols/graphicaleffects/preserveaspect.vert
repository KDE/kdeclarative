/*
SPDX-FileCopyrightText: 2021 Arjen Hiemstra <ahiemstra@heimr.nl>

SPDX-License-Identifier: LGPL-2.0-or-later
*/

// This shader performs UV coordinates that account for the aspect ratio of some
// source as specified by the sourceSize uniform. The effective result is that
// this results in behaviour similar to the "PreserveAspectFit" mode of QML Image.

uniform highp mat4 qt_Matrix;

uniform highp vec2 sourceSize;

attribute highp vec4 qt_Vertex;
attribute highp vec2 qt_MultiTexCoord0;

varying highp vec2 qt_TexCoord0;

void main() {
    highp vec2 aspect = sourceSize.x > sourceSize.y ?
                        vec2(1.0, sourceSize.x / sourceSize.y) :
                        vec2(sourceSize.y / sourceSize.x, 1.0);

    qt_TexCoord0 = (aspect - vec2(1.0)) / -2.0 + qt_MultiTexCoord0 * aspect;
    gl_Position = qt_Matrix * qt_Vertex;
}
