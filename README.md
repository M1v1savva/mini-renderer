This is my simple render that I made based on a wonderful series by @haqreu from habrahabr.com. Therefore, I do not own the model (see obj directory).


General project information:

The final product can generate 	7 types of pictures 800x800:

1. HOLES           -fill all the parts of the model with green(useful for debug when you need to check whether there are any holes in your picture).
2. BINARY          -black and white picture without any shading.
3. TEXTURE         -full model without any shading.
4. BINARY_GAURAUD  -black and white picture & Gauraud shading.
5. BINARY_PHONG    -black and white picture & Phong shading.
6. TEXTURE_GAURAUD -full model & Gauraud shading.
7. TEXTURE_PHONG   -full model & Phong shading.

Before start of the rendering process you can set the position of your eye, the center of the model in world scene, the vector that will be oriented vertically(you can set (0, 1, 0) by default) 
in the picture and the light vector. You might be wondering what is this vertical vector for. In fact, when you look at something with your eyes you can alwasy turn 
your head a littly bit and the picture will change even though the position of your eyes and the object stayed. So we need the vertical vector to resolve this.

Moreover, the render is not model-dependent, so you can try something else as well, though I am not sure if it works fine with none-square texture files.

Note, that I used .bmp format for all the pictures and texture files in the final version and .tga in old versions.


Directories information:

Final - the final product.

Obj - the model that I used. As I said before, I do now own this model, so if you want to use it not within this course you first have to obtain a permission of the 
owner(which you can find following the links I attached below).

Old - how I developed this small project following the articles in case you decide to try them as well.

Pictures - what I managed to achieve.

Links:

The article I used in Russian: https://habr.com/ru/post/248153/

The article (and much more interesting stuff) in English: https://github.com/ssloy/tinyrenderer/wiki

                 
