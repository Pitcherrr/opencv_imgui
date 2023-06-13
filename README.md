Start of a GUI project im going to attempt. imgui has pretty poor documantation in regards to building the application though this is probably cause im a c noob.

This is just a way to run the glfw opengl3 demo from imgui on linux using the meson build system, https://mesonbuild.com/:

```
git clone --resurse-submodules
```
```
meson setup builddir
```

```
cd builddir
```

```
meson compile
```

```
./demo
```