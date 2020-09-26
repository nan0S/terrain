# Terrain
Earth view both in 2D and 3D based on satellite terrain data. When you load the given terrain, you can explore it by moving around either in map view (2D) or in globe view (3D). In order for data to be displayed you have to provide program with HGT files representing square 1 deg by 1 deg on Earth. There are some example files in data directory. 

Example usage of the program: ./terrain ../data/eurasia -lat 0 2 -lon 3 5, which will load from ..data/eurasia directory, following files: N00E03.hgt, N00E04.hgt, N01E03.hgt, N01E04.hgt (notice that ranges of latitudes and longitude are [0,2) and [3, 5)). 

Negative numbers in latitude represent south (S), negative numbers in longitude represent west (W), so eg. ./terrain ../data/eurasia -lat -3 -2 -lon -6 -4, will load S03W06.hgt and S03W05.hgt.

This program is entirely written in C++ from scrach - it uses OpenGL, but no external, third-party library for OpenGL.

#### Navigation

Screenshots
---
### Map view 1
![](https://i.imgur.com/akx6tf2.png)

### Map view 2
![](https://i.imgur.com/6znMa2H.png)

### Map view 3
![](https://i.imgur.com/AzEOma8.png)

### Globe view 1
![](https://i.imgur.com/olVFbQj.png)

### Globe view 2
![](https://i.imgur.com/5bl3Nev.png)

