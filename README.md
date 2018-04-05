# uva_new
This repository contains a solution to a problem in UVa online judge.

## Introduction:
This question is asking for an optimal solution to the pushing box game.

## Data Structure Used:
* A 4-dimension graph storing the states of box position and pusher position.
* Since the goal is to achieve the optimum on both pushes and walks, it is algorithmically difficult to consider movements of box and pusher separately. 
One feasible approach is to combine the states of box position and pusher position into one general state, which consists of a 4-dimension graph. 

## Algorithm Used:
* To discover the shortest path between starting state and terminal state, higher privilege is assigned to box movement, which guarantees the box movement will always be shortest. 
* An extended BFS will be used to calculate the minimal movement cost for each state. Each time the algorithm chooses the state with minimal cost and extends all its frontiers to a priority queue. 
* Once the target is found, it must be the optimal solution and path reconstruction can be realized through the back-trace method.
