import pyzed.sl as sl
import numpy as np
import time

class ZedPerformanceTester:
    def __init__(self):
        self.zed = sl.Camera()
        init_params = sl.InitParameters()
        init_params.camera_resolution = sl.RESOLUTION.HD720
        init_params.depth_mode = sl.DEPTH_MODE.PERFORMANCE
        init_params.coordinate_units = sl.UNIT.METER

        status = self.zed.open(init_params)
        if status != sl.ERROR_CODE.SUCCESS:
            print("Impossibile aprire ZED: {}".format(status))  # Modificato qui
            exit(1)

        self.last_time = time.time()
        self.frame_count = 0
        self.avg_fps = 0.0
        self.avg_process_time = 0.0

    def run(self):
        try:
            while True:
                start_time = time.time()
                runtime_parameters = sl.RuntimeParameters()
                if self.zed.grab(runtime_parameters) == sl.ERROR_CODE.SUCCESS:
                    point_cloud = sl.Mat()
                    resolution = sl.Resolution()
                    resolution.width = 480
                    resolution.height = 480
                    
                    err = self.zed.retrieve_measure(point_cloud, sl.MEASURE.XYZRGBA, sl.MEM.CPU, resolution)
                    if err != sl.ERROR_CODE.SUCCESS:
                        print("Errore nel retrieve_measure: {}".format(err))  # Modificato qui
                        continue

                    array = point_cloud.get_data()
                    height, width, _ = array.shape
                    points = array.reshape(-1, 4)
                    mask = ~np.isnan(points[:, 0])
                    points = points[mask]

                    process_time = time.time() - start_time
                    self.frame_count += 1
                    self.avg_process_time = 0.9 * self.avg_process_time + 0.1 * process_time

                    if time.time() - self.last_time >= 1.0:
                        self.avg_fps = self.frame_count / (time.time() - self.last_time)
                        print("FPS: {:.2f} | Tempo elaborazione: {:.2f}ms | Punti: {}".format(  # Modificato qui
                            self.avg_fps, self.avg_process_time * 1000, len(points))
                        )
                        self.frame_count = 0
                        self.last_time = time.time()

        except KeyboardInterrupt:
            print("Interruzione da tastiera, chiusura...")
        finally:
            self.zed.close()

if __name__ == '__main__':
    tester = ZedPerformanceTester()
    tester.run()
