varying vec4 fcolor;
varying vec2 UV;

uniform sampler2D diffuse;

void main() 
{
	vec4 UVcolor = texture2D(diffuse, UV);
	vec4 color = fcolor + UVcolor;

	gl_FragColor = color;
	// gl_FragColor = fcolor;
} 