# multiAgents.py
# --------------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


from util import manhattanDistance
from game import Directions
import random, util
from math import inf, sqrt, log 
from game import Agent
import numpy as npy
import time
from ghostAgents import GhostAgent
import layout

class ReflexAgent(Agent):
    """
      A reflex agent chooses an action at each choice point by examining
      its alternatives via a state evaluation function.

      The code below is provided as a guide.  You are welcome to change
      it in any way you see fit, so long as you don't touch our method
      headers.
    """

    def getAction(self, gameState):
        """
        You do not need to change this method, but you're welcome to.

        getAction chooses among the best options according to the evaluation function.

        Just like in the previous project, getAction takes a GameState and returns
        some Directions.X for some X in the set {North, South, West, East, Stop}
        """
        # Collect legal moves and successor states
        legalMoves = gameState.getLegalActions()
        # Choose one of the best actions
        scores = [self.evaluationFunction(
            gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [index for index in range(
            len(scores)) if scores[index] == bestScore]
        # Pick randomly among the best
        chosenIndex = random.choice(bestIndices)

        "Add more of your code here if you want to"

        return legalMoves[chosenIndex]

    def evaluationFunction(self, currentGameState, action):
        """
        Design a better evaluation function here.

        The evaluation function takes in the current and proposed successor
        GameStates (pacman.py) and returns a number, where higher numbers are better.

        The code below extracts some useful information from the state, like the
        remaining food (newFood) and Pacman position after moving (newPos).
        newScaredTimes holds the number of moves that each ghost will remain
        scared because of Pacman having eaten a power pellet.

        Print out these variables to see what you're getting, then combine them
        to create a masterful evaluation function.
        """
        successorGameState = currentGameState.generatePacmanSuccessor(action)
        newPos = successorGameState.getPacmanPosition() #(1, 8)
        newFood = successorGameState.getFood().asList() #[(1, 5), (1, 7), (2, 6), (2, 8), (3, 5), (3, 7)]
        newGhostStates = successorGameState.getGhostStates()
        newScaredTimes = [ghostState.scaredTimer 
                          for ghostState in newGhostStates] #[0, 0]
        ghostDistances = [manhattanDistance(newPos, ghost.configuration.pos)
                          for ghost in newGhostStates
                          if ghost.scaredTimer == 0]
        minGhostDist = min(ghostDistances, default=100)
        if minGhostDist == 0:
            return -inf
        numFood = successorGameState.getNumFood()
        if numFood == 0:
            return inf
        food = currentGameState.getFood()
        if food[newPos[0]][newPos[1]]:
            minFoodDist = 0
        else:
            foodDistances = [
                manhattanDistance(newPos, (x, y))
                for x in range(food.width)
                for y in range(food.height)
                if food[x][y]
            ]
            minFoodDist = min(foodDistances, default=0)

        danger = 1 / (minGhostDist - 0.8)
        profit = 1 / (minFoodDist + 0.5)
        score = -danger + profit
        return score

def scoreEvaluationFunction(currentGameState):
    """
      This default evaluation function just returns the score of the state.
      The score is the same one displayed in the Pacman GUI.

      This evaluation function is meant for use with adversarial search agents
      (not reflex agents).
    """
    score = currentGameState.getScore()
    return score
    """
    pos = currentGameState.getPacmanPosition()
    foods = currentGameState.getFood().asList()
    ghostStates = currentGameState.getGhostStates()
    ghostDistances = [manhattanDistance(pos, ghost.configuration.pos)
                      for ghost in ghostStates
                      if ghost.scaredTimer == 0]
    foodDistances = [manhattanDistance(pos, food) for food in foods]
    minGhostDist = min(ghostDistances, default=100)
    minFoodDist = min(foodDistances, default=0)
    danger = 1 / (minGhostDist - 0.8)
    profit = 1 / (minFoodDist + 0.5)
    score = currentGameState.getScore() -danger + profit
    """
    

class MultiAgentSearchAgent(Agent):
    """
      This class provides some common elements to all of your
      multi-agent searchers.  Any methods defined here will be available
      to the MinimaxPacmanAgent, AlphaBetaPacmanAgent & ExpectimaxPacmanAgent.

      You *do not* need to make any changes here, but you can if you want to
      add functionality to all your adversarial search agents.  Please do not
      remove anything, however.

      Note: this is an abstract class: one that should not be instantiated.  It's
      only partially specified, and designed to be extended.  Agent (game.py)
      is another abstract class.
    """

    def __init__(self, evalFn = 'scoreEvaluationFunction', depth = '2'):
        self.index = 0 # Pacman is always agent index 0
        self.evaluationFunction = util.lookup(evalFn, globals())
        self.depth = int(depth)
        

class MinimaxAgent(MultiAgentSearchAgent):
    """
      Your minimax agent (question 1)
    """

    def getAction(self, gameState):
        """
          Returns the minimax action from the current gameState using self.depth
          and self.evaluationFunction.
          
          Here are some method calls that might be useful when implementing minimax.
          
          gameState.getLegalActions(agentIndex):
            Returns a list of legal actions for an agent
            agentIndex=0 means Pacman, ghosts are >= 1
            
          gameState.generateSuccessor(agentIndex, action):
            Returns the successor game state after an agent takes an action
            
          gameState.getNumAgents():
            Returns the total number of agents in the game
        """
        "*** YOUR CODE HERE ***" 
        dep = self.depth
        agentNum = gameState.getNumAgents()
        def value(gameState, agentIndex, dep_cnt): 
            if dep_cnt == dep or gameState.isWin() or gameState.isLose():
                return self.evaluationFunction(gameState)
            if not agentIndex:
                return max_value(gameState, agentIndex, dep_cnt)
            if agentIndex:
                if agentIndex == agentNum - 1:
                    return min_value(gameState, agentIndex, dep_cnt + 1)
                return min_value(gameState, agentIndex, dep_cnt)
        def max_value(gameState, agentIndex, dep_cnt):
            v = float("-inf")
            legalActions = gameState.getLegalActions(agentIndex)
            for legalAction in legalActions:
                v = max( v, value( gameState.generateSuccessor(agentIndex, legalAction), (agentIndex + 1) % agentNum, dep_cnt ) )
            return v
        def min_value(gameState, agentIndex, dep_cnt):
            v = float("inf")
            legalActions = gameState.getLegalActions(agentIndex)
            for legalAction in legalActions:
                v = min( v, value( gameState.generateSuccessor(agentIndex, legalAction), (agentIndex + 1) % agentNum, dep_cnt ) )
            return v
        
        # Collect legal moves and successor states
        legalMoves = gameState.getLegalActions(0)
        successorGameStates = []
        for legalMove in legalMoves:
            successorGameStates.append( gameState.generateSuccessor(0, legalMove) )
        successor_values = [value(successorGameState, 1, 0) for successorGameState in successorGameStates]
        # Choose one of the best actions
        best_value = max(successor_values)
        best_indices = [index for index in range(len(successor_values)) if successor_values[index] == best_value]
        # Pick randomly among the best
        chosen_index = random.choice(best_indices)
        return legalMoves[chosen_index]

class AlphaBetaAgent(MultiAgentSearchAgent):
    """
      Your minimax agent with alpha-beta pruning (question 2)
    """

    def getAction(self, gameState):
        """
          Returns the minimax action using self.depth and self.evaluationFunction
        """
        "*** YOUR CODE HERE ***"
        dep = self.depth
        agentNum = gameState.getNumAgents()
        def value(gameState, agentIndex, dep_cnt, alpha, beta):
            if dep_cnt == dep or gameState.isWin() or gameState.isLose():
                return self.evaluationFunction(gameState)
            if not agentIndex:
                return max_value(gameState, agentIndex, dep_cnt, alpha, beta)
            if agentIndex:
                if agentIndex == agentNum - 1:
                    return min_value(gameState, agentIndex, dep_cnt + 1, alpha, beta)
                return min_value(gameState, agentIndex, dep_cnt, alpha, beta)
        def max_value(gameState, agentIndex, dep_cnt, alpha, beta):
            v = float("-inf")
            legalActions = gameState.getLegalActions(agentIndex)
            for legalAction in legalActions:
                v = max( v, value( gameState.generateSuccessor(agentIndex, legalAction), (agentIndex + 1) % agentNum, dep_cnt, alpha, beta ) )
                if v > beta:
                    return v
                alpha = max(alpha, v)
            return v
        def min_value(gameState, agentIndex, dep_cnt, alpha, beta):
            v = float("inf")
            legalActions = gameState.getLegalActions(agentIndex)
            for legalAction in legalActions:
                v = min( v, value( gameState.generateSuccessor(agentIndex, legalAction), (agentIndex + 1) % agentNum, dep_cnt, alpha, beta ) )
                if v < alpha:
                    return v
                beta = min(beta, v)
            return v
        
        # Collect legal moves and successor states
        legalMoves = gameState.getLegalActions(0)
        successorGameStates = []
        for legalMove in legalMoves:
            successorGameStates.append( gameState.generateSuccessor(0, legalMove) )
        successor_values = []
        alpha_ = float("-inf")
        for successorGameState in successorGameStates:
            successor_values.append( value(successorGameState, 1, 0, alpha_, float("inf")) )
            alpha_ = max(successor_values)
        # Choose one of the best actions
        best_value = max(successor_values)
        best_indices = [index for index in range(len(successor_values)) if successor_values[index] == best_value]
        # Pick randomly among the best
        chosen_index = random.choice(best_indices)
        return legalMoves[chosen_index]
        

class MCTSAgent(MultiAgentSearchAgent):
    """
      Your MCTS agent with Monte Carlo Tree Search (question 3)
    """

    def getAction(self, gameState):
        gameState.explored.clear()
        class Node:
            '''
            We have provided node structure that you might need in MCTS tree.
            '''
            def __init__(self, state, parent, parent_action):
                self.parent = parent
                self.parent_action = parent_action
                self.children = []
                self.state = state
                self.numerator = 0 #分子，pacman赢的次数
                self.denominator = 0 #分母，traverse该Node的总次数
                if self.parent != None:
                    self.agentIndex = (self.parent.agentIndex + 1) % agentNum
                else:
                    self.agentIndex = 0
                self.untried_actions = self.state.getLegalActions(self.agentIndex)
                
            def UCT(self, c = sqrt(2), p = 1000000):
                children_evaluations = [
                    ( cd.numerator / cd.denominator + c * sqrt( log(self.denominator) / cd.denominator)
                     + ( p /(cd.denominator) ) * cd.parent.evaluation_function(cd.parent_action) )
                    for cd in self.children]
                #这里给UCT加了一点点heuristic，以便在rollout次数少时更多按启发来选择节点，保障初期性能；
                #而rollout次数增加后heuristic的权重降低，逐渐转为按照原版MCTS思想选择节点
                #由于算力和时间限制
                best_index = npy.argmax(children_evaluations)
                return self.children[best_index]
            
            def select(self):
                current_node = self
                while not current_node.is_terminal():
                    if current_node.is_fully_expanded():
                        current_node = current_node.UCT()
                    else:
                        return current_node.expand()
                return current_node 
                    
            def expand(self):
                action = self.untried_actions.pop()
                new_state = self.state.generateSuccessor(self.agentIndex, action)
                child_node = Node(new_state, self, action)
                self.children.append(child_node)
                return child_node
            
            def rollout(self):
                current_node = self
                while not current_node.is_terminal():
                    legalActions = current_node.state.getLegalActions(current_node.agentIndex)
                    action = current_node.rollout_policy(legalActions)
                    new_state = current_node.state.generateSuccessor(current_node.agentIndex, action)
                    current_node = Node(new_state, current_node, action)
                return current_node.state.isWin()
            
            def rollout_policy(self, actions):
                #可以用random或heuristic
                #heuristic想法1：用minimax来做rollout? -> 暂时跑不起来不知道哪里写错了
                #heuristic想法2：估值函数！
                #heuristic想法3：epsilon-greedy！
                #目前版本 -> epsilon-greedy + 估值
                #（但不知道为什么不要epsilon或者说epsilon=0的时候跑得最好）
                epsilon = 0
                if not self.agentIndex:
                    if random.random() < epsilon:
                        chosen_action = random.choice(actions)
                    else:
                        evals = npy.array([self.evaluation_function(action) for action in actions])
                        chosen_action = actions[npy.argmax(evals)]
                else:
                    chosen_action = random.choice(actions)

                return chosen_action

            def backpropagate(self, is_win):
                self.denominator += 1
                if is_win:
                    self.numerator += 1
                if self.parent:
                    self.parent.backpropagate(is_win)

            def evaluation_function(self, action):
                successor_game_state = self.state.generateSuccessor(self.agentIndex, action)
                
                if successor_game_state.isWin():
                    return inf
                if successor_game_state.isLose():
                    return -inf
                
                new_pos = successor_game_state.getPacmanPosition()
                
                new_ghost_states = successor_game_state.getGhostStates()
                ghost_distances = [manhattanDistance(new_pos, ghost.configuration.pos) for ghost in new_ghost_states if ghost.scaredTimer == 0]
                min_ghost_dist = min(ghost_distances, default=10000)

                food = self.state.getFood()
                food_distances = [manhattanDistance(new_pos, (x, y)) for x in range(food.width) for y in range(food.height) if food[x][y]]
                min_food_dist = min(food_distances, default=10000)

                capsules = successor_game_state.getCapsules()
                capsule_distances = [manhattanDistance(new_pos, capsule) for capsule in capsules]
                min_capsule_dist = min(capsule_distances, default=10000)

                scared_ghost_distances = [manhattanDistance(new_pos, ghost.configuration.pos) for ghost in new_ghost_states if ghost.scaredTimer > 0]
                min_scared_ghost_dist = min(scared_ghost_distances, default=10000)
                nearest_scared_ghost_time = min([ghost.scaredTimer for ghost in new_ghost_states if ghost.scaredTimer > 0], default=0)
                
                remaining_capsules = len(capsules)
                remaining_food = successor_game_state.getNumFood()
                
                position_penalty = 0
                now_pos = self.state.getPacmanPosition()
                if new_pos == now_pos:
                    position_penalty += 0.1
                if self.parent is not None:
                    prev_pos = self.parent.state.getPacmanPosition()
                    if new_pos == prev_pos:
                        position_penalty += 0.1

                w1, w2, w3, w4, w5, w6 = 1, 1, 1, 1, 1/(nearest_scared_ghost_time + 1), 1
                
                profit = 0
                profit += w1 / (min_food_dist + 0.5) 
                profit += w2 / (min_capsule_dist + 0.4)
                profit += w3  / (remaining_capsules + 5) 
                profit += w4  / (remaining_food + 100) 
                profit += w5 * (nearest_scared_ghost_time - min_scared_ghost_dist) / (min_scared_ghost_dist + 0.2)
                danger = w6 / (min_ghost_dist - 0.8)
                
                evaluation = profit - danger - position_penalty
                return evaluation

            def is_fully_expanded(self):
                return len(self.untried_actions) == 0
            
            def is_terminal(self):
                return ( self.state.isWin() or self.state.isLose() )
            
            
        agentNum = gameState.getNumAgents()       
        root = Node(gameState, None, None)
        startTime = time.time()
        timeLimit = 1
        
        layout_width, layout_height = gameState.data.layout.width, gameState.data.layout.height
        if layout_width == 28 and layout_height == 27: 
            timeLimit = 1.33  #图太大了，需要更多搜索时间
        
        while time.time() - startTime < timeLimit:
            leaf = root.select()
            outcome = leaf.rollout()
            leaf.backpropagate(outcome)
        
        best_child = root.UCT(c = 0) #注意这里参数c要取0
        return best_child.parent_action
