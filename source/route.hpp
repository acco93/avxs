//
// Created by acco on 7/23/18.
//

#ifndef XSTTRP_ROUTE_HPP
#define XSTTRP_ROUTE_HPP


#include "globals.hpp"
#include "route_type.hpp"

#include <vector>
#include <fstream>

class xsttrp_instance;



class route {

public:


    route(const xsttrp_instance &instance, int root, route_type type);

    route(const route &route);

    ~route();

    route &operator=(const route &other);

    /*
    * Returns the root vertex.
    */
    int begin() const;

    /*
    * Returns the successor of the given vertex.
    */
    int next(int vertex) const;

    /*
    * Returns the predecessor of the given vertex.
    */
    int prev(int vertex) const;

    /*
    * Returns the times-successor of the given vertex.
    */
    int next(int vertex, unsigned int times) const;

    /*
    * Returns the times-predecessor of the given vertex.
    */
    int prev(int vertex, unsigned int times) const;

    /*
    * Returns the lenght of the route as the number of vertices + 1.
    */
    int size() const;

    /*
    * Returns true if the given vertex is contained in the route.
    */
    bool contains(int vertex) const;

    /*
    * Reverses the sub-path between vertex1 and vertex2 included.
    */
    void reverse(int vertex1, int vertex2);

    /*
    * Sets vertex as the new root.
    */
    //void change_root(int vertex);

    float remove_root();
    void set_root(int vertex);

    /*
    * Insert vertex into the route before where.
    */
    void insert(int where, int vertex);

    /*
    * Append vertex before the root node.
    */
    void append(int vertex);

    /*
    * Removes vertex. Returns the delta cost.
    */
    float remove(int vertex);

    /*
    * Returns if the route contains the root vertex only.
    */
    bool is_empty() const;

    /*
    * Identify the best (cheapest) insertion position in the current solution.
    */
    float find_best_insertion(int vertex, int &before_this_vertex) const;

    void print() const;

    void reset(int root, route_type type);

    /*
    * Swap i and j
    */
    static void swap_vertices(route *iRoute, int i, route *jRoute, int j, float delta);

    /*
    * Returns the route cost.
    */
    float cost() const;

    /*
    * Returns the route load.
    */
    short load() const;

    /*
     * Returns the route type.
     */
    route_type get_type() const;

    /*
     * Computes the cumulative load.
     */
    void compute_cumulative_load();

    int get_load_before(int vertex) const;
    int get_load_after(int vertex) const;

    int get_last_vertex() const;

    int get_first_vertex() const;

    void print(std::ofstream& ofstream) const;

    void set_root(int where, int root);

private:

    int mRoot;
    int mCapacity;
    int mLen;
    int *mNext;
    int *mPrev;
    float **mCosts;
    short *mLoads;
    int *load_before;
    int *load_after;
    short mLoad;
    double mCost;
    route_type mType;

};


#endif //XSTTRP_ROUTE_HPP
