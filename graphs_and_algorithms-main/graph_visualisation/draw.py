from manim import *

class DynamicalSystemSphere(ThreeDScene):
    separatress = []
    with open('../separatres.txt') as file:
        lines = [line.rstrip() for line in file]
        for line in lines:
            separatress.append([line[0]] + list(map(int, line[2:].split(' '))))
    
    def func_sphere(self, u, v, radius = 1):
        sphereCentre = np.array([0, 0, 0])
        return radius * np.array([np.cos(u) * np.sin(v) - sphereCentre[0]/radius,
                                  np.sin(u) * np.sin(v) - sphereCentre[1]/radius,
                                  np.cos(v) - sphereCentre[2]/radius])

    def construct(self):
        rng = 7
        axes = ThreeDAxes(
            x_range=[-rng, rng], y_range=[-rng, rng], z_range=[-rng, rng],
            x_length=rng, y_length=rng, z_length=rng
            )
        x_label = axes.get_x_axis_label(Tex("x"))
        y_label = axes.get_y_axis_label(Tex("y"))
        z_label = axes.get_z_axis_label(Tex("z"))
        mainSphere = Surface(
            lambda u, v: axes.c2p(*self.func_sphere(u, v, rng)),
            u_range=[0, TAU],
            v_range=[0, TAU],
            resolution=32, fill_opacity=0.1, checkerboard_colors=['#29ABCA', '#236B8E'], stroke_color=BLACK, stroke_width=0.1
        )
        for sep in self.separatress:
            u_sep1 = ParametricFunction(
                lambda t: (0.5 * rng) * np.array([
                    np.cos(np.pi * (t * sep[3] + sep[4]) / 180) * np.sin(np.pi * (t * sep[1] + sep[2]) / 180),
                    np.cos(np.pi * (t * sep[3] + sep[4]) / 180) * np.cos(np.pi * (t * sep[1] + sep[2]) / 180),
                    np.sin(np.pi * (t * sep[3] + sep[4]) / 180)
                ]), color=RED_A if sep[0]=='u' else BLUE_A, t_range = np.array([0, 1/3, 0.01])
            )
            u_sep1.set_z_index(mainSphere.z_index)
            self.add(u_sep1)

            u_sep2 = ParametricFunction(
                lambda t: (0.5 * rng) * np.array([
                    np.cos(np.pi * (t * sep[3] + sep[4]) / 180) * np.sin(np.pi * (t * sep[1] + sep[2]) / 180),
                    np.cos(np.pi * (t * sep[3] + sep[4]) / 180) * np.cos(np.pi * (t * sep[1] + sep[2]) / 180),
                    np.sin(np.pi * (t * sep[3] + sep[4]) / 180)
                ]), color=RED_C if sep[0]=='u' else BLUE_C, t_range = np.array([1/3, 2/3, 0.01])
            )
            u_sep2.set_z_index(mainSphere.z_index)
            self.add(u_sep2)

            u_sep3 = ParametricFunction(
                lambda t: (0.5 * rng) * np.array([
                    np.cos(np.pi * (t * sep[3] + sep[4]) / 180) * np.sin(np.pi * (t * sep[1] + sep[2]) / 180),
                    np.cos(np.pi * (t * sep[3] + sep[4]) / 180) * np.cos(np.pi * (t * sep[1] + sep[2]) / 180),
                    np.sin(np.pi * (t * sep[3] + sep[4]) / 180)
                ]), color=RED_E if sep[0]=='u' else BLUE_E, t_range = np.array([2/3, 1, 0.01])
            )
            u_sep3.set_z_index(mainSphere.z_index)
            self.add(u_sep3)
        # u_sep.rotate(PI / 4, about_point=[0, 0, 0], axis=RIGHT)
        self.add(axes, mainSphere, x_label, y_label, z_label)
        self.set_camera_orientation(theta=75*DEGREES, phi=75*DEGREES)
        self.begin_ambient_camera_rotation(rate=PI/10, about='theta')
        self.wait(10)
        self.stop_ambient_camera_rotation()