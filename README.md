PC Components used for the test :
CPU : Intel i5 13th gen
RAM : 32 Go DDR5

FPS Min Max :
1 Boids : 4500 - 5500 FPS
10 Boids : 3800 - 5000 FPS
100 Boids : 2900 - 3500 FPS
500 Boids : 700 - 850 FPS
1000 Boids : 300 - 500 FPS

Improvement Needed :

Upgrade dijkstra pathfinding :
The current dijkstra pathfinding has the same distance given between each node as the distance is not computed since the node are made procedurally. so the program need to compute the distance 
when updating the dijkstra node to permit more linear path and less useless diagonals.

Pathfinding Computing freeze :
Using multi threading to find paths for more boids at the same time or delays the multiple pathfinding function to avoid doing all the path at the same time
