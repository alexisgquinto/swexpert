//
//  main.cpp
//  SWC_Expert_Nov
//
//  Created by Alexis Quinto on 11/29/21.
//

#include <iostream>

using namespace std;

#define MAX_SIZE 50
#define COMPRESSION_SIZE 15000000

char m_graph[MAX_SIZE][MAX_SIZE];
char graph[MAX_SIZE][MAX_SIZE];
char m_data[COMPRESSION_SIZE];
char m_result[MAX_SIZE][MAX_SIZE];

/* Solution starts here */
void print(char canvas[MAX_SIZE][MAX_SIZE]) {
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            printf("%d", canvas[i][j]);
        }
        printf("\n");
    }
}

short getHorizontalLength(short r, short c, char gmap[MAX_SIZE][MAX_SIZE]) {
    short length = 0;
    while (gmap[r][c + length] == 1) {
        ++length;
    }
    return length;
}

short getVerticalLength(short r, short c, char gmap[MAX_SIZE][MAX_SIZE]) {
    short length = 0;
    while (gmap[r + length][c] == 1) {
        ++length;
    }
    
    return length;
}

void clearLine(short r, short c, short len, char canvas[MAX_SIZE][MAX_SIZE]) {
    int counter = 0;
    if (len > MAX_SIZE) { // vertical
        len -= MAX_SIZE;
        while (counter < len) {
            canvas[r + counter][c] = 0;
            ++counter;
        }
    } else { // horizontal
        while (counter < len) {
            canvas[r][c + counter] = 0;
            ++counter;
        }
    }
}

void compress(char f_graph[MAX_SIZE][MAX_SIZE], char f_data[COMPRESSION_SIZE]) {
    /*
     Sample pointer hack:
     char temp[20];
    
    short a = 1;
    short b = 1;
    short c = 7;
    
    short *pointer = (short*)(&temp[3]);
    *pointer = a;
    pointer += 2;
    
    *pointer = b;
    pointer += 2;
    
    *pointer = c;
    
    short *newpointer = (short*)(&temp[3]);
    printf("%d %d %d", *newpointer, *(newpointer+2), *(newpointer+4));*/
    
    short index = 0;
    for (short i = 0; i < MAX_SIZE; ++i) {
        for (short j = 0; j < MAX_SIZE; ++j) {
            if (f_graph[i][j] == 1) {
                short r = i;
                short c = j;
                short lengthH = getHorizontalLength(r, c, f_graph);
                short lengthV = getVerticalLength(r, c, f_graph);
                short finalLength = lengthH > lengthV ? lengthH : lengthV + MAX_SIZE;

                //save it to the f_data
                short *shortPointer = (short*)(&f_data[index]);
                *shortPointer = r;
                
                shortPointer += 2;
                *shortPointer = c;
                
                shortPointer += 2;
                *shortPointer = finalLength;
                
                index += 6;
                
                printf("Compressing[%d]: %d %d %d\n", index - 6, r, c, finalLength > MAX_SIZE ? finalLength - MAX_SIZE : finalLength);
                
                clearLine(r, c, finalLength, f_graph);
                //print(f_graph);
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

void decompress(char f_result[MAX_SIZE][MAX_SIZE], char f_data[COMPRESSION_SIZE]) {
    for (int i = 0; i < COMPRESSION_SIZE; i += 6) {
        short *pointer = (short*)(&f_data[i]);
        short r = *pointer;
        
        pointer += 2;
        short c = *pointer;
        
        pointer += 2;
        short length = *pointer;
        
        printf("Decompressing[%d]: %d %d %d\n", i, r, c, length > MAX_SIZE ? length - MAX_SIZE : length);
        
        if (length <= 0) {
            break;
        } else if (length > MAX_SIZE) {
            drawVertical(r, c, length - MAX_SIZE, f_result);
        } else {
            drawHorizontal(r, c, length, f_result);
        }
        
        //print(f_result);
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
