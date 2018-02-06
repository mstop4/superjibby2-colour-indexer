current_directory = program_directory;
search_criteria = "*.png";
found_files = ds_list_create();

var _file = file_find_first(search_criteria,0);

while (_file <> "")
{
	ds_list_add(found_files,_file);
	_file = file_find_next();
}

file_find_close();

for (var i=0; i<ds_list_size(found_files); i++)
{
	show_message(found_files[| i]);
}

ds_list_destroy(found_files);