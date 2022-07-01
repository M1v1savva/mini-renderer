## Minimalist renderer from scratch

This is my implementation of the [tinyrenderer](https://github.com/ssloy/tinyrenderer/wiki) by [ssloy](https://github.com/ssloy).
Tinyrenderer is a series of articles about 3D graphics and basic concepts behind them. The explanations are neat and results and aethetically pleasing, so I definitely recommend cheking it out. 

I wrote this when I was in middle school so the code is very similar to original tinyrenderer as I was just starting to learn advanced C++ concepts and relied a lot on the implementation in the articles. 

Also, I obviously don't own the model used for rendering.

[Same articles written in Russian](https://habr.com/ru/post/248153/)

---

## Results

Web model:<br/>
![0 web](https://user-images.githubusercontent.com/18361541/176958784-07da72cc-737c-4960-be3f-dfb4e49cbf9d.png)

Added light:<br/>
![1 light](https://user-images.githubusercontent.com/18361541/176958798-4a24d642-ffee-4e48-8b89-c6368c61c815.png)

Added z-buffer:<br/>
![2 z-buffer](https://user-images.githubusercontent.com/18361541/176958806-d2a16dc5-02a9-40ae-9072-73fe21588aed.png)

Added matrix transformations: camera, perspective distortion, etc<br/>
![4 binary](https://user-images.githubusercontent.com/18361541/176958814-1841f857-49b4-4ead-8e17-d15ffd7915e4.png)

Added textures:<br/>
![5 texture](https://user-images.githubusercontent.com/18361541/176958819-2474d11b-7545-4838-b39e-d006e284c82a.png)

Gouraud shading, no textures:<br/>
![6 gouraud_binary](https://user-images.githubusercontent.com/18361541/176958830-60c67b16-5ab7-420c-9ff3-581afb0df870.png)

Phong shading, no textures:<br/>
![7 phong_binary](https://user-images.githubusercontent.com/18361541/176958840-88824e83-50db-422e-99c9-c3ed93a92aae.png)

Gouraud shading, with textures:<br/>
![8 gouraud_texture](https://user-images.githubusercontent.com/18361541/176958850-28f0d030-ef70-47a9-8d0f-6315c3b9210d.png)

Phong shading, with textures:<br/>
![9 phong_texture](https://user-images.githubusercontent.com/18361541/176958865-d585bbd2-1f4e-4106-94e3-b78d9608b0b7.png)
