Soko v1.0 by Boro Sitnikovski
=============================

This program is intended for demonstration purposes only.  I wrote this project in about 2 hours.

To clone me: Write `git clone git@github.com:bor0/soko.git`

- Linux users:
    1. Write "make deps" to install dependencies.  I know that "configure" should handle this, but it's a small project anyway.
    2. Write "make package" to make a .tar.gz archive of the binary files.
    3. Write "make" to get soko executable.
    4. Write "./soko 1" or "./soko 2" to run soko with level 1.dat and 2.dat, respectively.
- Windows users:
    1. Use Pelles C on the soko.ppj project file

TODO:
- render() is handling victory, while it is supposed to do rendering only. I did this to avoid parsing the matrix more than once.
- Use configure instead of make?

Music by Spotz.

Boro Sitnikovski

Licensed under GPL.  Read LICENSE for more information.

January, 2014
