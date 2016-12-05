/***************************
	Used Quentin's Code
***************************/

attribute vec4 vPosition;
attribute vec2 vUV;
attribute vec4 vNormal;

varying vec4 fcolor;
varying vec2 UV;
varying vec4 posWorldSpace;
varying vec4 normalCameraSpace;
varying vec4 eyeDirectionCameraSpace;
varying vec4 lightDirectionCameraSpace;

uniform mat4 model_view;
uniform mat4 projection;
uniform vec4 vcolor;
uniform vec4 lightPositionWorldSpace;

void main() 
{
	gl_Position = projection*model_view*vPosition;

	posWorldSpace= model_view * vPosition;

	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec4 vertexPositionCameraSpace = projection * model_view * vPosition;
	eyeDirectionCameraSpace = vec4(0, 0, 0, 0) - vertexPositionCameraSpace;
  
	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec4 lightPositionCameraSpace = projection * lightPositionWorldSpace;
	lightDirectionCameraSpace = lightPositionCameraSpace + eyeDirectionCameraSpace;

	normalCameraSpace = projection * model_view * vNormal;

	fcolor = vcolor;
	UV = vUV;
} 