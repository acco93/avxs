TEST PROBLEMS FOR THE SINGLE TRUCK AND TRAILER ROUTING PROBLEM

Here you will find 32 test problems for the Single truck and Trailer Routing Problem with Satellite Depots  (STTRP) as described in:

J.G. Villegas, C.Prins, C. Prodhon, A.L. Medaglia and N. Velasco. GRASP/VND and Multi-Start Evolutionary Local Search for the Single Truck and Trailer Routing Problem with Satellite Depots. 2009 

The name of the files is constructed with the following convention:

STTRP-n-p-Q-x 

where:

n: number of customers
p: number of satelite depots
Q: Capacity of the truck divided by 1000
x: indicates the distribution of the points: rd for randomly distributed, and c for clustered points


The structure of the files is as follows:

- The first line indicates the size of the problem: number of customers (n) and number of depots (p+1)

- The second line has the capacities of the truck and the trailer

- p+1 lines with the x and y coordinates of the depots (the first line is for the main depot and the next correspond to the satellite depots): 

- n lines with the x and y coordinates of the customers and their demands. 


To calculate the cost matrix we use the Euclidean distance between any two points (p1=(x1,y1) and p2=(x2,y2)), using the following formula:

((x1-x2)^2 +(y1-y2)^2)^0.5 



