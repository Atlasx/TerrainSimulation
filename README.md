# Terrain Simulation
A terrain texture simulation for cs5610 Interactive Computer Graphics.

Based off of "Pattern based procedural textures" by Sylvain Lefebvre and Fabrice Neyret.
http://dl.acm.org/citation.cfm?doid=641480.641518

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

Since I am not calculating perlin noise from a function and just sampling from a texture, there are some repeating patterns caused by my texture settings.

```
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
```

![Sampled Noise](https://raw.githubusercontent.com/Atlasx/TerrainSimulation/master/ProgressImages/perlinOctave.png)

## Dithering Bugs

Turns out dithering is pretty hard.

![Dithering Bugs](https://raw.githubusercontent.com/Atlasx/TerrainSimulation/master/ProgressImages/bugs1.png)

## Example Leaf Pattern

This could possibly be used for a terrain chunk underneath some colored trees.

![Colored Leaves](https://raw.githubusercontent.com/Atlasx/TerrainSimulation/master/ProgressImages/leafpattern.png)

## Example Beach

Another possible use.

![Beach Example](https://raw.githubusercontent.com/Atlasx/TerrainSimulation/master/ProgressImages/beachexample.png)
