![image](https://user-images.githubusercontent.com/2289361/188794031-8aeacf03-742a-43ef-be7e-729d8090c5b8.png)

STnl stands for Software Transform and Lighting. It's a simple software renderer / rasterizer that demonstrates the basic principles of a programmable GPU.

We use 4 threads to render each horizontal quarter of the screen in parallel, so this application runs best on systems with 4 or more CPU cores.

**How to Build**

You'll need Visual Studio 2012. Build *Workspace\STnL\STnL.sln*.

**Key Bindings**

* Left mouse button drag - Rotate camera
* Mouse wheel - Zoom in / out
* Right mouse button drag - Move light source
* C - Cycle through models
* W - Toggle wire frame mode
* R - Reset scene
