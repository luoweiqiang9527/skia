#version 400
const float sk_PrivkGuardedDivideEpsilon = false ? 1e-08 : 0.0;
const float sk_PrivkMinNormalHalf = 6.10351562e-05;
out vec4 sk_FragColor;
uniform vec4 src;
uniform vec4 dst;
float guarded_divide_Qhhh(float n, float d);
float color_burn_component_Qhh2h2(vec2 s, vec2 d);
float guarded_divide_Qhhh(float n, float d) {
    return n / (d + sk_PrivkGuardedDivideEpsilon);
}
float color_burn_component_Qhh2h2(vec2 s, vec2 d) {
    float dyTerm = d.y == d.x ? d.y : 0.0;
    float delta = abs(s.x) >= sk_PrivkMinNormalHalf ? d.y - min(d.y, guarded_divide_Qhhh((d.y - d.x) * s.y, s.x)) : dyTerm;
    return (delta * s.y + s.x * (1.0 - d.y)) + d.x * (1.0 - s.y);
}
void main() {
    sk_FragColor = vec4(color_burn_component_Qhh2h2(src.xw, dst.xw), color_burn_component_Qhh2h2(src.yw, dst.yw), color_burn_component_Qhh2h2(src.zw, dst.zw), src.w + (1.0 - src.w) * dst.w);
}
