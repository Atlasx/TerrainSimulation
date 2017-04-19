# Terrain Simulation
A terrain simulation for cs5610 Interactive Computer Graphics, work in progress.

## Virtual Grid

This is the virtual grid that has a user-specified size. It can be any size and will create a virtual texture several times larger than the input texture.

![Virtual Grid](https://raw.githubusercontent.com/Atlasx/TerrainSimulation/master/ProgressImages/virtualgrid.png)

## Texture Atlas Sampling

Based on a small input image, it will sample the texture atlas. This creates a virtual image larger than what could be done with just a single texture.

![Coastline](https://raw.githubusercontent.com/Atlasx/TerrainSimulation/master/ProgressImages/coastline.png)

## Random Rotation

Based on a sample of perlin noise, rotate the textures from the atlas to create less grid-like textures.

![Random Rotation](https://raw.githubusercontent.com/Atlasx/TerrainSimulation/master/ProgressImages/randomrotation.png)

## Larger Virtual Grids

Since the size of the virtual grids is user specified, and the virtual map can be of any size, we will need to smoothly interpolate between areas otherwise the virtual grid will look chunky like this.

![Chunky Virtual Grid](https://raw.githubusercontent.com/Atlasx/TerrainSimulation/master/ProgressImages/largevirtualgrid.png)

## Octaves in Perlin Noise

Now you may have noticed the random rotation in the Chunky Virtual Grid wasn't very random. It was actually sampled from perlin noise overlapping the whole vritual grid space. To not make so obvious we can include multiple octaves of perlin noise to change the rotation more in some areas, and less in others.

![Random Noise](https://raw.githubusercontent.com/Atlasx/TerrainSimulation/master/ProgressImages/randomoctaves.png)
