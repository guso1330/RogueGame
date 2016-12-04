varying vec4 fcolor;
varying vec2 texCoord0;

uniform sampler2D diffuse;

void main() 
{
	//vec4 UVcolor = texture2D(diffuse, texCoord0);
	//vec4 color = fcolor + UVcolor;

	//gl_FragColor = texture2D(diffuse, texCoord0);
	gl_FragColor = fcolor;
} 