#version  330  core
uniform  sampler2D  diffuseTex;
uniform  sampler2D  bumpTex; //New!
uniform sampler2D sandTex;
uniform sampler2D grassTex;
uniform sampler2D sandBump;
uniform sampler2D grassBump;
uniform  sampler2D shadowTex;
uniform  vec3    cameraPos;

uniform  vec4    lightColour[2];
uniform  vec3    lightPos[2];
uniform  float   lightRadius[2];
uniform  float   directional[2];

in  Vertex {
	vec4  colour;
	vec2  texCoord;
	vec3  normal;
	vec3  tangent;   //New!
	vec3  binormal; //New!
	vec3  worldPos;
	vec4  shadowProj;
} IN;

out  vec4  fragColour;

void  main(void)    {
	for(int i = 0; i < 2; i++){
		vec3  incident   = normalize(lightPos[i]  - IN.worldPos );
		vec3  viewDir    = normalize(cameraPos  - IN.worldPos );
		vec3  halfDir    = normalize(incident + viewDir );
		int offset = 350;
		vec2 sandLayer = vec2(1, 1600);
		vec2 grassLayer = vec2(sandLayer.y + offset, sandLayer.y + offset * 2);
		vec2 stoneLayer = vec2(grassLayer.y + offset, grassLayer.y + offset *2);

		mat3  TBN         = mat3(normalize(IN.tangent), normalize(IN.binormal), normalize(IN.normal ));
		vec4  diffuse;
		vec3  bumpNormal;
		if(IN.worldPos.y < sandLayer.y) {
			diffuse       = texture(sandTex , IN.texCoord );
			bumpNormal    = texture(sandBump , IN.texCoord ).rgb;
		}
		else if(IN.worldPos.y > sandLayer.y && IN.worldPos.y < grassLayer.x) {
			diffuse = mix(texture(grassTex, IN.texCoord), texture(sandTex, IN.texCoord), (grassLayer.x - IN.worldPos.y) / offset); 
			bumpNormal = mix(texture(grassBump, IN.texCoord).rgb, texture(sandBump, IN.texCoord).rgb, (grassLayer.x - IN.worldPos.y) / offset); 
		}
		else if(IN.worldPos.y < grassLayer.y) {
			diffuse       = texture(grassTex , IN.texCoord );
			bumpNormal    = texture(grassBump , IN.texCoord ).rgb;
		}
		else if(IN.worldPos.y > grassLayer.y && IN.worldPos.y < stoneLayer.x) {
			diffuse = mix(texture(diffuseTex, IN.texCoord), texture(grassTex, IN.texCoord), (stoneLayer.x - IN.worldPos.y) / offset); 
			bumpNormal = mix(texture(bumpTex, IN.texCoord).rgb, texture(grassBump, IN.texCoord).rgb, (stoneLayer.x - IN.worldPos.y) / offset); 
		}
		else if(IN.worldPos.y > stoneLayer.x){
			diffuse       = texture(diffuseTex , IN.texCoord );
			bumpNormal    = texture(bumpTex , IN.texCoord ).rgb;
		}
		bumpNormal   = normalize(TBN * normalize(bumpNormal * 2.0 - 1.0));
		float  lambert          = max(dot(incident , bumpNormal), 0.0f);
		float  distance         = length(lightPos[i]  - IN.worldPos );
		float attenuation = 0.4f;
		if(directional[i] == 0){
			attenuation     = 1.0f - clamp(distance / lightRadius[i] ,0.0 ,1.0);
		}
		//else {
		//	attenuation = 0.4f;
		//}
		float  specFactor   = clamp(dot(halfDir , bumpNormal ) ,0.0 ,1.0);
		specFactor          = pow(specFactor , 60.0 );
		float  shadow       = 1.0;  
		vec3  shadowNDC = IN.shadowProj.xyz / IN.shadowProj.w;
		if(abs(shadowNDC.x) < 1.0f && abs(shadowNDC.y) < 1.0f &&  abs(shadowNDC.z) < 1.0f) {
			vec3  biasCoord = shadowNDC  *0.5f + 0.5f;
			float  shadowZ   = texture(shadowTex , biasCoord.xy).x;
			if(shadowZ  < biasCoord.z) {
				shadow = 0.0f;
			}
		}
		vec3  surface    = (diffuse.rgb * lightColour[i].rgb);
		vec4 tmp =vec4(surface * lambert * attenuation, 1.0);
		tmp.rgb += (lightColour[i].rgb * specFactor )* attenuation *0.33;
		tmp.rgb *=  shadow; 
		tmp.rgb += surface * 0.1f; 
		tmp.a = diffuse.a;
		fragColour += tmp;
		fragColour *= IN.colour;
	}
}