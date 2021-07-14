#version 150 core

/*
    SPDX-FileCopyrightText: 2021 Arjen Hiemstra <ahiemstra@heimr.nl>

    Hyllian's lanczos windowed-jinc 2-lobe sharper 3D with anti-ringing Shader

    Copyright (C) 2011/2016 Hyllian - sergiogdb@gmail.com

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

uniform sampler2D source;
uniform highp vec2 targetSize;

uniform highp float windowSinc;
uniform highp float sinc;

uniform highp float antiRingingStrength;
uniform highp float resolution;

uniform highp float qt_Opacity;

in highp vec2 qt_TexCoord0;
out highp vec4 out_color;

// A=0.5, B=0.825 is the best jinc approximation for x<2.5. if B=1.0, it's a lanczos filter.
// Increase A to get more blur. Decrease it to get a sharper picture.
// B = 0.825 to get rid of dithering. Increase B to get a fine sharpness, though dithering returns.

#define wa (windowSinc * pi)
#define wb (sinc * pi)

const highp float pi = 3.1415926535897932384626433832795;
const highp vec3 dtt = vec3(65536.0, 255.0, 1.0);

highp vec4 reduce4(highp vec3 A, highp vec3 B, highp vec3 C, highp vec3 D)
{
    return dtt * mat4x3(A, B, C, D);
}

highp vec3 min4(highp vec3 a, highp vec3 b, highp vec3 c, highp vec3 d)
{
    return min(a, min(b, min(c, d)));
}

highp vec3 max4(highp vec3 a, highp vec3 b, highp vec3 c, highp vec3 d)
{
    return max(a, max(b, max(c, d)));
}

highp vec4 lanczos(highp vec4 x)
{
    return (x == vec4(0.0)) ?  vec4(wa * wb) : sin(x * wa) * sin(x * wb) / (x * x);
}

void main()
{
    // Discard any pixels that are outside the bounds of the texture.
    // This prevents artifacts when the texture doesn't have a full-alpha border.
    if (any(lessThan(qt_TexCoord0, vec2(0.0))) || any(greaterThan(qt_TexCoord0, vec2(1.0)))) {
        discard;
    }

    highp vec2 dx = vec2(1.0, 0.0);
    highp vec2 dy = vec2(0.0, 1.0);

    highp vec2 pixelCoord = qt_TexCoord0 * targetSize / resolution;
    highp vec2 texel = (floor(pixelCoord) + vec2(0.5, 0.5)) * resolution / targetSize;

    highp vec2 texelCenter = (floor(pixelCoord - vec2(0.5, 0.5)) + vec2(0.5, 0.5));

    highp mat4 weights;
    weights[0] = lanczos(vec4(distance(pixelCoord, texelCenter - dx - dy),
                              distance(pixelCoord, texelCenter - dy),
                              distance(pixelCoord, texelCenter + dx - dy),
                              distance(pixelCoord, texelCenter + 2.0 * dx - dy)));
    weights[1] = lanczos(vec4(distance(pixelCoord, texelCenter - dx),
                              distance(pixelCoord, texelCenter),
                              distance(pixelCoord, texelCenter + dx),
                              distance(pixelCoord, texelCenter + 2.0 * dx)));
    weights[2] = lanczos(vec4(distance(pixelCoord, texelCenter - dx + dy),
                              distance(pixelCoord, texelCenter + dy),
                              distance(pixelCoord, texelCenter + dx + dy),
                              distance(pixelCoord, texelCenter + 2.0 * dx + dy)));
    weights[3] = lanczos(vec4(distance(pixelCoord, texelCenter - dx + 2.0 * dy),
                              distance(pixelCoord, texelCenter + 2.0 * dy),
                              distance(pixelCoord, texelCenter + dx + 2.0 * dy),
                              distance(pixelCoord, texelCenter + 2.0 * dx + 2.0 * dy)));

    dx = dx * resolution / targetSize;
    dy = dy * resolution / targetSize;
    texelCenter = texelCenter * resolution / targetSize;

    // reading the texels
    highp vec3 color = texture(source, qt_TexCoord0).xyz;

    highp vec3 c00 = texture(source, texelCenter - dx - dy).xyz;
    highp vec3 c10 = texture(source, texelCenter - dy).xyz;
    highp vec3 c20 = texture(source, texelCenter + dx - dy).xyz;
    highp vec3 c30 = texture(source, texelCenter + 2.0 * dx - dy).xyz;
    highp vec3 c01 = texture(source, texelCenter - dx).xyz;
    highp vec3 c11 = texture(source, texelCenter).xyz;
    highp vec3 c21 = texture(source, texelCenter + dx).xyz;
    highp vec3 c31 = texture(source, texelCenter + 2.0 * dx).xyz;
    highp vec3 c02 = texture(source, texelCenter - dx + dy).xyz;
    highp vec3 c12 = texture(source, texelCenter + dy).xyz;
    highp vec3 c22 = texture(source, texelCenter + dx + dy).xyz;
    highp vec3 c32 = texture(source, texelCenter + 2.0 * dx + dy).xyz;
    highp vec3 c03 = texture(source, texelCenter - dx + 2.0 * dy).xyz;
    highp vec3 c13 = texture(source, texelCenter + 2.0 * dy).xyz;
    highp vec3 c23 = texture(source, texelCenter + dx + 2.0 * dy).xyz;
    highp vec3 c33 = texture(source, texelCenter + 2.0 * dx + 2.0 * dy).xyz;

    highp vec3 F6 = texture(source, texel + dx + 0.25 * dx + 0.25 * dy).xyz;
    highp vec3 F7 = texture(source, texel + dx + 0.25 * dx - 0.25 * dy).xyz;
    highp vec3 F8 = texture(source, texel + dx - 0.25 * dx - 0.25 * dy).xyz;
    highp vec3 F9 = texture(source, texel + dx - 0.25 * dx + 0.25 * dy).xyz;

    highp vec3 H6 = texture(source, texel + 0.25 * dx + 0.25 * dy + dy).xyz;
    highp vec3 H7 = texture(source, texel + 0.25 * dx - 0.25 * dy + dy).xyz;
    highp vec3 H8 = texture(source, texel - 0.25 * dx - 0.25 * dy + dy).xyz;
    highp vec3 H9 = texture(source, texel - 0.25 * dx + 0.25 * dy + dy).xyz;

    highp vec4 f0 = reduce4(F6, F7, F8, F9);
    highp vec4 h0 = reduce4(H6, H7, H8, H9);

    //  Get min/max samples
    highp vec3 min_sample = min4(c11, c21, c12, c22);
    highp vec3 max_sample = max4(c11, c21, c12, c22);

    color = weights[0] * transpose(mat4x3(c00, c10, c20, c30));
    color += weights[1] * transpose(mat4x3(c01, c11, c21, c31));
    color += weights[2] * transpose(mat4x3(c02, c12, c22, c32));
    color += weights[3] * transpose(mat4x3(c03, c13, c23, c33));
    color = color / dot(vec4(1.0) * weights, vec4(1.0));

    // Anti-ringing
    highp vec3 aux = color;
    color = clamp(color, min_sample, max_sample);

    color = mix(aux, color, antiRingingStrength);

    float alpha = texture(source, qt_TexCoord0).a * qt_Opacity;
    out_color = vec4(color, alpha);
}
