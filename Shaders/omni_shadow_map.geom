#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 light_matrices[6];

out vec4 frag_pos;

void main() {

	for(int face = 0; face < 6; face++) {
		
		gl_Layer = face;
		for(int vertex_index = 0; vertex_index < 3; vertex_index++) {
		
			frag_pos = gl_in[vertex_index].gl_Position;
			gl_Position = light_matrices[face] * frag_pos;
			EmitVertex();
		}

		EndPrimitive();
	}

}