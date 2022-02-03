#version  330  core

uniform  sampler2D  diffuseTex;
uniform  vec3    cameraPos;
uniform  vec4    lightColour[4];
uniform  vec3    lightPos[4];
uniform  float   lightRadius[4];

in  Vertex {
	vec3  colour;
	vec2  texCoord;
	vec3  normal;
	vec3  worldPos;
} IN;

out  vec4  fragColour;
void  main(void)    {

	for (int i = 0; i < 4; ++i){

		vec3  incident   = normalize(lightPos[i]  - IN.worldPos );
		vec3  viewDir    = normalize(cameraPos  - IN.worldPos );
		vec3  halfDir    = normalize(incident + viewDir );
	
		vec4  diffuse    = texture(diffuseTex , IN.texCoord );

		float  lambert      = max(dot(incident , IN.normal), 0.0f);
		float  distance     = length(lightPos[i]  - IN.worldPos );
		float  attenuation = 1.0 - clamp(distance / lightRadius[i] , 0.0,  1.0);
		float  specFactor   = clamp(dot(halfDir , IN.normal ) ,0.0 ,1.0);
		specFactor    = pow(specFactor , 60.0 );
		vec3  surface = (diffuse.rgb * lightColour[i].rgb);
		/*fragColour[i].rgb = surface * lambert * attenuation;
		fragColour[i].rgb += (lightColour[i].rgb * specFactor )* attenuation *0.33;
		fragColour[i].rgb +=  surface * 0.1f;    // ambient!
		fragColour[i].a = diffuse.a;*/

		vec4 tmp =vec4(surface * lambert * attenuation, 1.0);
		tmp.rgb += (lightColour[i].rgb * specFactor )* attenuation *0.33;
		tmp.rgb += surface * 0.1f; 
		tmp.a = diffuse.a;

		fragColour += tmp;
	}
}