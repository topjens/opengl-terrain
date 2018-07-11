uniform sampler2D tex_heightmap;

uniform vec4 map_position;
uniform vec4 offset;
uniform vec4 scale;

varying vec2 vs_tex_coord;

void main(void)
{
  vec4 pos = scale * (offset + vec4(gl_Vertex.xyz, 1.0));
  vec4 pos_map = (pos + map_position) * 2.0;
  pos.z = texture2D(tex_heightmap, pos_map.xy).x;
  pos.z *= -0.05;
  gl_Position = gl_ModelViewProjectionMatrix * pos;

  vs_tex_coord.xy = pos_map.xy;
}
