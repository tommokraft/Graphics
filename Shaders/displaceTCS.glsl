#version 400 core
layout(vertices =4) out;

in  Vertex    {
	vec4  colour;
	vec2  texCoord;
	vec3  normal;
	vec3  worldPos;

} IN[];             //Equal  to size of the  draw  call  vertex  count

out  Vertex   {
	vec4  colour;
	vec2  texCoord;
	vec3  normal;
	vec3  worldPos;
} OUT [];            // Equal to the  size of the  layout  vertex  count

void  main() {
	gl_TessLevelInner [0] = 6400;
	gl_TessLevelInner [1] = 6400;
	gl_TessLevelOuter [0] = 6400;
	gl_TessLevelOuter [1] = 6400;
	gl_TessLevelOuter [2] = 6400;
	gl_TessLevelOuter [3] = 6400;
	
	OUT[gl_InvocationID ]. texCoord = IN[gl_InvocationID ]. texCoord;
	
	gl_out[gl_InvocationID ]. gl_Position = gl_in[gl_InvocationID ]. gl_Position;

}