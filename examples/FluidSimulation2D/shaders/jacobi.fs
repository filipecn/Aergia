#version 150 core

in vec2 texCoord;
out float outColor;

struct grid {
    vec2 offset;
    sampler2D m;
};

uniform grid u;
uniform grid v;
uniform grid q;
uniform float dx;
uniform float dt;

vec2 worldCoordinates(grid g, vec2 c){
    return c*dx + g.offset;
}

vec2 textureCoordinates(grid g, vec2 w){
    return (w - g.offset)/dx;
}

void main() {
    vec2 w = worldCoordinates(q, texCoord);
    vec2 pos = w - dt * dx * vec2(texture(u.m, texCoord).r,texture(v.m, texCoord).r);

     half4 xL = h4texRECT(x, coords - half2(1, 0));
     half4 xR = h4texRECT(x, coords + half2(1, 0));
     half4 xB = h4texRECT(x, coords - half2(0, 1));
     half4 xT = h4texRECT(x, coords + half2(0, 1));

      // b sample, from center

       half4 bC = h4texRECT(b, coords);

      // evaluate Jacobi iteration
      xNew = (xL + xR + xB + xT + alpha * bC) * rBeta;

    outColor = texture(q.m, textureCoordinates(q, pos)).r;
}