# LIDAR-3D
A super-sketchy proof of concept to try and build a 3D scanner for cave mapping
by re-purposing a robot vacuum's 2D LIDAR module.

## Parts:
- STM32 bluepill
- Hitachi HLS-LFCD2 LIDAR
- 28BYJ-48 stepper + ULN2003 driver
- 3d printed bits

Currently the bluepill spits out splays (Az, Inc, Dst) which are dumped to a CSV.
That is then fed into the Perl script to translate into cartesian coordinates and formatted into a Stanford poly file.

## References:
http://faculty.nps.edu/jenn/seminars/radarfundamentals.pdf
https://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates

