# Design Notes

## crawler
Goal for right now: create a directory crawler that will crawl a root directory, and print the following information to stdout: path, size, and time created

### crawl_file_tree
This is the entry point into the crawler. Takes a string "rootdir" that s the root directory to start descending. 
Opens rootdir, calls handle_dir

### handle_dir
Takes a directory path dirpath (string). 
Loop through directory contents, creating a struct(?) that stores the  path name, the size, and the date created, and also if its a directory or not. 
These structs go in an array.

Loop through each of the structs and ask if it is a directory, if it is then call handle_dir again with its file name as the rootdir. otherwise, call handle_file

### handle_file
takes in a file struct, for now, just print the contents to stdout