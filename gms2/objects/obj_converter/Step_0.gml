if (converting)
{
	var _pix, _match, _index;
	
	// convert sprite to indexed sprite
	surface_set_target(surf_out);
		_pix = surface_getpixel(surf_in,u,v);
				
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
	surface_reset_target();
	
	u++;
	
	if (u >= spr_w)
	{
		u = 0;
		v++;
		
		if (v >= spr_h )
		{
			surface_save(surf_out,ofn);
			
			// Clean up
			surface_free(surf_in);
			surface_free(surf_out);
			sprite_delete(spr);
			
			converting = false;
		}
	}
}