Using imgui to display webcam data from opencv. This can be used in the future to apply filters to these images using the imgui UI.

Issues:
* Only runs at 10fps, I think this is a hard cap from openCV, going to look into using a buffer.

First install openCV and other regular c++ gl dependencies.

```
git clone --recurse-submodules
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