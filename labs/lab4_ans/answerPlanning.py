import numpy as np
from typing import List
from utils import TreeNode
from simuScene import PlanningMap
import pdb


### 定义一些你需要的变量和函数 ###
STEP_DISTANCE = 1.0
TARGET_THREHOLD = 0.5
TARGET_TIMES = 10
RADIUS_FACTOR = 50.0
SPEED_THREHOLD = 0.5
### 定义一些你需要的变量和函数 ###


class Node:
    def __init__(self, p_idx, x, y) -> None:
        self.parent_idx = p_idx
        self.pos = np.array([x, y])
        self.cost = 0.0


class RRT:
    def __init__(self, walls) -> None:
        """
        输入包括地图信息，你需要按顺序吃掉的一列事物位置 
        注意：只有按顺序吃掉上一个食物之后才能吃下一个食物，在吃掉上一个食物之前Pacman经过之后的食物也不会被吃掉
        """
        self.map = PlanningMap(walls)
        self.walls = walls
        # 其他需要的变量
        
        ### 你的代码 ###      
        self.current_target_idx = 0
        self.current_target_calls = 0
        ### 你的代码 ###
        # 如有必要，此行可删除
        self.path = []
        
        
    def find_path(self, current_position, next_food):
        """
        在程序初始化时，以及每当 pacman 吃到一个食物时，主程序会调用此函数
        current_position: pacman 当前的仿真位置
        next_food: 下一个食物的位置
        
        本函数的默认实现是调用 build_tree，并记录生成的 path 信息。你可以在此函数增加其他需要的功能
        """
        
        ### 你的代码 ###
        self.current_target_idx = 0
        self.current_target_calls = 0
        ### 你的代码 ###
        # 如有必要，此行可删除
        self.path = self.build_tree(current_position, next_food)      
        
        
    def get_target(self, current_position, current_velocity):
        """
        主程序将在每个仿真步内调用此函数，并用返回的位置计算 PD 控制力来驱动 pacman 移动
        current_position: pacman 当前的仿真位置
        current_velocity: pacman 当前的仿真速度
        一种可能的实现策略是，仅作为参考：
        （1）记录该函数的调用次数
        （2）假设当前 path 中每个节点需要作为目标 n 次
        （3）记录目前已经执行到当前 path 的第几个节点，以及它的执行次数，如果超过 n，则将目标改为下一节点
        
        你也可以考虑根据当前位置与 path 中节点位置的距离来决定如何选择 target
        
        同时需要注意，仿真中的 pacman 并不能准确到达 path 中的节点。你可能需要考虑在什么情况下重新规划 path
        """
        
        ### 你的代码 ###
        speed = np.linalg.norm(current_velocity)
        #print(f"spped: {speed}")
        if speed < SPEED_THREHOLD:
            self.find_path(current_position, self.path[-1])
        
        self.current_target_calls += 1
        
        if self.current_target_idx >= len(self.path):
            return self.path[-1]
        
        #step_size = int(np.linalg.norm(current_velocity) / 10) + 1
        #print(f"step_size: {step_size}")
        step_size = 1
        
        if self.current_target_calls >= TARGET_TIMES \
        or np.linalg.norm(current_position - self.path[self.current_target_idx]) < TARGET_THREHOLD:
            self.current_target_idx += step_size
            self.current_target_calls = 0
            
            if self.current_target_idx >= len(self.path):
                return self.path[-1]

        return self.path[self.current_target_idx]
        ### 你的代码 ###
        
        
    ### 以下是RRT中一些可能用到的函数框架，全部可以修改，当然你也可以自己实现 ###
    def build_tree(self, start, goal):
        """
        实现你的快速探索搜索树，输入为当前目标食物的编号，规划从 start 位置食物到 goal 位置的路径
        返回一个包含坐标的列表，为这条路径上的pd targets
        你可以调用find_nearest_point和connect_a_to_b两个函数
        另外self.map的checkoccupy和checkline也可能会需要，可以参考simuScene.py中的PlanningMap类查看它们的用法
        """
        path = []
        graph: List[Node] = []
        start_node = Node(-1, start[0], start[1])
        start_node.cost = 0.0
        graph.append(start_node)
        
        while True:
            x_rand = np.random.rand() * self.map.height
            y_rand = np.random.rand() * self.map.width
            p_rand = np.array([x_rand, y_rand])
            if self.map.checkoccupy(p_rand):
                continue
            
            idx_near, _ = self.find_nearest_point(p_rand, graph)
            p_near = graph[idx_near].pos
            
            is_empty, p_new = self.connect_a_to_b(p_near, p_rand)
            if is_empty:
                new_node = Node(idx_near, p_new[0], p_new[1])
                new_node.cost = graph[idx_near].cost + np.linalg.norm(p_new - p_near)
                
                # Find nodes in the neighborhood of the new node
                neighbors = self.find_neighbors(new_node, graph)

                # Choose the node with the least cost to reach the new node
                node_min, cost_min = self.choose_parent(new_node, neighbors)
                if node_min:
                    new_node.parent_idx = graph.index(node_min)
                    new_node.cost = cost_min
                
                graph.append(new_node)

                # Update the cost of reaching the nodes in the neighborhood through the new node
                self.rewire(new_node, neighbors, graph)

                if np.linalg.norm(p_new - goal) < TARGET_THREHOLD:
                    break            
                
                
        path.append(goal)
        node = graph[-1]
        while node.parent_idx != -1:
            path.append(node.pos)
            node = graph[node.parent_idx]
        path.reverse()

        return path


    @staticmethod
    def find_nearest_point(point, graph):
        """
        找到图中离目标位置最近的节点，返回该节点的编号和到目标位置距离、
        输入：
        point：维度为(2,)的np.array, 目标位置
        graph: List[TreeNode]节点列表
        输出：
        nearest_idx, nearest_distance 离目标位置距离最近节点的编号和距离
        """
        nearest_idx = -1
        nearest_distance = 10000000.
        ### 你的代码 ###
        for i in range(len(graph)):
            distance = np.linalg.norm(graph[i].pos - point)
            if distance < nearest_distance:
                nearest_distance = distance
                nearest_idx = i
        ### 你的代码 ###
        return nearest_idx, nearest_distance


    def connect_a_to_b(self, point_a, point_b):
        """
        以A点为起点，沿着A到B的方向前进STEP_DISTANCE的距离，并用self.map.checkline函数检查这段路程是否可以通过
        输入：
        point_a, point_b: 维度为(2,)的np.array，A点和B点位置，注意是从A向B方向前进
        输出：
        is_empty: bool，True表示从A出发前进STEP_DISTANCE这段距离上没有障碍物
        newpoint: 从A点出发向B点方向前进STEP_DISTANCE距离后的新位置，如果is_empty为真，之后的代码需要把这个新位置添加到图中
        """
        is_empty = False
        newpoint = np.zeros(2)
        ### 你的代码 ###
        #pdb.set_trace()
        will_hit, _ = self.map.checkline(point_a.tolist(), point_b.tolist())
        if not will_hit:
            is_empty = True
            newpoint = point_a + (point_b - point_a) * STEP_DISTANCE
        ### 你的代码 ###
        return is_empty, newpoint


    def find_neighbors(self, new_node: Node, graph: List[Node]):
        """
        Find nodes in the neighborhood of the new node.
        """
        N = len(graph) + 1
        r = RADIUS_FACTOR * ((np.log(N) / N) ** 0.5)  # Sphere radius
        nodes_near = [node for node in graph if np.linalg.norm(node.pos - new_node.pos) <= r]
        return nodes_near


    def choose_parent(self, new_node: Node, nodes_near: List[Node]):
        """
        Choose the node with the least cost to reach the new node.
        """
        cost_min = new_node.cost
        node_min = None
        for node in nodes_near:
            is_empty, _ = self.connect_a_to_b(node.pos, new_node.pos)
            if is_empty:
                cost = node.cost + np.linalg.norm(new_node.pos - node.pos)
                if cost < cost_min:
                    node_min = node
                    cost_min = cost
        return node_min, cost_min


    def rewire(self, new_node: Node, nodes_near: List[Node], graph: List[Node]):
        """
        Update the cost of reaching the nodes in the neighborhood through the new node.
        """
        for node in nodes_near:
            is_empty, _ = self.connect_a_to_b(new_node.pos, node.pos)
            if is_empty:
                cost = new_node.cost + np.linalg.norm(new_node.pos - node.pos)
                if cost < node.cost:
                    node.parent_idx = graph.index(new_node)
                    node.cost = cost