/*
SPDX-FileCopyrightText: 2021 Arjen Hiemstra <ahiemstra@heimr.nl>

SPDX-License-Identifier: LGPL-2.0-or-later
*/

// This shader performs UV coordinates that account for the aspect ratio of some
// source as specified by the sourceSize uniform. The effective result is that
// this results in behaviour similar to the "PreserveAspectFit" mode of QML Image.

uniform highp mat4 qt_Matrix;

uniform highp vec2 sourceSize;

in highp vec4 qt_Vertex;
in highp vec2 qt_MultiTexCoord0;

out highp vec2 qt_TexCoord0;

void main() {
    highp float scale = min(targetSize.x / sourceSize.x, targetSize.y / sourceSize.y);

    highp vec2 newSize = sourceSize * scale;
    highp vec2 newOffset = (targetSize - newSize) / 2.0;
    highp vec2 uvOffset = (1.0 / newSize) * newOffset;

    qt_TexCoord0 = -uvOffset + (targetSize / newSize) * qt_MultiTexCoord0;
    gl_Position = qt_Matrix * qt_Vertex;
}
