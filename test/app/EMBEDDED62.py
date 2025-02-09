from test.TestInstances import TestInstances
import math
import matplotlib.pyplot as plt
import numpy as np

class EMBEDDED62(TestInstances):
    def __init__(self,  port=None,
                        baudrate=None,
                        center=[0, 0, 0],
                        width=2,
                        height=1,
                        length=4):
        super().__init__(port, baudrate)
        self.board_name = None
        # Cube properties
        self.center = center
        self.width = width
        self.height = height
        self.length = length
        self.vertices = None
        self.faces = [
            [0, 1, 2, 3],
            [4, 5, 6, 7],
            [0, 1, 5, 4],
            [2, 3, 7, 6],
            [1, 2, 6, 5],
            [0, 3, 7, 4]
        ]

        # Roll and pitch values
        self.roll = 0
        self.pitch = 0


    def run(self):
        self.init(port=self.port, baudrate=self.baudrate)
        self.board_name = self.get_board_name()

        # Plot the background
        fig = plt.figure()
        ax = fig.add_subplot(111, projection='3d')
        self.create_cuboid()  # Create and update the cube vertices and faces

        plt.ion()  # Interactive mode for live updates
        plt.show()

        try:
            while True:
                self.get_tilt()     # Update roll and pitch values
                rotated_vertices = self.rotate_object()     # Rotate cube based on roll and pitch

                # Clear the previous plot
                ax.clear()

                # Plot the cube
                for face in self.faces:
                    if len(face) == 4:
                        # Draw each edge of the face individually to ensure all lines are drawn
                        for i in range(4):
                            ax.plot([rotated_vertices[face[i], 0], rotated_vertices[face[(i + 1) % 4], 0]],
                                    [rotated_vertices[face[i], 1], rotated_vertices[face[(i + 1) % 4], 1]],
                                    [rotated_vertices[face[i], 2], rotated_vertices[face[(i + 1) % 4], 2]], 'k-')

                # Set axis limits (important for consistent view)
                ax.set_xlim([-3, 3])
                ax.set_ylim([-3, 3])
                ax.set_zlim([-3, 3])
                ax.set_xlabel('X')
                ax.set_ylabel('Y')
                ax.set_zlabel('Z')

                # Add title to show roll and pitch values
                ax.set_title(f"{self.board_name} - Roll: {self.roll:.2f}, Pitch: {self.pitch:.2f}")

                plt.pause(0.01)  # Small delay for visualization

        except:
            print("Exiting...")


    def create_cuboid(self):
        x, y, z = self.center
        half_width = self.width / 2
        half_height = self.height / 2
        half_length = self.length / 2

        self.vertices = np.array([
            [x - half_width, y - half_length, z - half_height],
            [x + half_width, y - half_length, z - half_height],
            [x + half_width, y + half_length, z - half_height],
            [x - half_width, y + half_length, z - half_height],
            [x - half_width, y - half_length, z + half_height],
            [x + half_width, y - half_length, z + half_height],
            [x + half_width, y + half_length, z + half_height],
            [x - half_width, y + half_length, z + half_height]
        ])
        return self.vertices, self.faces

    # Function to rotate the object
    def rotate_object(self):
        # Convert angles to radians
        roll = np.radians(self.roll)
        pitch = np.radians(self.pitch)

        # Rotation matrices (order matters: pitch then roll)
        Rx = np.array([
            [1, 0, 0],
            [0, np.cos(pitch), -np.sin(pitch)],
            [0, np.sin(pitch), np.cos(pitch)]
        ])
        Ry = np.array([
            [np.cos(roll), 0, np.sin(roll)],
            [0, 1, 0],
            [-np.sin(roll), 0, np.cos(roll)]
        ])

        rotated_vertices = self.vertices.copy()
        for i in range(len(self.vertices)):
            rotated_vertices[i] = np.dot(Ry, np.dot(Rx, self.vertices[i]))  # Apply rotations

        return rotated_vertices

    @staticmethod
    def calculate_tilt(accel):
        roll = math.degrees(math.atan2(accel[1], accel[2]))
        pitch = math.degrees(math.atan2(-accel[0], np.sqrt(accel[1] ** 2 + accel[2] ** 2)))
        return roll, pitch

    def get_accel(self):
        return self.mcu.readaccel()

    def get_tilt(self):
        accel = self.get_accel()
        self.roll, self.pitch = self.calculate_tilt(accel)

if __name__ == "__main__":
    blink_led = EMBEDDED62(port='COM9',
                           baudrate=500000,
                           center=[0, 0, 0],
                           width=2,
                           height=1,
                           length=4)

    blink_led.run()
    blink_led.cleanup()