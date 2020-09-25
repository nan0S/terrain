#version 330 core

layout(location = 0) in vec2 offset;

// uniform vec3 ePosition;
uniform mat4 vp;

flat out vec3 vColor;
const float earthRadius = 6371;
const float scale = 0.001;

void main()
{
	float lat = offset.x;
	float lon = offset.y;
	float r = earthRadius * scale - 0.001;

	float y = r * sin(lat);
	float x = r * cos(lat) * sin(lon);
	float z = r * cos(lat) * cos(lon);

	vec3 pos = vec3(x, y, z);
	gl_Position = vp * vec4(pos, 1);

	vColor = vec3(0, 1, 0);
}