#version  330  core

uniform  sampler2D  diffuseTex;
uniform sampler2D bumpTex;
uniform  sampler2D shadowTex;
uniform  int         useTexture;
uniform  vec3    cameraPos;
uniform  vec4    lightColour[2];
uniform  vec3    lightPos[2];
uniform  float   lightRadius[2];
uniform  float   directional[2];

in  Vertex    {
	vec2  texCoord;
	vec4  colour;
	vec3  normal;
	vec3  tangent;
	vec3  binormal;
	vec3  worldPos;
	vec4  shadowProj;
} IN;

out  vec4  fragColour;

void  main(void)    {
	for(int i = 0; i < 2; i++){
		vec3  incident   = normalize(lightPos[i]  - IN.worldPos );
		vec3  viewDir    = normalize(cameraPos  - IN.worldPos );
		vec3  halfDir    = normalize(incident + viewDir );
	
		mat3  TBN         = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal ));
		vec4  diffuse;
		vec3  bumpNormal;
		diffuse = texture(diffuseTex, IN.texCoord);
		bumpNormal = texture(bumpTex, IN.texCoord).rgb;
		bumpNormal   = normalize(TBN * normalize(bumpNormal * 2.0 - 1.0));
		float  lambert          = max(dot(incident , bumpNormal), 0.0f);
		float  distance         = length(lightPos[i]  - IN.worldPos );
		float attenuation = 0.4f;
		if(directional[i] == 0){
		   attenuation     = 1.0f - clamp(distance / lightRadius[i] ,0.0 ,1.0);
		}
	 
		float  specFactor   = clamp(dot(halfDir , bumpNormal ) ,0.0 ,1.0);
		specFactor          = pow(specFactor , 60.0 );
		float  shadow       = 1.0;    //New!
	
		vec3  shadowNDC = IN.shadowProj.xyz / IN.shadowProj.w;
		if(abs(shadowNDC.x) < 1.0f && abs(shadowNDC.y) < 1.0f &&  abs(shadowNDC.z) < 1.0f) {
			float bias = max(0.05 * (1.0 - dot( normalize(IN.normal ), incident)), 0.005);
			vec3  biasCoord = shadowNDC  *bias + 0.5f;
			float  shadowZ   = texture(shadowTex , biasCoord.xy).x;
			if(shadowZ  < biasCoord.z) {
				shadow = 0.0f;
			}
		}
		vec3  surface    = (diffuse.rgb * lightColour[i].rgb);
		vec4 tmp =vec4(surface * lambert * attenuation, 1.0);
		tmp.rgb += (lightColour[i].rgb * specFactor )* attenuation *0.33;
		tmp.rgb *= shadow;
		tmp.rgb += surface * 0.1f; 
		tmp.a = diffuse.a;
		fragColour += tmp;
		fragColour *= IN.colour;
	
	}
}