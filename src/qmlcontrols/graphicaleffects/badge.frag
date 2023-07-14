/*
 *  SPDX-FileCopyrightText: 2023 Mike Noe <noeerover@gmail.com>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

#version 440

layout(location = 0) in vec2 texcoord;
layout(location = 0) out vec4 fragColor; 

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
} ubuf;
           
layout(binding = 1) uniform sampler2D source;
layout(binding = 2) uniform sampler2D mask;

void main() {
    fragColor = texture(source, texcoord) * (1.0 - (texture(mask, texcoord).a)) * ubuf.qt_Opacity;
}

