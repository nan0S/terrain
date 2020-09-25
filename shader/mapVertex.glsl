#version 330 core

layout(location = 0) in mediump int vHeight;
layout(location = 1) in mediump ivec2 vOffset;

uniform ivec2 tOffset;

uniform float rowSize;
uniform float colSize;
uniform float zoom;
uniform vec2 pos;

uniform vec2 mult;

out vec3 vColor;

void main()
{
	vec2 tileOffset = vec2(-1 + colSize * tOffset.y, -1 + rowSize * tOffset.x);
	vec2 inTileOffset = vec2(vOffset.y / 1200. * colSize, ((1 - vOffset.x / 1200.) * rowSize));

	gl_Position = vec4(zoom * (tileOffset + inTileOffset - pos) * mult, 0, 1);

	mediump int ht = vHeight;

	if (ht == -32768) 	vColor = vec3(0, 0, 0); // black
	else if (ht <= 0) 	vColor = vec3(0, 0, 1); // blue
	else if (ht < 500)  vColor = vec3(0, ht / 500. + 0.1, 0); // green
	else if (ht < 1000) vColor = vec3(ht / 500. - 1, 1, 0); // yellow
	else if (ht < 2000) vColor = vec3(1, 2 - ht / 1000., 0); // red
	else	   			vColor = vec3(1, pow(ht / 2000., 3) - 1, pow(ht / 2000., 3) - 1); // white
}