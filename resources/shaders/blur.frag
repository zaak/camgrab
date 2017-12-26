uniform sampler2D colorTexture;
uniform vec2 resolution;
uniform int renderX;
uniform int renderY;

const float step_w = 0.0015625;
const float step_h = 0.0027778;
// gl_Color, gl_FragCoord

vec4 sharpen(in sampler2D tex, in vec2 coords, in vec2 renderSize) {
  float dx = 1.0 / renderSize.x;
  float dy = 1.0 / renderSize.y;
  vec4 sum = vec4(0.0);
  sum += -1. * texture2D(tex, coords + vec2( -1.0 * dx , 0.0 * dy));
  sum += -1. * texture2D(tex, coords + vec2( 0.0 * dx , -1.0 * dy));
  sum += 5. * texture2D(tex, coords + vec2( 0.0 * dx , 0.0 * dy));
  sum += -1. * texture2D(tex, coords + vec2( 0.0 * dx , 1.0 * dy));
  sum += -1. * texture2D(tex, coords + vec2( 1.0 * dx , 0.0 * dy));
  return sum;
}

void main(void)
{
    vec2 uv = vec2((gl_FragCoord.x - renderX) / (resolution.x), resolution.y - (gl_FragCoord.y + renderY) / resolution.y);
//    vec3 t1 = texture2D(colorTexture, vec2(uv.x - step_w, uv.y - step_h)).rgb;
//    vec3 t2 = texture2D(colorTexture, vec2(uv.x, uv.y - step_h)).rgb;
//    vec3 t3 = texture2D(colorTexture, vec2(uv.x + step_w, uv.y - step_h)).rgb;
//    vec3 t4 = texture2D(colorTexture, vec2(uv.x - step_w, uv.y)).rgb;
//    vec3 t5 = texture2D(colorTexture, uv).rgb;
//    vec3 t6 = texture2D(colorTexture, vec2(uv.x + step_w, uv.y)).rgb;
//    vec3 t7 = texture2D(colorTexture, vec2(uv.x - step_w, uv.y + step_h)).rgb;
//    vec3 t8 = texture2D(colorTexture, vec2(uv.x, uv.y + step_h)).rgb;
//    vec3 t9 = texture2D(colorTexture, vec2(uv.x + step_w, uv.y + step_h)).rgb;
//    vec3 xx = t1 + 2.0*t2 + t3 - t7 - 2.0*t8 - t9;
//    vec3 yy = t1 - t3 + 2.0*t4 - 2.0*t6 + t7 - t9;
//    vec3 rr = sqrt(xx * xx + yy * yy);
//    vec3 col = rr * 2.0 * t5;


    gl_FragColor = sharpen(colorTexture, uv, resolution);
// ok    gl_FragColor = texture2D(colorTexture, vec2((gl_FragCoord.x - renderX) / (resolution.x), resolution.y - (gl_FragCoord.y + renderY) / resolution.y));
}
