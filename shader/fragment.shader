unifrom sampler2D tex_terrain;

varying vec2 vs_tex_coord;

void main(void)
{
  gl_FragColor = texture2D(tex_terrain, vs_tex_coord);
}
