#version 330 core
out vec4 FragColor;

in vec3 vBC;

uniform vec3 uEdgeColor;
uniform vec3 uFaceColor;
uniform float uLineWidth; 

void main()
{
    // Find the closest distance to an edge
    float closestEdge = min(vBC.x, min(vBC.y, vBC.z));

    // Standard derivative-based smoothing
    float delta = fwidth(closestEdge);
    
    // Smoothstep creates the anti-aliased edge
    float edgeFactor = smoothstep(uLineWidth - delta, uLineWidth + delta, closestEdge);

    FragColor = vec4(mix(uEdgeColor, uFaceColor, edgeFactor), 1.0);
}