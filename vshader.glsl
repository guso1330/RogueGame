attribute vec4 vPosition;
attribute vec2 texCoord;

varying vec4 fcolor;
varying vec2 texCoord0;

uniform mat4 model_view;
uniform mat4 projection;
uniform vec4 vcolor;

void main() 
{
   gl_Position = projection*model_view*vPosition;
   fcolor = vcolor;
   texCoord0 = texCoord;
}