import numpy as np
import time
import torch 
import random as rand
import sensor_msgs_py.point_cloud2 as pc2


def diane_multinomial_i16(points: torch.Tensor, output: torch.Tensor, points_reshaped: torch.Tensor, 
                    x: torch.Tensor, y: torch.Tensor, z: torch.Tensor, c: torch.Tensor, 
                    bw: float, fps: float, dist_guarant: float,
                    default_color: float, nan_color: float, n_points: int, multiplier: float = 1.0):
    bw = bw
    dist_guarant = 4.
    # Copy the reshaped data into the preallocated tensor without reallocating
    PSZ = 4 * 16 # bits per point
    MFS = bw / fps # Maximum frame size in bits

    max_points = int(np.min([MFS / PSZ, n_points])) # Maximum number of points in a frame

    points_reshaped[:] = points.view(-1, 4)  # Copy the reshaped data into the preallocated tensor

    # Extract x, y, z, c from points using preallocated tensors
    x[:] = points_reshaped[:, 0]
    y[:] = points_reshaped[:, 1]
    z[:] = points_reshaped[:, 2]
    c[:] = points_reshaped[:, 3]
    
    is_nan_c = torch.isnan(c)
    c[is_nan_c] = default_color

    # Handle NaN values and invalid conditions
    invalid_mask = torch.isnan(x) | torch.isinf(x) | (z * multiplier > torch.iinfo(torch.int16).max)

    

    is_valid = torch.nan_to_num((1/( (z/(torch.max(z[~invalid_mask])+1e-6)) +1e-6)) * ~invalid_mask, nan=0) # priority based on distance and validity

    #print("desired FPS", fps, "available BW", bw/1000000, "Mbit/s")
    #print("got", x.shape[0], "points")





    # pump up the priority of points within the distance guarantee

    is_valid[z < dist_guarant] = torch.max(is_valid)    #TO CHECK


    # compute valid indices using multinomial distribution based on validity and inverse of normalized distance
    is_valid = torch.multinomial(is_valid, max_points, replacement=False)


    # Step 1: Scan - Compute prefix sum to get valid indices
    #valid_indices = torch.cumsum((is_valid!=0), dim=0) - 1     #TO CHECK

    #num_valid = valid_indices[-1] + 1 if n_points > 0 else 0  # Number of valid points     #TO CHECK

    num_valid = x[is_valid].shape[0]

    #print("sending ", num_valid, "points")

    if num_valid == 0:
        return torch.empty(0, dtype=output.dtype, device=output.device)  # Return empty if no valid points

    c_rgb = c[is_valid].view(torch.uint8).to(torch.int16)
    # Convert RGB to RGB332 format
    rgb664 = (((c_rgb[:-1:4] >> 2) & 0b111111) << 10) | (((c_rgb[1:-1:4] >> 2) & 0b111111) << 4) | ((c_rgb[2:-1:4] >> 4) & 0b1111)

    # Fill the output tensor with the compacted XYZ values
    output[:num_valid * 3:3] = (x[is_valid] * multiplier).to(torch.int16)
    output[1:num_valid * 3:3] = (y[is_valid] * multiplier).to(torch.int16)
    output[2:num_valid * 3:3] = (z[is_valid] * multiplier).to(torch.int16)
    output[num_valid*3: num_valid*4] = rgb664
    

    #print("num_valid: ",num_valid)
    
    newoutput = pointcloud2_to_xyz_array(cloud_msg)
    return [newoutput, num_valid * 8]
    
    
def pointcloud2_to_xyz_array(cloud_msg):
    # Prende solo i punti xyz
    points = np.array([list(p) for p in pc2.read_points(cloud_msg, field_names=("x", "y", "z"), skip_nans=True)])
    return points


######## Utils ########

def color_to_float(color = (0.9, 0.9, 0.9, 1.0)):
    r, g, b, a = color
    packed = (int(r * 255) << 24) | (int(g * 255) << 16) | (int(b * 255) << 8) | int(a * 255)
    return float(np.frombuffer(packed.to_bytes(4, byteorder='little'), dtype=np.float32)[0])


class Encoder:
    def __init__(self, resolution, default_color = (0.9, 0.9, 0.9, 1.0), nan_color = (1.0, 1.0, 1.0, 1.0)):
        self.resolution = resolution
        self.n_points = resolution.height * resolution.width

        self.default_color = color_to_float(default_color)
        self.nan_color = color_to_float(nan_color)

        self._init_fields()

    def _init_fields(self):
        """
        Initialize input and output fields based on resolution and encoding.
        """

        self.points = torch.empty(
                (self.resolution.height, self.resolution.width, 4),
                dtype=torch.float32, 
                device='cuda'
            )
        
        self.points_reshaped = torch.empty(
                (self.resolution.height*self.resolution.width, 4),
                dtype=torch.float32, 
                device='cuda'
            )
        
        self.x = torch.empty(
                (self.resolution.height*self.resolution.width),
                dtype=torch.float32, 
                device='cuda'
            )
        
        self.y = torch.empty(
                (self.resolution.height*self.resolution.width),
                dtype=torch.float32, 
                device='cuda'
            )
        
        self.z = torch.empty(
                (self.resolution.height*self.resolution.width),
                dtype=torch.float32, 
                device='cuda'
            )
        
        self.c = torch.empty(
                (self.resolution.height*self.resolution.width),
                dtype=torch.float32, 
                device='cuda'
            )

        self.output = torch.empty(
		    (self.resolution.height*self.resolution.width*5),
		    dtype=torch.int16, 
		    device='cuda'
                )
        
    def encode(self, numpy_pc, bw, fps=20, dist_guarant=2.0):
        self.points[:] = torch.tensor(numpy_pc, dtype=torch.float32, device='cuda')
        bufferSize = None
        bufferSize = diane_multinomial_i16(
                self.points,
                self.output,
                self.points_reshaped,
                self.x,
                self.y,
                self.z,
                self.c,
                bw,
                fps,
                dist_guarant,
                self.default_color,
                self.nan_color,
                (self.resolution.height*self.resolution.width),
                10.0
            )
        
        output_pinned = self.output.to("cpu", non_blocking=True).pin_memory()

        if bufferSize is not None:
            return output_pinned.numpy().tobytes()[:bufferSize]
        else:
            return self.output.cpu().numpy().tobytes()
