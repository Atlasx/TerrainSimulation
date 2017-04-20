# Virtual Terrain Texture
A virtual terrain texture for cs5610 Interactive Computer Graphics.

Based off of "Pattern based procedural textures" by Sylvain Lefebvre and Fabrice Neyret.
http://dl.acm.org/citation.cfm?doid=641480.641518

## Virtual Grid

The virtual grid divides the UV space into as many chunks as the user specifies. Each chunk will later be rendered from a texture atlas encoded from the virtual map.

![Virtual Grid](https://raw.githubusercontent.com/Atlasx/TerrainSimulation/master/ProgressImages/virtualgrid.png)

## Texture Atlas Sampling

Based off of the virtual map, chunks are selected from the texture atlas and placed into the virtual grid.

![Coastline](https://raw.githubusercontent.com/Atlasx/TerrainSimulation/master/ProgressImages/coastline.png)

## Random UV Flip

Based on a sample of perlin noise, flip the UVs of the sampled chunk to make the repetition less obvious.

![Random Rotation](https://raw.githubusercontent.com/Atlasx/TerrainSimulation/master/ProgressImages/randomrotation.png)

## Larger Virtual Grids

Since the size of the virtual grids is user specified, and the virtual map can be of any size, we will need to smoothly interpolate between areas otherwise the virtual grid will look chunky like this.

![Chunky Virtual Grid](https://raw.githubusercontent.com/Atlasx/TerrainSimulation/master/ProgressImages/largevirtualgrid.png)

## 2nd Layer of Perlin Noise

Now you may have noticed the random UV flip in the Chunky Virtual Grid wasn't very random. It was actually sampled from perlin noise overlapping the whole vritual grid space. To not make so obvious we can include multiple samples of perlin noise at different scales get a higher detail, yet not repeating pattern.

![Random Noise](https://raw.githubusercontent.com/Atlasx/TerrainSimulation/master/ProgressImages/randomoctaves.png)

Since it is not calculating perlin noise from a function and just sampling from a texture, there are some repeating patterns caused by my texture settings.

```
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
```

![Sampled Noise](https://raw.githubusercontent.com/Atlasx/TerrainSimulation/master/ProgressImages/perlinOctave.png)

## Dithering Bugs

When trying to implement dithering there were many confusing bugs.

![Dithering Bugs](https://raw.githubusercontent.com/Atlasx/TerrainSimulation/master/ProgressImages/bugs1.png)

## Example Leaf Pattern

This could possibly be used for a terrain chunk underneath some colored trees.

![Colored Leaves](https://raw.githubusercontent.com/Atlasx/TerrainSimulation/master/ProgressImages/leafpattern.png)

## Example Beach

Another possible use.

![Beach Example](https://raw.githubusercontent.com/Atlasx/TerrainSimulation/master/ProgressImages/beachexample.png)
