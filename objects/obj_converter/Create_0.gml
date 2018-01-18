spr_palette = -1;
palette_texture = -1;

//shader stuff
max_colors = 8;
max_palettes = 8;

u_palette_sampler = shader_get_sampler_index(shd_superjibby2, "palette_sampler");
u_u_scale = shader_get_uniform(shd_superjibby2, "u_scale");
u_v_scale = shader_get_uniform(shd_superjibby2, "v_scale");