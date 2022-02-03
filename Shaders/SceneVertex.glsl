#version  330  core

uniform  mat4  modelMatrix;
uniform  mat4  viewMatrix;
uniform  mat4  projMatrix;
uniform mat4 shadowMatrix;

uniform vec3 lightPos[2];
uniform  vec4  nodeColour;


in   vec3  position;
in   vec2  texCoord;
in vec3 normal;
in vec4 tangent;

out  Vertex   {
	vec2  texCoord;
	vec4  colour;
	vec3  normal;
	vec3  tangent;
	vec3  binormal;
	vec3  worldPos;
	vec4  shadowProj;
} OUT;

void main(void) {
	for(int i = 0; i < 2; ++i){
		OUT.texCoord = texCoord;
		OUT.colour = nodeColour;
		mat3  normalMatrix = transpose(inverse(mat3(modelMatrix )));
		vec3 wNormal = normalize(normalMatrix * normalize(normal));
		vec3 wTangent = normalize(normalMatrix * normalize(tangent.xyz));
		OUT.normal          = wNormal;
		OUT.tangent = wTangent;
		OUT.binormal = cross(wTangent, wNormal) * tangent.w;
	
		vec4  worldPos      = (modelMatrix * vec4(position ,1));
		OUT.worldPos       = worldPos.xyz;
	
		gl_Position         = (projMatrix * viewMatrix) * worldPos;
		vec3  viewDir    = normalize(lightPos[0]  - worldPos.xyz);
		vec4  pushVal    = vec4(OUT.normal  , 0) * dot(viewDir , OUT.normal );
		OUT.shadowProj   = shadowMatrix * (worldPos + pushVal );
	}
}
