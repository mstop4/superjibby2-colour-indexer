var _fn = get_open_filename("PNG|*.png|All Files|*.*","")

if (_fn)
{
	if (sprite_exists(spr_palette))
		sprite_delete(spr_palette);
	
	spr_palette = sprite_add(_fn,1,false,false,0,0);
	palette_texture = sprite_get_texture(spr_palette,0);
}