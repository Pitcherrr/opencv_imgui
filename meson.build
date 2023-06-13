project('imgui test', 'cpp')


incdir = include_directories('./imgui/backends', './imgui')

glfw_lib = subproject('glfw').get_variable('glfw_dep')
gl = dependency('gl')
dl = dependency('dl')
opencv_dep = dependency('opencv4')

gui_files = files(
    'webcam_view.cpp',
    './imgui/imgui.cpp',
    './imgui/imgui_draw.cpp',
    './imgui/imgui_tables.cpp',
    './imgui/imgui_demo.cpp',
    './imgui/imgui_widgets.cpp',
    './imgui/backends/imgui_impl_glfw.cpp',
    './imgui/backends/imgui_impl_opengl3.cpp'
)


executable('demo', gui_files, include_directories: incdir, dependencies: [glfw_lib, opencv_dep, gl, dl])
