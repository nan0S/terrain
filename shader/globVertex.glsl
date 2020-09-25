#version 330 core

layout(location = 0) in mediump int vHeight;
layout(location = 1) in mediump ivec2 vOffset;

uniform ivec2 tOffset;
uniform mat4 vp;
uniform ivec2 mini;

out vec3 vColor;

const float earthRadius = 6371;
const float scale = 0.001;

void main()
{
	float lat = radians(tOffset.x + mini.x + ((1200 - vOffset.x) / 1200.));
	float lon = radians(tOffset.y + mini.y + vOffset.y / 1200.);	
	float r = (earthRadius + vHeight / 1000.) * scale;

	float y = sin(lat);
	float x = cos(lat) * sin(lon);
	float z = cos(lat) * cos(lon);

	vec3 pos = vec3(x, y, z);
	gl_Position = vp * vec4(r * pos, 1);

	mediump int ht = vHeight;

	if (ht == -32768) 	vColor = vec3(0, 0, 0); // black
	else if (ht <= 0) 	vColor = vec3(0, 0, 1); // blue
	else if (ht < 500)  vColor = vec3(0, ht / 500. + 0.1, 0); // green
	else if (ht < 1000) vColor = vec3(ht / 500. - 1, 1, 0); // yellow
	else if (ht < 2000) vColor = vec3(1, 2 - ht / 1000., 0); // red
	else	   			vColor = vec3(1, pow(ht / 2000., 3) - 1, pow(ht / 2000., 3) - 1); // white
}