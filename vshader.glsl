attribute vec4 vPosition;
attribute vec2 vUV;
attribute vec4 vNormal;

varying vec4 fcolor;
varying vec2 UV;

uniform mat4 model_view;
uniform mat4 projection;
uniform vec4 vcolor;

void main() 
{
   gl_Position = projection*model_view*vPosition;
   fcolor = vcolor;
   UV = vUV;
} 