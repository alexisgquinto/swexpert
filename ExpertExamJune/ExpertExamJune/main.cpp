//
//  main.cpp
//  ExpertExamJune
//
//  Created by Alexis Quinto on 6/4/22.
//

#define TANK_MAX_LEN 1000000
#define HEAP_MAX_LEN 350000
#define PARENT(i) (i - 1) / 2
#define LEFT(i) (i * 2) + 1
#define RIGHT(i) (i * 2) + 2

#include <cstdio>
#include <stdlib.h>

using namespace std;

int computeAbs(int x) {
    if (x < 0) return x * -1;
    return x;
}

int computeDistance(int x1, int y1, int x2, int y2) {
    return computeAbs(x1 - x2) + computeAbs(y1 - y2);
}

class Tank {
public:
    int x;
    int y;
    int index;
    int distance;
    int* basePriority;
    int currBase;
    
    Tank() {
    }
    
    Tank(int x, int y, int index, int distance, int *basePriority, int currBase) {
        this->x = x;
        this->y = y;
        this->index = index;
        this->distance = distance;
        this->basePriority = basePriority;
        this->currBase = currBase;
    }
    
    void computePrioDistance(int newBase, int **base) {
        this->distance = computeDistance(this->x, this->y, base[newBase][0], base[newBase][1]);
        this->currBase = newBase;
    }
};

class MaxHeap {
public:
    int size;
    Tank* tanks[HEAP_MAX_LEN];
    
    MaxHeap() {
        size = 0;
        for (int i = 0; i < HEAP_MAX_LEN; ++i) {
            tanks[i] = new Tank();
        }
    }
    
    void siftDown(int currIdx) {
        int l = LEFT(currIdx);
        int r = RIGHT(currIdx);
        
        while(l < size || r < size) {
            int largest = currIdx;
            if (l < size && tanks[largest]->distance < tanks[l]->distance) {
                largest = l;
            }
            if (r < size && tanks[largest]->distance < tanks[r]->distance) {
                largest = r;
            }
            
            if (currIdx != largest) {
                Tank* tmpTank = tanks[largest];
                tanks[largest] = tanks[currIdx];
                tanks[currIdx] = tmpTank;
                
                currIdx = largest;
                l = LEFT(currIdx);
                r = RIGHT(currIdx);
            } else {
                break;
            }
        }
    }
    
    void siftUp(int currIdx) {
        int p = PARENT(currIdx);
        while(currIdx > 0 && tanks[p]->distance < tanks[currIdx]->distance) {
            Tank* tmpTank = tanks[p];
            tanks[p] = tanks[currIdx];
            tanks[currIdx] = tmpTank;
            
            currIdx = p;
            p = PARENT(currIdx);
        }
    }
    
    Tank* peekFromHeap() {
        return tanks[0];
    }
    
    Tank* removeFromHeap() {
        if (size == 0) return nullptr;
        
        Tank* tmpTank = tanks[0];
        tanks[0] = tanks[size - 1];
        tanks[size - 1] = tmpTank;
        
        Tank* toBeRemoved = tanks[size - 1];
        --size;
        
        siftDown(0);
        return toBeRemoved;
    }
    
    void insertToHeap(Tank *t) {
        if (size >= HEAP_MAX_LEN) return;
        
        tanks[size] = t;
        siftUp(size);
        ++size;
    }
    
};



void test(int **tanks, int **base, int *arrange) {
    MaxHeap *base0 = new MaxHeap();
    MaxHeap *base1 = new MaxHeap();
    MaxHeap *base2 = new MaxHeap();
    
    for (int c = 0; c < TANK_MAX_LEN; ++c) {
        int baseDis0 = computeDistance(tanks[c][0], tanks[c][1], base[0][0], base[0][1]);
        int baseDis1 = computeDistance(tanks[c][0], tanks[c][1], base[1][0], base[1][1]);
        int baseDis2 = computeDistance(tanks[c][0], tanks[c][1], base[2][0], base[2][1]);
        
        int *priority = { new int[3] {0,1,2} };
        
        if (baseDis0 > baseDis2) {
            int tmp = priority[0];
            priority[0] = priority[2];
            priority[2] = tmp;
        }
        
        if (baseDis0 > baseDis1) {
            int tmp = priority[0];
            priority[0] = priority[1];
            priority[1] = tmp;
        }
        
        if (baseDis1 > baseDis2){
            int tmp = priority[1];
            priority[1] = priority[2];
            priority[2] = tmp;
        }
        
        
        
        Tank *newTank = new Tank(tanks[c][0], tanks[c][1], c, 0, priority, -1);
        bool assigned = false;
        
        while(!assigned && newTank->currBase < 3) {
            newTank->currBase++;
            newTank->computePrioDistance(newTank->currBase, base);
            
            if (newTank->basePriority[newTank->currBase] == 0) {
                if (base0->size < HEAP_MAX_LEN) {
                    base0->insertToHeap(newTank);
                    assigned = true;
                } else {
                    if (base0->peekFromHeap()->distance > newTank->distance) {
                        Tank *removedTank = base0->removeFromHeap();
                        base0->insertToHeap(newTank);
                        newTank = removedTank;
                    }
                }
            } else if (newTank->basePriority[newTank->currBase] == 1) {
                if (base1->size < HEAP_MAX_LEN) {
                    base1->insertToHeap(newTank);
                    assigned = true;
                } else {
                    if (base1->peekFromHeap()->distance > newTank->distance) {
                        Tank *removedTank = base1->removeFromHeap();
                        base1->insertToHeap(newTank);
                        newTank = removedTank;
                    }
                }
            } else {
                if (base2->size < HEAP_MAX_LEN) {
                    base2->insertToHeap(newTank);
                    assigned = true;
                } else {
                    if (base2->peekFromHeap()->distance > newTank->distance) {
                        Tank *removedTank = base2->removeFromHeap();
                        base2->insertToHeap(newTank);
                        newTank = removedTank;
                    }
                }
            }
        }
    }
    
    while(base0->size > 0) {
        Tank* tank = base0->removeFromHeap();
        arrange[tank->index] = 0;
    }
    
    while(base1->size > 0) {
        Tank* tank = base1->removeFromHeap();
        arrange[tank->index] = 1;
    }
    
    while(base2->size > 0) {
        Tank* tank = base2->removeFromHeap();
        arrange[tank->index] = 0;
    }
}

int computeResult(int **tanks, int **base, int *arrange) {
    int ret = 0;
    
    for (int c = 0; c < TANK_MAX_LEN; ++c) {
        if (arrange[c] == 0) {
            ret += computeDistance(tanks[c][0], tanks[c][1], base[0][0], base[0][1]);
        } else if (arrange[c] == 1) {
            ret += computeDistance(tanks[c][0], tanks[c][1], base[1][0], base[1][1]);
        } else {
            ret += computeDistance(tanks[c][0], tanks[c][1], base[2][0], base[2][1]);
        }
    }
    
    return ret;
}

int main () {
    int **tanks = new int*[TANK_MAX_LEN];
    int **base = new int*[3];
    int *arrange = new int[TANK_MAX_LEN];
    
    
    for (int i = 0; i < TANK_MAX_LEN; ++i) {
        tanks[i] = new int(2);
        tanks[i][0] = rand() % 10000;
        tanks[i][1] = rand() % 10000;
    }
    
    for (int i = 0; i < 3; ++i) {
        base[i] = new int(2);
        base[i][0] = rand() % 10000;
        base[i][1] = rand() % 10000;
    }
    
    test(tanks, base, arrange);
    
    printf("Result: %d\n", computeResult(tanks, base, arrange));
    
    return 0;
}
