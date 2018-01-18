if (spr_palette == -1)
	show_message_async("No palette file loaded");

else
{
	var _fn = get_open_filename("PNG|*.png|All Files|*.*","");

	if (_fn)
	{
		// Load image to sprite and get info
		var _spr = sprite_add(_fn,1,false,false,0,0);
		var _spr_w = sprite_get_width(_spr);
		var _spr_h = sprite_get_height(_spr);
	
		var _surf = surface_create(_spr_w,_spr_h);
		
		// convert sprite to indexed sprite
		surface_set_target(_surf);
			shader_set(shd_superjibby2);
			
			texture_set_stage(u_palette_sampler,palette_texture);
			shader_set_uniform_f(u_u_scale, 256 / max_colors);
			shader_set_uniform_f(u_v_scale, 256 / max_palettes);
			
			draw_sprite(_spr,0,0,0);
			
			shader_reset();
		surface_reset_target();
		
		// save
		var _out_name = "i_" + filename_name(_fn);
		_fn = get_save_filename("PNG|*.png",_out_name);
		
		if (_fn)
			surface_save(_surf,_fn);
			
		else
			show_message_async("Error saving image.");
	
		// Clean up
		surface_free(_surf);
		sprite_delete(_spr);
	}
	
	else
		show_message_async("Error loading image.");
}