from typing import List
import numpy as np
from utils import Particle
import pdb
from copy import deepcopy

### 可以在这里写下一些你需要的变量和函数 ###
COLLISION_DISTANCE = 1 / np.sqrt(2)
MAX_ERROR = 50000
k = 0.5
sigma_p = 0.09
sigma_t = 0.009
### 可以在这里写下一些你需要的变量和函数 ###


def generate_uniform_particles(walls, N):
    """
    输入：
    walls: 维度为(xxx, 2)的np.array, 地图的墙壁信息，具体设定请看README关于地图的部分
    N: int, 采样点数量
    输出：
    particles: List[Particle], 返回在空地上均匀采样出的N个采样点的列表，每个点的权重都是1/N
    """
    all_particles: List[Particle] = []
    ### 你的代码 ###
    min_x, min_y = np.min(walls, axis=0)
    max_x, max_y = np.max(walls, axis=0)
    for _ in range(N):
        #theta = np.random.uniform(0, 2 * np.pi)
        theta = np.random.uniform(-np.pi, np.pi)
        while True:
            x = np.random.uniform(min_x, max_x)
            y = np.random.uniform(min_y, max_y)
            if np.all(np.linalg.norm(np.array([x, y]) - walls, axis=1) > COLLISION_DISTANCE):
                all_particles.append(Particle(x, y, theta, 1.0 / N))
                break
    ### 你的代码 ###
    return all_particles


def calculate_particle_weight(estimated, gt):
    """
    输入：
    estimated: np.array, 该采样点的距离传感器数据
    gt: np.array, Pacman实际位置的距离传感器数据
    输出：
    weight, float, 该采样点的权重
    """
    ### 你的代码 ###
    weight = np.exp(-k * np.linalg.norm(estimated - gt))
    ### 你的代码 ###
    return weight


def resample_particles(walls, particles: List[Particle]):
    """
    输入：
    walls: 维度为(xxx, 2)的np.array, 地图的墙壁信息，具体设定请看README关于地图的部分
    particles: List[Particle], 上一次采样得到的粒子，注意是按权重从大到小排列的
    输出：
    particles: List[Particle], 返回重采样后的N个采样点的列表
    """
    resampled_particles: List[Particle] = []
    ### 你的代码 ### 
    N = len(particles)
    weights = np.array([p.weight for p in particles])
    n_particles = (weights * N).astype(int)
    
    for i, p in enumerate(particles):
        for _ in range(n_particles[i]):
            resampled_particles.append(deepcopy(p))
    
    min_x, min_y = np.min(walls, axis=0)
    max_x, max_y = np.max(walls, axis=0)
    while len(resampled_particles) < N:
        #theta = np.random.uniform(0, 2 * np.pi)
        theta = np.random.uniform(-np.pi, np.pi)
        while True:
            x = np.random.uniform(min_x, max_x)
            y = np.random.uniform(min_y, max_y)
            if np.all(np.linalg.norm(np.array([x, y]) - walls, axis=1) > COLLISION_DISTANCE):
                resampled_particles.append(Particle(x, y, theta, 1.0 / N))
                break
    ### 你的代码 ###
    return resampled_particles


def apply_state_transition(p: Particle, traveled_distance, dtheta):
    """
    输入：
    p: 采样的粒子
    traveled_distance, dtheta: ground truth的Pacman这一步相对于上一步运动方向改变了dtheta，并移动了traveled_distance的距离
    particle: 按照相同方式进行移动后的粒子
    """
    ### 你的代码 ###
    p.position += np.random.normal(0, sigma_p, size=p.position.shape)
    p.theta += np.random.normal(0, sigma_t)
    
    p.theta += dtheta
    p.position += traveled_distance * np.array([np.cos(p.theta), np.sin(p.theta)])
    ### 你的代码 ###
    return p

def get_estimate_result(particles: List[Particle]):
    """
    输入：
    particles: List[Particle], 全部采样粒子
    输出：
    final_result: Particle, 最终的猜测结果
    """
    ### 你的代码 ###
    return max(particles, key=lambda p: p.weight)
    ### 你的代码 ###