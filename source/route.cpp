//
// Created by acco on 7/23/18.
//

#include "route.hpp"
#include <iostream>
#include "xsttrp_instance.hpp"
#include <cassert>
#include <cfloat>

#define EMPTY (-1)

route::route(const xsttrp_instance &instance, int root, route_type type) {

    this->mLen = 1;
    this->mCapacity = instance.matrix_size;
    this->mCosts = instance.c;
    this->mLoads = instance.q;

    this->mRoot = root;
    this->mLoad = 0;//mLoads[mRoot];
    this->mCost = 0;
    this->mNext = new int[this->mCapacity];
    this->mPrev = new int[this->mCapacity];

    this->load_before = new int[instance.matrix_size];
    this->load_after = new int[instance.matrix_size];


    for (int i = 0; i < this->mCapacity; i++) {
        this->mNext[i] = EMPTY;
        this->mPrev[i] = EMPTY;
    }

    this->mNext[mRoot] = mRoot;
    this->mPrev[mRoot] = mRoot;
    this->mType = type;

}

route::route(const route &route)  {
    mLen = route.mLen;
    mCapacity = route.mCapacity;
    mCosts = route.mCosts;
    mLoads = route.mLoads;
    mRoot = route.mRoot;
    mLoad = route.mLoad;
    mNext = new int[mCapacity];
    mPrev = new int[mCapacity];
    load_before = new int[mCapacity];
    load_after = new int[mCapacity];
    for (int i = 0; i < this->mCapacity; i++) {
        mNext[i] = route.mNext[i];
        mPrev[i] = route.mPrev[i];
    }
    mCost = route.mCost;

    mType = route.mType;
}

route::~route() {
    delete[] this->mNext;
    delete[] this->mPrev;
    delete[] this->load_after;
    delete[] this->load_before;
}

route &route::operator=(const route &route) {

    mLen = route.mLen;
    mCapacity = route.mCapacity;
    mCosts = route.mCosts;
    mLoads = route.mLoads;
    mRoot = route.mRoot;
    mLoad = route.mLoad;

    for (int i = 0; i < this->mCapacity; i++) {
        mNext[i] = route.mNext[i];
        mPrev[i] = route.mPrev[i];
    }

    mCost = route.mCost;

    mType = route.mType;

    return *this;
}

void route::print() const {

    std::cout << mRoot << " ";
    auto curr = mNext[mRoot];
    while(curr != mRoot) {
        std::cout << curr << " ";
        curr = mNext[curr];
    }
    std::cout << mRoot << "\n";

}

void route::reset(int root, route_type type) {
    this->mRoot = root;
    this->mLen = 1;
    this->mLoad = 0;//mLoads[mRoot];
    this->mCost = 0;
    for (int i = 0; i < this->mCapacity; i++) {
        this->mNext[i] = EMPTY;
        this->mPrev[i] = EMPTY;
    }
    this->mNext[mRoot] = mRoot;
    this->mPrev[mRoot] = mRoot;
    this->mType = type;
}

int route::begin() const {
    return this->mRoot;
}

int route::next(const int vertex) const {
    assert(contains(vertex));
    return this->mNext[vertex];
}

int route::prev(const int vertex) const {
    assert(contains(vertex));
    return this->mPrev[vertex];
}

int route::next(const int vertex, unsigned times) const {
    int succ = vertex;
    for (; times; --times) {
        succ = next(succ);
    }
    return succ;
}

int route::prev(const int vertex, unsigned times) const {
    int pred = vertex;
    for (; times; --times) {
        pred = prev(pred);
    }
    return pred;
}

int route::size() const {
    return this->mLen;
}

bool route::contains(int vertex) const {
    assert(vertex >=0 && vertex <= mCapacity);
    return this->mPrev[vertex] != EMPTY;
}

void route::reverse(int vertex1, int vertex2) {

    assert(contains(vertex1) && contains(vertex2));

    const int prevVertex1 = prev(vertex1);
    const int nextVertex2 = next(vertex2);


    int curr = vertex1;
    do {
        const int tmp = next(curr);
        mNext[curr] = prev(curr);
        mPrev[curr] = tmp;
        curr = tmp;
    } while (curr != nextVertex2);

    if (vertex1 == nextVertex2) {
        // full path reverse => nothing more to do

    } else {

        mNext[prevVertex1] = vertex2;
        mPrev[vertex2] = prevVertex1;

        mPrev[nextVertex2] = vertex1;
        mNext[vertex1] = nextVertex2;

        mCost += -mCosts[prevVertex1][vertex1] - mCosts[vertex2][nextVertex2] + mCosts[prevVertex1][vertex2] +
                 mCosts[vertex1][nextVertex2];
    }


}

/*void route::change_root(int vertex) {

    assert(contains(vertex));
    this->mLoad -= mLoads[vertex];
    this->mLoad += mLoads[this->mRoot];
    this->mRoot = vertex;

}*/

