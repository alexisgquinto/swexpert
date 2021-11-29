//
//  main.cpp
//  SWC_Expert_Nov
//
//  Created by Alexis Quinto on 11/29/21.
//

#include <iostream>

using namespace std;

#define MAX_SIZE 50
#define COMPRESSION_SIZE 150000

char m_graph[MAX_SIZE][MAX_SIZE];
char graph[MAX_SIZE][MAX_SIZE];
char m_data[COMPRESSION_SIZE];
char m_result[MAX_SIZE][MAX_SIZE];

/* Solution starts here */

short getHorizontalLength(short r, short c, char gmap[MAX_SIZE][MAX_SIZE]) {
    short length = 1;
    short tmp = 0;
    while (gmap[r][c + tmp + 1] == 1) {
        gmap[r][c + tmp] = 0;
        ++length;
        ++tmp;
    }
    return length;
}

short getVerticalLength(short r, short c, char gmap[MAX_SIZE][MAX_SIZE]) {
    short length = 1;
    short tmp = 0;
    while (gmap[r + tmp + 1][c] == 1) {
        gmap[r + tmp][c] = 0;
        ++length;
        ++tmp;
    }
    
    return length;
}

void compress(char f_graph[MAX_SIZE][MAX_SIZE], char f_data[COMPRESSION_SIZE]) {
    int index = 0;
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            if (f_graph[i][j] == 1) {
                short r = i;
                short c = j;
                short lengthH = getHorizontalLength(r, c, f_graph);
                short lengthV = getVerticalLength(r, c, f_graph) + MAX_SIZE;
                f_graph[r][c] = 0;

                //save it to the f_data
                short *shortPointer;
                if (lengthH > 1) {
                    shortPointer = (short*)(&f_data[index]);
                    *shortPointer = r;
                    index += 2;
                    
                    shortPointer = (short*)(&f_data[index]);
                    *shortPointer = c;
                    index += 2;
                    
                    shortPointer = (short*)(&f_data[index]);
                    *shortPointer = lengthH;
                    index += 2;
                }
                
                if (lengthV > 1 || (lengthH == 1 && lengthV == 1)) {
                    shortPointer = (short*)(&f_data[index]);
                    *shortPointer = r;
                    index += 2;
                    
                    shortPointer = (short*)(&f_data[index]);
                    *shortPointer = c;
                    index += 2;
                    
                    shortPointer = (short*)(&f_data[index]);
                    *shortPointer = lengthV;
                    index += 2;
                }
            }
        }
    }
}

void drawHorizontal(short r, short c, short length, char f_result[MAX_SIZE][MAX_SIZE]) {
    int counter = 0;
    while (counter != length) {
        f_result[r][c + counter] = 1;
        ++counter;
    }
}

void drawVertical(short r, short c, short length, char f_result[MAX_SIZE][MAX_SIZE]) {
    int counter = 0;
    while (counter != length) {
        f_result[r + counter][c] = 1;
        ++counter;
    }
}

void print(char canvas[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            printf("%d", canvas[i][j]);
        }
        printf("\n");
    }
}

void decompress(char f_result[MAX_SIZE][MAX_SIZE], char f_data[COMPRESSION_SIZE]) {
    for (int i = 0; i < COMPRESSION_SIZE; i += 6) {
        short *pointer1 = (short*)(&f_data[i + 0]);
        short r = *pointer1;
        
        short *pointer2 = (short*)(&f_data[i + 2]);
        short c = *pointer2;
        
        short *pointer3 = (short*)(&f_data[i + 4]);
        short length = *pointer3;
        
        printf("%d %d %d\n", r, c, length > MAX_SIZE ? length - MAX_SIZE : length);
        
        if (length <= 0) {
            break;
        } else if (length > MAX_SIZE) {
            drawVertical(r, c, length - MAX_SIZE, f_result);
        } else {
            drawHorizontal(r, c, length, f_result);
        }
        
        print(f_result);
    }
}
/* Solution ends here */

void build() {
    int sX, sY, eX, eY;
    
    for (int c = 0; c < MAX_SIZE; ++c) {
        sX = rand() % MAX_SIZE;
        sY = rand() % MAX_SIZE;
        eX = rand() % MAX_SIZE;
        eY = rand() % MAX_SIZE;
        
        if (eX < sX || eY < sY) {
            c -= 1;
            continue;
        }
        
        for (int i = sX; i <= eX; ++i) {
            m_graph[sY][i] = graph[sY][i] = 1;
            m_graph[eY][i] = graph[eY][i] = 1;
        }
        
        for (int i = sY + 1; i <= eY - 1; ++i) {
            m_graph[i][sX] = graph[i][sX] = 1;
            m_graph[i][eX] = graph[i][eX] = 1;
        }
        
    }
}

void compare() {
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            if (m_graph[i][j] != m_result[i][j]) {
                printf("There is unmatch cell!\n");
                return;
            }
        }
    }
    printf("Compression and decompression data matched successfully!\n");
}

int main(int argc, const char * argv[]) {
    //generate 10k x 10k random 1 and 0
    build();
    
    print(m_graph);
    printf("\n");
    
    //compress
    compress(graph, m_data);
    
    //clear the memory
    memset(graph, 0, MAX_SIZE * MAX_SIZE);
    
    //decompress
    decompress(m_result, m_data);
    
    print(m_result);
    printf("\n");
    
    //compare
    compare();
    
    return 0;
}
