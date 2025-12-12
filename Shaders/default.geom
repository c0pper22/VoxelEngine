#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 vBC;

void main() {
    // Determine if this is the first or second triangle of a quad face.
    // Even IDs (0, 2, 4...) are usually the first triangle.
    // Odd IDs (1, 3, 5...) are usually the second triangle.
    bool isEven = (gl_PrimitiveID % 2) == 0;

    if (isEven) {
        // --- Triangle 1 ---
        // We hide the edge opposite Vertex 1 (the diagonal)
        // We do this by setting the 'y' component to 1.0 for both other vertices
        
        gl_Position = gl_in[0].gl_Position;
        vBC = vec3(1.0, 1.0, 0.0); // y=1
        EmitVertex();

        gl_Position = gl_in[1].gl_Position;
        vBC = vec3(0.0, 0.0, 0.0); // The corner opposite the diagonal
        EmitVertex();

        gl_Position = gl_in[2].gl_Position;
        vBC = vec3(0.0, 1.0, 1.0); // y=1
        EmitVertex();
    } else {
        // --- Triangle 2 ---
        // We hide the edge opposite Vertex 2 (the diagonal)
        // We do this by setting the 'z' component to 1.0 for both other vertices
        
        gl_Position = gl_in[0].gl_Position;
        vBC = vec3(1.0, 0.0, 1.0); // z=1
        EmitVertex();

        gl_Position = gl_in[1].gl_Position;
        vBC = vec3(0.0, 1.0, 1.0); // z=1
        EmitVertex();

        gl_Position = gl_in[2].gl_Position;
        vBC = vec3(0.0, 0.0, 0.0); // The corner opposite the diagonal
        EmitVertex();
    }

    EndPrimitive();
}