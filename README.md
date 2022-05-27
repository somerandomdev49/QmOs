# QmOS

VGA driver from https://github.com/pritamzope/OS/blob/master/VGA/kernel_c/Colors

# TODO:
- port PortableGL here!
- filesystem
- multithreading?
- malloc
- rewrite the vga driver to support more colors/resolutions.
- reorganise the header files.

- driver-agnostic scrolling
  (add flag for having a getchar func)
  * requires malloc

As the filesystem im using doesnt store file/folder creation time, I estimate that the first file I created and never edited again is `linker.ld`, which was last modified 2021-08-19.
(this estimation is ~98.39281492719% correct)

