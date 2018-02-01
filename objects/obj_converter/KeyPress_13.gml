var _fn = get_open_filename("PNG|*.png|All Files|*.*","");

if (_fn <> "")
{
	// Load image to sprite and get info
	var _spr = sprite_add(_fn,1,false,false,0,0);
	var _spr_w = sprite_get_width(_spr);
	var _spr_h = sprite_get_height(_spr);
	
	var _surf_in = surface_create(_spr_w,_spr_h);
	var _surf_out = surface_create(_spr_w,_spr_h);
	var _pix, _index, _r, _g, _b, _match;
		
	// prepare source surface
	surface_set_target(_surf_in);
		draw_sprite(_spr,0,0,0);
	surface_reset_target();		
		
	// convert sprite to indexed sprite
	surface_set_target(_surf_out);
		
		for (var u=0; u<_spr_w; u++)
		{
			for (var v=0; v<_spr_h; v++)
			{
				_pix = surface_getpixel(_surf_in,u,v);
					
				for (var i=0; i<max_colors; i++)
				{
					_match = false;
						
					for (var j=0; j<max_palettes; j++)
					{
						_index = i*max_palettes+j;
						if (_pix == in_color[_index])
						{
							draw_point_color(u,v,out_color[_index]);
							_match = true;
							break;
						}
					}
						
					if (_match)
						break;
				}
			}
		}
			
	surface_reset_target();
		
	// save
	var _out_name = "i_" + filename_name(_fn);
	_fn = get_save_filename("PNG|*.png",_out_name);
		
	if (_fn <> "")
		surface_save(_surf_out,_fn);
			
	else
		show_message_async("Error saving image.");
	
	// Clean up
	surface_free(_surf_in);
	surface_free(_surf_out);
	sprite_delete(_spr);
}
	
else
	show_message_async("Error loading image.");