void route::insert(int before, int vertex) {

    assert(contains(before) && !contains(vertex));

    const int beforePrev = mPrev[before];

    mPrev[before] = vertex;
    mNext[vertex] = before;
    mPrev[vertex] = beforePrev;
    mNext[beforePrev] = vertex;
    this->mLen++;

    this->mLoad += mLoads[vertex];
    this->mCost += -mCosts[beforePrev][before] + mCosts[beforePrev][vertex] + mCosts[vertex][before];

}

void route::append(int vertex) {
    return insert(mRoot, vertex);
}

float route::remove(const int vertex) {

    assert(contains(vertex) && vertex != mRoot);

    const int prevVertex = mPrev[vertex];
    const int nextVertex = mNext[vertex];

    mNext[prevVertex] = nextVertex;
    mPrev[nextVertex] = prevVertex;

    mPrev[vertex] = EMPTY;
    mNext[vertex] = EMPTY;

    this->mLen--;

    this->mLoad += -mLoads[vertex];
    float delta = -mCosts[prevVertex][vertex] - mCosts[vertex][nextVertex] + mCosts[prevVertex][nextVertex];
    this->mCost += delta;

    return delta;
}

bool route::is_empty() const {
    return this->size() == 1 && mType != main_route;
}

float route::find_best_insertion(int vertex, int &before_this_vertex) const {

    float cost = FLT_MAX;
    before_this_vertex = -1;

    auto curr = mRoot;
    auto next = mNext[curr];

    do {

        const auto delta = + mCosts[curr][vertex]
                           + mCosts[vertex][next]
                           - mCosts[curr][next];

        if (delta < cost) {
            cost = delta;
            before_this_vertex = next;
        }

        curr = next;
        next = mNext[next];
    } while(curr != mRoot);

    return cost;
}

float route::cost() const {
    return static_cast<float>(this->mCost);
}

short route::load() const {
    return this->mLoad;
}

void route::swap_vertices(route *iRoute, int i, route *jRoute, int j, float delta) {

    if(iRoute == jRoute){

        const auto iNext = iRoute->next(i);
        const auto iPrev = iRoute->prev(i);

        const auto jNext = iRoute->next(j);
        const auto jPrev = iRoute->prev(j);

        assert(!(j == iNext || i == iPrev));

        iRoute->mNext[jPrev] = i;
        iRoute->mNext[i] = jNext;

        iRoute->mNext[iPrev] = j;
        iRoute->mNext[j] = iNext;

        iRoute->mPrev[i] = jPrev;
        iRoute->mPrev[iNext] = j;

        iRoute->mPrev[j] = iPrev;
        iRoute->mPrev[jNext] = i;

        iRoute->mCost += delta;

    } else {

        const int nextv1 = iRoute->next(i);
        iRoute->remove(i);
        jRoute->insert(j, i);
        jRoute->remove(j);
        iRoute->insert(nextv1, j);


    }



}

route_type route::get_type() const {
    return this->mType;
}

void route::compute_cumulative_load() {

    assert(mLen > 1);

    const auto root = mRoot;

    auto prev = mNext[mRoot];

    load_before[prev] = mLoads[prev];
    load_after[prev] = mLoad;

    auto curr = mNext[prev];

    while(curr != root) {

        load_before[curr] = load_before[prev] + mLoads[curr];
        load_after[curr] = load_after[prev] - mLoads[prev];

        prev = curr;
        curr = mNext[curr];
    }


}

int route::get_load_before(const int vertex) const {
    assert(contains(vertex));
    return load_before[vertex];
}

int route::get_load_after(const int vertex) const {
    assert(contains(vertex));
    return load_after[vertex];
}

int route::get_last_vertex() const {
    return mPrev[mRoot];
}

int route::get_first_vertex() const {
    return mNext[mRoot];
}

void route::print(std::ofstream& ofstream) const {

    ofstream << mRoot << " ";
    auto curr = mNext[mRoot];
    while(curr != mRoot) {
        ofstream << curr << " ";
        curr = mNext[curr];
    }
    ofstream << mRoot << "\n";


}

float route::remove_root() {

    const auto vertex = mRoot;

    const int prevVertex = mPrev[vertex];
    const int nextVertex = mNext[vertex];

    mNext[prevVertex] = nextVertex;
    mPrev[nextVertex] = prevVertex;

    mPrev[vertex] = EMPTY;
    mNext[vertex] = EMPTY;

    float delta = -mCosts[prevVertex][vertex] - mCosts[vertex][nextVertex] + mCosts[prevVertex][nextVertex];
    this->mCost += delta;

    // just to have an entry point for the route
    mRoot = nextVertex;

    return delta;
}

void route::set_root(int vertex) {
    assert(contains(vertex));
    this->mLoad -= mLoads[vertex];
    this->mLen--;
    mRoot = vertex;
}

void route::set_root(int before, int vertex) {
    assert(contains(before) && !contains(vertex));

    const int beforePrev = mPrev[before];

    mPrev[before] = vertex;
    mNext[vertex] = before;
    mPrev[vertex] = beforePrev;
    mNext[beforePrev] = vertex;

    this->mCost += -mCosts[beforePrev][before] + mCosts[beforePrev][vertex] + mCosts[vertex][before];

    mRoot = vertex;
}

