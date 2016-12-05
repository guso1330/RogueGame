// OLD VARIABLES
varying vec4 fcolor;
varying vec2 UV;
uniform sampler2D diffuse;

// NEW VARIABLES
varying vec4 posWorldSpace;
varying vec4 normalCameraSpace;
varying vec4 eyeDirectionCameraSpace;
varying vec4 lightDirectionCameraSpace;

uniform mat4 projection;
uniform mat4 model_view;
uniform vec4 lightPositionWorldSpace;

void main() 
{

	vec4 LightColor = vec4(0.8f, 0.3f, 0.1f, 1.0f);
	float LightPower = 50.0f;

	// Material properties
	vec4 MaterialDiffuseColor = texture2D(diffuse, UV);
	vec4 MaterialAmbientColor = vec4(0.2, 0.2, 0.1, 1.0) * MaterialDiffuseColor;
	vec4 MaterialSpecularColor = vec4(0.1, 0.1, 0.1, 1.0);

	// Distance to the light
	float distance = length(lightPositionWorldSpace - posWorldSpace);

	// Normal of the computed fragment, in camera space
	vec4 n = normalize(normalCameraSpace);
	// Direction of the light (from the fragment to the light)
	vec4 l = normalize(lightDirectionCameraSpace);
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp( dot( n,l ), 0.0, 1.0);

	// Eye vector (towards the camera)
	vec4 E = normalize(eyeDirectionCameraSpace);
	// Direction in which the triangle reflects the light
	vec4 R = reflect(-l,n);

	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp( dot( E,R ), 0,1 );

	vec4 color =
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse : "color" of the object
		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
		// Specular : reflective highlight, like a mirror
		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);

	gl_FragColor = color + fcolor;



// OLD STUFF
	// vec4 UVcolor = texture2D(diffuse, UV);
	// vec4 color = fcolor + UVcolor;

	// gl_FragColor = color;
	// gl_FragColor = fcolor;
} 