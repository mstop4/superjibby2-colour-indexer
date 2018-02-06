var _ifn = get_open_filename("PNG|*.png|All Files|*.*","");
_ifn = "paths2.png";

if (_ifn <> "")
{
	// save
	var _out_name = "i_" + filename_name(ofn);
	ofn = get_save_filename("PNG|*.png",_out_name);
	
	if (ofn <> "")
	{
		// Load image to sprite and get info
		spr = sprite_add(_ifn,1,false,false,0,0);
		spr_w = sprite_get_width(spr);
		spr_h = sprite_get_height(spr);
	
		surf_in = surface_create(spr_w,spr_h);
		surf_out = surface_create(spr_w,spr_h);
		
		// prepare source surface
		surface_set_target(surf_in);
			draw_sprite(spr,0,0,0);
		surface_reset_target();		
		converting = true;
	}
		
	else
		show_message_async("Error saving image.");
}
	
else
	show_message_async("Error loading image.");