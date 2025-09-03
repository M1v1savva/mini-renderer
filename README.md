# 3D mini-renderer 🎨 Tiny OpenGL from Scratch

A minimalist 3D renderer implemented in C++ with **~~almost~~ zero external dependencies**, running entirely on the CPU. 

This renderer does not require any external graphics dependencies for 3D such as OpenGL. It implements its own rasterization, matrix transformations in 3D and shading techniques.

Dependencies used (now): **json, gtest**

Key topics: **rasterization, geometry, linear algebra**.

---

## ✨ Results 

Given two files:  
- `*.obj` (polygonal mesh)  
- `*.bmp` / `*.tga` (texture)  

The renderer managed to produce images like this:

<table>
<tr>
  <td style="text-align: center;">
    <img src="https://user-images.githubusercontent.com/18361541/176958840-88824e83-50db-422e-99c9-c3ed93a92aae.png" width="400"/><br>
    Gouraud shading (no texture)
  </td>
  <td style="text-align: center;">
    <img src="https://user-images.githubusercontent.com/18361541/176958850-28f0d030-ef70-47a9-8d0f-6315c3b9210d.png" width="400"/><br>
    Phong shading (with texture)
  </td>
</tr>
</table>

## 🚀 Features

- **Z-buffering**: depth handling for correct rendering order  
- **Transformations**: camera view, projection, viewport; applied to lights too  
- **Rasterization**: texture mapping, Gouraud and Phong shading
- **Modularity**: camera view configured in `config.json`, loaders, mini geometry library, rasterizer factory

## 📚 Reference

This project is my hands-on take on [ssloy's Tinyrenderer](https://github.com/ssloy/tinyrenderer/wiki). The model used to generate examples with the renderer is the model used in the blog with the permission from creator.  

While TinyRenderer prioritizes minimal code, **mini-renderer** uses a more object-oriented design, making it easier to embed in a GUI or extend for other uses. 

## Examples

<table>
<tr>
  <td style="text-align: center;">
    <img src="https://user-images.githubusercontent.com/18361541/176958840-88824e83-50db-422e-99c9-c3ed93a92aae.png" width="400"/><br>
    Gouraud shading (no texture)
  </td>
  <td style="text-align: center;">
    <img src="https://user-images.githubusercontent.com/18361541/176958850-28f0d030-ef70-47a9-8d0f-6315c3b9210d.png" width="400"/><br>
    Phong shading (with texture)
  </td>
</tr>
</table>

## 🛠️ Build & Run

### CMake 
```
git clone https://github.com/M1v1savva/mini-renderer.git
cd mini-renderer
mkdir build && cd build
cmake ..
cd ..
cmake --build build
./mini-renderer
```

### Docker
```
git clone https://github.com/M1v1savva/mini-renderer.git
docker build -t mini-renderer .
docker run -rm mini-renderer
```

## ▶️ Run

Run `./mini-renderer` to see help.

#### Flags

- `--mode` : RASTERIZER_MODE  
- `--path` : path to the output bmp    

**RASTERIZER_MODE options:**

- `bin`   - black & white with basic lighting  
- `bing`  - black & white with Gouraud shading  
- `binp`  - black & white with Phong shading  
- `tex`   - color with basic lighting  
- `texg`  - color with Gouraud shading 

#### Example
```
./mini-renderer --mode bing --path out.bmp
```

## ⚙️ Config (Optional)

After the build, you can set up `config.json`. The fields for adjastment are: `eye`, `light`. Other fields are left in more so for debugging purposes.

`eye` represents the points of camera view, camera is always pointing towards `(0, 0, 0)`. `light` represents a 3D lighting vector.

```
{
  "model_path":   "obj/african_head.obj",
  "texture_path": "obj/african_head_diffuse.bmp",
  "output_width":  800,
  "output_height": 800,
  "output_depth":  255,
  "eye":      [3.0, -3.0, 6.0],
  "center":   [0.0, 0.0,  0.0],
  "vertical": [0.0, 1.0,  0.0],
  "light":    [0.0, 0.0, -1.0]
}
```

## ✅ Running Tests

After the build, test binaries will be in the root folder:

```
test_vec
test_matrix
test_model
test_graphics
```

```test_copy_image.bmp``` and ```test_graphics_output.bmp``` are also created for debug purposes. 

run ```./model-overview --path <PATH_TO_OBJ>``` for description of an obj file.

## 📝 License

This project is licensed under the MIT License.