#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <conio.h>

//n1 = 2, n2 = 1, n3 = 1, n4 = 2
//Кількість вершин - 11
//Розміщення - прямокутник (квадрат)

#define EXTENT 15
#define GRAPH_STEP 330
#define SEED 2112
#define ROWS 3
#define COLS 4
#define X_DISTANCE 130
#define Y_DISTANCE 100
#define ELLIPSE_WIDTH 44
#define ELLIPSE_HEIGHT 22
#define RADIUS 30
#define OFFSET 22
#define DTX 5


LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

__attribute__((unused)) HWND button_draw_direct, button_draw_undirected, button_power, button_check_for_reg, button_isolated_or_pendant, button_draw_A2, button_steps, button_reachability, button_str_connected_matrix,
        button_component, button_condensate;

void arrow(float rotate_angle, int arrow_x, int arrow_y, HDC hdc)
{
    int left_x, left_y, right_x, right_y;
    left_x = arrow_x -  EXTENT * cos(rotate_angle + 0.3);
    right_x = arrow_x - EXTENT * cos(rotate_angle - 0.3);
    left_y = arrow_y - EXTENT * sin(rotate_angle + 0.3);
    right_y = arrow_y - EXTENT * sin(rotate_angle - 0.3);
    MoveToEx(hdc, arrow_x, arrow_y, NULL);
    LineTo(hdc, left_x, left_y);
    MoveToEx(hdc, arrow_x, arrow_y, NULL);
    LineTo(hdc, right_x, right_y);
}

void draw_arrow(int start_x, int start_y, int end_x, int end_y, int radius, HDC hdc)
{
    float to_rad = atan2((end_y - start_y), (end_x - start_x));
    float arrow_x = end_x - radius * cos(to_rad);
    float arrow_y = end_y - radius * sin(to_rad);
    arrow(to_rad, arrow_x, arrow_y, hdc);
}

float** random_matrix(int n)
{
    srand(SEED);
    float** arr = (float**)malloc(n * sizeof(float*));
    for(int i = 0; i < n; i++){
        arr[i] = (float*)malloc(n * sizeof(float));
        for(int j = 0; j < n; j++){
            arr[i][j] = ((float)rand() / (float)RAND_MAX) * 2.0;
        }
    }
    return arr;
}

float** mult_matrix(float coef, float** matrix, int n)
{
    float** res = (float**)malloc(n * sizeof(float*));
    for(int i = 0; i < n; i++){
        res[i] = (float*)malloc(n * sizeof(float));
        for(int j = 0; j < n; j++){
            res[i][j] = floor(matrix[i][j] * coef);
        }
    }
    return res;
}

char** create_symbol_array(int N)
{
    char** array = malloc(N * sizeof(char*));
    for(int i = 0; i < N; i++) {
        array[i] = malloc(3 * sizeof(char));
        sprintf(array[i], "%d", i+1);
    }
    return array;
}

float** make_symmetric_matrix(float** matrix, int N)
{
    float** res = (float**)malloc(N * sizeof(float*));
    for(int i = 0; i < N; i++){
        res[i] = (float*)malloc(N * sizeof(float));
        for(int j = 0; j < N; j++){
            if(matrix[i][j] == 1 || matrix[j][i] == 1){
                res[i][j] = 1;
            } else {
                res[i][j] = 0;
            }
        }
    }
    return res;
}

void print_matrix(int N, float** matrix)
{
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            printf("%.0f ", matrix[i][j]);
        }
        printf("\n");
    }
}

void print_int_array(int N, int* array)
{
    for(int i = 0; i < N; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
}

int verify_for_regularity(int N, int* array)
{
    for(int i = 0; i < N; i++){
        if(array[0] == array[i]) continue;
        else return 0;
    }
    return 1;
}

void print_regularity(int flag, int* array)
{
    if(flag == 0){
        printf("-\n");
    } else if(flag == 1){
        printf("YES\nPower of regularity is: %d\n", array[1]);
    }
}

void array_x(int N, int* array)
{
    int index = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            array[index] = 100 + j * X_DISTANCE + GRAPH_STEP;
            index++;
        }
    }
}

void array_y(int N, int* array)
{
    int index = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            array[index] = 100 + i * Y_DISTANCE;
            index++;
        }
    }
}

void draw_undirected_ellipses(int i, int j, int nx[], int ny[], HDC hdc, int edge_сeil)
{
    int dir = (int) ceil((i+1)/(float) edge_сeil);
    if(dir%2 == 0){
        if(dir > edge_сeil){
            Ellipse(hdc, nx[i]-ELLIPSE_WIDTH, ny[i]-ELLIPSE_HEIGHT, nx[i], ny[i]+ELLIPSE_HEIGHT);
        } else{
            Ellipse(hdc, nx[i]+ELLIPSE_WIDTH, ny[i]-ELLIPSE_HEIGHT, nx[i], ny[i]+ELLIPSE_HEIGHT);
        }
    } else{
        if(dir >= edge_сeil){
            Ellipse(hdc, nx[i]-ELLIPSE_HEIGHT, ny[i]+ELLIPSE_WIDTH, nx[i]+ELLIPSE_HEIGHT, ny[i]);
        } else{
            Ellipse(hdc, nx[i]-ELLIPSE_HEIGHT, ny[i]-ELLIPSE_WIDTH, nx[i]+ELLIPSE_HEIGHT, ny[i]);
        }
    }
}

void draw_ellipses(int i, int j, HDC hdc, int nx[], int ny[], int dx, int edge_сeil)
{
    if(i == j){
        int dir = (int) ceil((i + 1) / (float) edge_сeil);
        if(dir % 2 == 0){
            if(dir > edge_сeil){
                Ellipse(hdc, nx[i]-ELLIPSE_WIDTH, ny[i]-ELLIPSE_HEIGHT, nx[i], ny[i]+ELLIPSE_HEIGHT);
                draw_arrow(nx[i]-RADIUS, ny[i]+ELLIPSE_WIDTH, nx[j], ny[j], dx, hdc);
            } else{
                Ellipse(hdc, nx[i]+ELLIPSE_WIDTH, ny[i]-ELLIPSE_HEIGHT, nx[i], ny[i]+ELLIPSE_HEIGHT);
                draw_arrow(nx[i]+RADIUS, ny[i]-ELLIPSE_WIDTH, nx[j], ny[j], dx, hdc);
            }
        } else{
            if(dir >= edge_сeil){
                Ellipse(hdc, nx[i]-ELLIPSE_HEIGHT, ny[i]+ELLIPSE_WIDTH, nx[i]+ELLIPSE_HEIGHT, ny[i]);
                draw_arrow(nx[i]+ELLIPSE_WIDTH, ny[i]+ELLIPSE_WIDTH, nx[j], ny[j], dx, hdc);
            } else{
                Ellipse(hdc, nx[i]-ELLIPSE_HEIGHT, ny[i]-ELLIPSE_WIDTH, nx[i]+ELLIPSE_HEIGHT, ny[i]);
                draw_arrow(nx[i]-ELLIPSE_WIDTH, ny[i]-ELLIPSE_WIDTH, nx[j], ny[j], dx, hdc);
            }
        }
    }
}

void draw_undirected_same_row_lines(int i, int j, int nx[], int ny[], HDC hdc, int edge_сeil)
{
    if(nx[i] == nx[j]){
        MoveToEx(hdc, nx[i], ny[i], NULL);
        LineTo(hdc, nx[j]+RADIUS, ny[i]-(ny[i]-ny[j])/2);
        MoveToEx(hdc, nx[j]+RADIUS, ny[i]-(ny[i]-ny[j])/2, NULL);
        LineTo(hdc, nx[j], ny[j]);
    } else{
        MoveToEx(hdc, nx[i], ny[i], NULL);
        LineTo(hdc, nx[j]+(nx[i]-nx[j])/2, ny[i]-RADIUS);
        MoveToEx(hdc, nx[j]+(nx[i]-nx[j])/2, ny[i]-RADIUS, NULL);
        LineTo(hdc, nx[j], ny[j]);
    }
}

void draw_same_row_lines(int i, int j, HDC hdc, int nx[], int ny[], int dx)
{
    if(nx[i] == nx[j]){
        if(i > j){
            MoveToEx(hdc, nx[i], ny[i], NULL);
            LineTo(hdc, nx[j]+RADIUS, ny[i]-(ny[i]-ny[j])/2);
            MoveToEx(hdc, nx[j]+RADIUS, ny[i]-(ny[i]-ny[j])/2, NULL);
            LineTo(hdc, nx[j], ny[j]);
            draw_arrow(nx[j]+RADIUS, ny[i]-(ny[i]-ny[j])/2, nx[j], ny[j], dx, hdc);
        } else{
            MoveToEx(hdc, nx[i], ny[i], NULL);
            LineTo(hdc, nx[j]-RADIUS, ny[i]-(ny[i]-ny[j])/2);
            MoveToEx(hdc, nx[j]-RADIUS, ny[i]-(ny[i]-ny[j])/2, NULL);
            LineTo(hdc, nx[j], ny[j]);
            draw_arrow(nx[j]-RADIUS, ny[i]-(ny[i]-ny[j])/2, nx[j], ny[j], dx, hdc);
        }
    } else{
        if(i > j){
            MoveToEx(hdc, nx[i], ny[i], NULL);
            LineTo(hdc, nx[j]+(nx[i]-nx[j])/2, ny[i]+RADIUS);
            MoveToEx(hdc, nx[j]+(nx[i]-nx[j])/2, ny[i]+RADIUS, NULL);
            LineTo(hdc, nx[j], ny[j]);
            draw_arrow(nx[j]+(nx[i]-nx[j])/2, ny[i]+RADIUS, nx[j], ny[j], dx, hdc);

        } else{
            MoveToEx(hdc, nx[i], ny[i], NULL);
            LineTo(hdc, nx[j]+(nx[i]-nx[j])/2, ny[i]-RADIUS);
            MoveToEx(hdc, nx[j]+(nx[i]-nx[j])/2, ny[i]-RADIUS, NULL);
            LineTo(hdc, nx[j], ny[j]);
            draw_arrow(nx[j]+(nx[i]-nx[j])/2, ny[i]-RADIUS, nx[j], ny[j], dx, hdc);
        }
    }
}

void draw_undirected_lines(int i, int j, HDC hdc, int nx[], int ny[])
{
    MoveToEx(hdc, nx[i], ny[i], NULL);
    LineTo(hdc, nx[j], ny[j]);
}

void draw_lines(int i, int j, HDC hdc, int nx[], int ny[], int dx, float** A)
{
    if(i > j && A[j][i] == 1){
        MoveToEx(hdc, nx[i], ny[i], NULL);
        LineTo(hdc, (nx[i]+nx[j])/2, (ny[i]+ny[j])/2+ELLIPSE_HEIGHT);
        MoveToEx(hdc, (nx[i]+nx[j])/2, (ny[i]+ny[j])/2+ELLIPSE_HEIGHT, NULL);
        LineTo(hdc, nx[j], ny[j]);
        draw_arrow((nx[i]+nx[j])/2, (ny[i]+ny[j])/2+ELLIPSE_HEIGHT, nx[j], ny[j], dx, hdc);
    } else if(ny[i] == ny[j]){
        MoveToEx(hdc, nx[i], ny[i], NULL);
        LineTo(hdc, (nx[i]+nx[j])/2+ELLIPSE_HEIGHT, (ny[i]+ny[j])/2);
        MoveToEx(hdc, (nx[i]+nx[j])/2+ELLIPSE_HEIGHT, (ny[i]+ny[j])/2, NULL);
        LineTo(hdc, nx[j], ny[j]);
        draw_arrow((nx[i]+nx[j])/2+ELLIPSE_HEIGHT, (ny[i]+ny[j])/2, nx[j], ny[j], dx, hdc);
    } else{
        MoveToEx(hdc, nx[i], ny[i], NULL);
        LineTo(hdc, nx[j], ny[j]);
        draw_arrow(nx[i], ny[i], nx[j], ny[j], dx, hdc);
    }
}

void draw_undirected_graph(HWND hWnd, HDC hdc, const int N, char** nn, int nx[], int ny[], float** Matrix)
{
    int edge_сeil = ceil(N / 4.0);
    HPEN KPen = CreatePen(PS_SOLID, 1, RGB(20, 20, 5));
    SelectObject(hdc, KPen);

    printf("Undirected Matrix matrix:\n");
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(Matrix[j][i] == 1 || Matrix[i][j] == 1){
                printf("1 ");
            } else printf("%.0f ", Matrix[i][j]);
        }
        printf("\n");
    }
    Matrix[0][9] = 1;
    int last_row = 2;
    int last_col = 3;
    int temp_x = nx[10];
    int temp_y = ny[10];
    nx[10] = nx[last_row * 4 + last_col];
    ny[10] = ny[last_row * 4 + last_col];
    nx[last_row * 4 + last_col] = temp_x;
    ny[last_row * 4 + last_col] = temp_y;
    Matrix[9][0] = 0;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(Matrix[i][j] == 1){
                if(i == j){
                    draw_undirected_ellipses(i, j, nx, ny, hdc, edge_сeil);
                }
                else if(((abs(i-j) >=2 && abs(i-j) <= edge_сeil) || abs(i-j) >= 3*edge_сeil) && (nx[i] == nx[j] || ny[i] == ny[j])){
                    draw_undirected_same_row_lines(i,j,nx,ny,hdc,edge_сeil);
                }
                else{
                    draw_undirected_lines(i,j,hdc,nx,ny);
                }
            }
        }
    }
    HPEN RPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));

    SelectObject(hdc, RPen);
    for(int i = 0; i < N; i++){
        Ellipse(hdc, nx[i] - OFFSET, ny[i] - OFFSET, nx[i] + OFFSET, ny[i] + OFFSET);
        TextOut(hdc, nx[i] - DTX, ny[i] - OFFSET / 2, nn[i], 2);
    }
}

void draw_graph(HWND hWnd, HDC hdc, int N, int nx[], int ny[], char** nn, float** A)
{
    int edge_сeil = ceil(N / 4.0);

    printf("Adjacency matrix:\n");
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            printf("%.0f ", A[i][j]);
        }
        printf("\n");
    }
    A[0][9] = 1;
    int last_row = 2;
    int last_col = 3;
    int temp_x = nx[10];
    int temp_y = ny[10];
    nx[10] = nx[last_row * 4 + last_col];
    ny[10] = ny[last_row * 4 + last_col];
    nx[last_row * 4 + last_col] = temp_x;
    ny[last_row * 4 + last_col] = temp_y;
    A[9][0] = 0;
    int sumOfLines = 0;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(A[i][j] == 1){
                sumOfLines++;
                draw_ellipses(i, j, hdc, nx, ny, OFFSET, edge_сeil);
                if(((abs(i-j) >=2 && abs(i-j) <= edge_сeil) || abs(i-j) >= 3*edge_сeil) && (nx[i] == nx[j] || ny[i] == ny[j])){
                    draw_same_row_lines(i, j, hdc, nx, ny, OFFSET);
                }
                else if(i!=j){
                    draw_lines(i, j, hdc, nx, ny, OFFSET, A);
                }
            }
        }
    }

    printf("Number of all lines: %d\n", sumOfLines);

    HPEN RPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    HPEN KPen = CreatePen(PS_SOLID, 1, RGB(20, 20, 5));

    SelectObject(hdc, RPen);
    for(int i = 0;i < N; i++){
        Ellipse(hdc, nx[i] - OFFSET, ny[i] - OFFSET, nx[i] + OFFSET, ny[i] + OFFSET);
        TextOut(hdc, nx[i] - DTX, ny[i] - OFFSET / 2, nn[i], 2);
    }
}

void draw_isolated_or_pendant(HWND hWnd, HDC hdc, int N, int nx[], int ny[], char** nn, int* pen)
{
    HPEN BPen = CreatePen(PS_SOLID, 2, RGB(50, 0, 255));
    HPEN YPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 0));

    for(int i = 0; i < N; i++){
        if(pen[i] == 0){
            SelectObject(hdc, BPen);
            Ellipse(hdc, nx[i] - OFFSET, ny[i] - OFFSET, nx[i] + OFFSET, ny[i] + OFFSET);
            TextOut(hdc, nx[i] - DTX, ny[i] - OFFSET / 2, nn[i], 2);
        } else if(pen[i] == 1){
            SelectObject(hdc, YPen);
            Ellipse(hdc, nx[i] - OFFSET, ny[i] - OFFSET, nx[i] + OFFSET, ny[i] + OFFSET);
            TextOut(hdc, nx[i] - DTX, ny[i] - OFFSET / 2, nn[i], 2);
        }
    }
}

int* undirected_isolated_or_pendant(int N, int* array)
{
    int* res = (int*) malloc(N * sizeof(int));

    for(int i = 0; i < N; i++){
        if(array[i] == 0){
            res[i] = 0;
        } else if(array[i] == 1){
            res[i] = 1;
        } else res[i] = 2;
    }

    return res;
}

void print_isolated_or_pendant(int N, int* array)
{

    printf("\nIsolated:\n");
    for(int i = 0; i < N; i++){
        if(array[i] == 0){
            printf("%d  ", (i+1));
        }
    }
    printf("\n\nPendant:\n");
    for(int i = 0; i < N; i++){
        if(array[i] == 1){
            printf("%d  ", (i+1));
        }
    }
}

void free_memory(float** matrix, int N)
{
    if (matrix != NULL) {
        for (int i = 0; i < N; i++) {
            free(matrix[i]);
        }
        free(matrix);
    }
}

int* calc_outcome(int N, float** matrix)
{
    int* res = (int*) malloc(N * sizeof(int));
    int count = 0;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (matrix[i][j] == 1) count++;
        }
        res[i] = count;
    }

    return res;
}

int* calc_income(int N, float** matrix)
{
    int* res = (int*) malloc(N * sizeof(int));

    for (int i = 0; i < N; i++) {
        res[i] = 0;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (matrix[i][j] == 1) {
                res[j]++;
            }
        }
    }

    return res;
}

int* power_of_undirected_graph(int N, float** matrix)
{
    int* res = malloc(N * sizeof(int));
    int count;

    for(int i = 0; i < N; i++){
        count = 0;
        for(int j = 0; j < N; j++){
            if(matrix[i][j] == 1.0) count++;
        }
        res[i] = count;
    }

    return res;
}

float** mult_matrices(float** matrix1, float** matrix2, int N)
{
    float** res = (float**)malloc(N * sizeof(float*));

    for (int i = 0; i < N; i++) {
        res[i] = (float*)malloc(N * sizeof(float));
        for (int j = 0; j < N; j++) {
            res[i][j] = 0;
            for (int a = 0; a < N; a++) {
                res[i][j] += matrix1[i][a] * matrix2[a][j];
            }
        }
    }

    return res;
}

float** power_of_matrix(float** matrix, int N, int power)
{
    float** res = (float**)malloc(N * sizeof(float*));

    for (int i = 0; i < N; i++) {
        res[i] = (float*)malloc(N * sizeof(float));
        for (int j = 0; j < N; j++) {
            res[i][j] = matrix[i][j];
        }
    }

    for (int i = 1; i < power; i++) {
        float** temp = mult_matrices(res, matrix, N);
        for (int j = 0; j < N; j++) {
            free(res[j]);
        }
        free(res);
        res = temp;
    }

    return res;
}

float** transpose_matrix(float** matrix, int rows, int cols)
{
    float** res = (float**)malloc(cols * sizeof(float*));
    for (int i = 0; i < cols; i++) {
        res[i] = (float*)malloc(rows * sizeof(float));
        for (int j = 0; j < rows; j++) {
            res[i][j] = matrix[j][i];
        }
    }
    return res;
}

float** mult_matrices_straight(float** matrix1, float** matrix2, int N)
{
    float** res = (float**)malloc(N * sizeof(float*));

    for (int i = 0; i < N; i++) {
        res[i] = (float*)malloc(N * sizeof(float));
        for (int j = 0; j < N; j++) {
            res[i][j] = matrix1[i][j] * matrix2[i][j];
        }
    }

    return res;
}

void paths_of_length_two(float** adj_matrix, int N)
{

    printf("Paths of length two:\n");

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (adj_matrix[i][j] == 1) {
                for (int a = 0; a < N; a++) {
                    if (adj_matrix[j][a] == 1) {
                        printf("%d -> %d -> %d\n", (i+1), (j+1), (a+1));
                    }
                }
            }
        }
    }
}

void paths_of_length_three(float** adj_matrix, int N)
{

    printf("Paths of length three:\n");

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int a = 0; a < N; a++) {
                if (adj_matrix[i][j] == 1 && adj_matrix[j][a] == 1) {
                    for (int b = 0; b < N; b++) {
                        if (adj_matrix[a][b] == 1) {
                            printf("%d -> %d -> %d -> %d\n", (i+1), (j+1), (a+1), (b+1));
                        }
                    }
                }
            }
        }
    }
}

float** matrix_of_reachability(float** adj_matrix, int N)
{
    float** reachability_matrix = (float**)malloc(N * sizeof(float*));

    for (int i = 0; i < N; i++) {
        reachability_matrix[i] = (float*)malloc(N * sizeof(float));
        for (int j = 0; j < N; j++) {
            if (adj_matrix[i][j] > 0 || i == j)
                reachability_matrix[i][j] = 1;
            else
                reachability_matrix[i][j] = 0;
        }
    }

    for (int a = 0; a < N; a++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                reachability_matrix[i][j] = reachability_matrix[i][j] || (reachability_matrix[i][a] && reachability_matrix[a][j]);
            }
        }
    }

    return reachability_matrix;
}

float** strongly_connected_matrix(float** adj_matrix, int N)
{
    float** trans = transpose_matrix(adj_matrix, N, N);
    float** res = mult_matrices_straight(adj_matrix, trans, N);

    free_memory(trans, N);

    return res;
}

void depth_first_search(int vertex, float** adj_matrix, int* visited, int N)
{
    visited[vertex] = 1;
    printf("%d ", vertex + 1);

    for (int i = 0; i < N; i++) {
        if (adj_matrix[vertex][i] != 0 && !visited[i]) {
            depth_first_search(i, adj_matrix, visited, N);
        }
    }
}

void print_connected_components(float** adj_matrix, int N)
{
    int* visited = (int*)calloc(N, sizeof(int));

    for (int i = 0; i < N; i++) {
        if (!visited[i]) {
            printf("Component: ");
            depth_first_search(i, adj_matrix, visited, N);
            printf("\n");
        }
    }

    free(visited);
}

void depth_first_search2(float** adj_matrix, int N, int vertex, int* visited, int* labels, int* indices, int component)
{
    visited[vertex] = 1;
    labels[vertex] = component;

    for (int i = 0; i < N; i++) {
        if (adj_matrix[vertex][i] != 0 && !visited[i]) {
            depth_first_search2(adj_matrix, N, i, visited, labels, indices, component);
        }
    }
}

float** condensed_graph(float** adj_matrix, int N, int* count)
{
    int* visited = (int*)calloc(N, sizeof(int));
    int* labels = (int*)malloc(N * sizeof(int));
    int* indices = (int*)calloc(N, sizeof(int));
    int component = 0;

    for (int i = 0; i < N; i++) {
        if (!visited[i]) {
            component++;
            depth_first_search2(adj_matrix, N, i, visited, labels, indices, component);
        }
    }

    int number_of_components = component;
    float** cond_graph = (float**)malloc(number_of_components * sizeof(float*));
    for (int i = 0; i < number_of_components; i++) {
        cond_graph[i] = (float*)calloc(number_of_components, sizeof(float));
    }

    for (int i = 0; i < N; i++) {
        int componentLabel = labels[i];
        for (int j = 0; j < N; j++) {
            if (adj_matrix[i][j] != 0 && componentLabel != labels[j]) {
                cond_graph[componentLabel - 1][labels[j] - 1] = 1.0;
            }
        }
    }

    free(visited);
    free(labels);
    free(indices);

    *count = number_of_components;
    return cond_graph;
}


//main function from which we call all needed function onclick of buttons. Also this function response all of calculations and let hem in argument of functions
void main_func(int option, HWND hWnd, HDC hdc)
{
    const int N = 11;
    int nx[N], ny[N];
    int flag;
    int components;

    array_x(N, nx);
    array_y(N, ny);

    char** nn = create_symbol_array(N);
    float** T = random_matrix(N);
    float** A = mult_matrix(0.66, T, N);//Fill our matrix
    float** A2 = mult_matrix(0.71, T, N);
    float** A2_power2 = power_of_matrix(A2, N, 2);
    float** A2_power3 = power_of_matrix(A2, N, 3);
    float** symA = make_symmetric_matrix(A, N);
    float** symA2 = make_symmetric_matrix(A2, N);
    float** reachability_matrix = matrix_of_reachability(A2, N);
    float** strong_con_mat = strongly_connected_matrix(reachability_matrix, N);
    float** condensate = condensed_graph(A2, N, &components);
    int* undirected_power = power_of_undirected_graph(N, symA);
    int* outcoming = calc_outcome(N, A);
    int* incoming = calc_income(N, A);
    int* isolated_or_pendant = undirected_isolated_or_pendant(N, undirected_power);

    switch(option){
        case 1:
            draw_graph(hWnd, hdc, N, nx, ny, nn, A);
            break;
        case 2:
            draw_undirected_graph(hWnd, hdc, N, nn, nx, ny, A);
            break;
        case 3:
            printf("Directed graph: \n");
            print_matrix(N, A);
            printf("Half degree of incoming:\n");
            print_int_array(N, incoming);
            printf("Half degree of outcoming:\n");
            print_int_array(N, outcoming);
            printf("\n");
            printf("Undirected graph:\n");
            print_matrix(N, symA);
            printf("Power of vertexes in undirected graph:\n");
            print_int_array(N, undirected_power);
            break;
        case 4:
            printf("Is undirected graph is regular: ");
            flag = verify_for_regularity(N, undirected_power);
            print_regularity(flag, undirected_power);
            printf("Is directed graph is regular for incoming: ");
            flag = verify_for_regularity(N, outcoming);
            print_regularity(flag, incoming);
            printf("Is directed graph is regular for outcoming: ");
            flag = verify_for_regularity(N, outcoming);
            print_regularity(flag, outcoming);
            break;
        case 5:
            draw_undirected_graph(hWnd, hdc, N, nn, nx, ny, A);
            draw_isolated_or_pendant(hWnd, hdc, N, nx, ny, nn, isolated_or_pendant);
            print_isolated_or_pendant(N, isolated_or_pendant);
            break;
        case 6:
            draw_graph(hWnd, hdc, N, nx, ny, nn, A2);
            free(undirected_power);
            free(outcoming);
            free(incoming);
            undirected_power = power_of_undirected_graph(N, symA2);
            outcoming= calc_outcome(N, A2);
            incoming = calc_income(N, A2);
            printf("Half degree of incoming:\n");
            print_int_array(N, incoming);
            printf("Half degree of outcoming:\n");
            print_int_array(N, outcoming);
            printf("\n");
            printf("Undirected graph:\n");
            print_matrix(N, symA2);
            printf("Power of vertexes in undirected graph:\n");
            print_int_array(N, undirected_power);
            break;
        case 7:
            printf("(A2)^2:\n");
            print_matrix(N, A2_power2);
            printf("(A2)^3:\n");
            print_matrix(N, A2_power3);
            draw_graph(hWnd, hdc, N, nx, ny, nn, A2);
            paths_of_length_two(A2, N);
            paths_of_length_three(A2, N);
            break;
        case 8:
            printf("Reachability matrix:\n");
            print_matrix(N, reachability_matrix);
            draw_graph(hWnd, hdc, N, nx, ny, nn, A2);
            break;
        case 9:
            printf("Strongly connected matrix:\n");
            print_matrix(N, strong_con_mat);
            draw_graph(hWnd, hdc, N, nx, ny, nn, A2);
            break;
        case 10:
            draw_graph(hWnd, hdc, N, nx, ny, nn, A2);
            printf("\n");
            print_connected_components(A2, N);
            break;
        case 11:
            print_connected_components(A2, N);
            draw_graph(hWnd, hdc, components, nx, ny, nn, condensate);
            break;
        default:
            break;
    }

    free(nn);
    free(outcoming);
    free(incoming);
    free(undirected_power);
    free(isolated_or_pendant);
    free_memory(T, N);
    free_memory(A, N);
    free_memory(A2, N);
    free_memory(A2_power2, N);
    free_memory(A2_power3, N);
    free_memory(symA, N);
    free_memory(symA2, N);
    free_memory(reachability_matrix, N);
    free_memory(strong_con_mat, N);
    free_memory(condensate, components);
}
void windowUpdate(HWND hWnd, HDC hdc, PAINTSTRUCT ps, int option)
{
    InvalidateRect(hWnd, NULL, TRUE);
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
    UpdateWindow(hWnd);
    system("cls");
    hdc = BeginPaint(hWnd, &ps);
    FillRect(hdc, &ps.rcPaint, hBrush);
    main_func(option, hWnd, hdc);
    EndPaint(hWnd, &ps);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpszCmdLine, int nCmdShow) {
    WNDCLASS window;
    window.lpszClassName = "Lab 4";
    window.hInstance = hInstance;
    window.lpfnWndProc = WindowProc;
    window.hCursor = LoadCursor(NULL, IDC_ARROW);
    window.hIcon = 0;
    window.lpszMenuName = 0;
    window.hbrBackground = WHITE_BRUSH;
    window.style = CS_HREDRAW | CS_VREDRAW;
    window.cbClsExtra = 0;
    window.cbWndExtra = 0;
    if (!RegisterClass(&window)) {
        printf("Error! Window was not registered!");
        return 0;
    }
    HWND hWnd;
    MSG lpMsg;
    hWnd = CreateWindow("Lab 4",
                        "Lab 4 by Sofiia Krytska IM-21",
                        WS_OVERLAPPEDWINDOW,
                        15,
                        15,
                        1250,
                        650,
                        (HWND) NULL,
                        (HMENU) NULL,
                        (HINSTANCE) hInstance,
                        (HINSTANCE) NULL);
    ShowWindow(hWnd, nCmdShow);
    int b;
    while((b = GetMessage(&lpMsg, hWnd, 0, 0))!= 0) {
        if(b == -1)	{
            return lpMsg.wParam;
        }
        else {
            TranslateMessage(&lpMsg);
            DispatchMessage(&lpMsg);
        }
    }

    return(lpMsg.wParam);
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    switch(message){
        case WM_CREATE:

            button_draw_direct = CreateWindow("BUTTON",
                                            "Draw directed graph",
                                            WS_VISIBLE | WS_CHILD | WS_BORDER,
                                            270, 340, 290, 50,
                                            hWnd, (HMENU) 1, NULL, NULL);
            button_draw_undirected = CreateWindow("BUTTON",
                                         "Draw undirected graph",
                                         WS_VISIBLE | WS_CHILD | WS_BORDER,
                                         680, 340, 290, 50,
                                         hWnd, (HMENU) 2, NULL, NULL);
            button_power = CreateWindow("BUTTON",
                                       "Power graph",
                                       WS_VISIBLE | WS_CHILD | WS_BORDER,
                                       70, 400, 290, 50,
                                       hWnd, (HMENU) 3, NULL, NULL);
            button_check_for_reg = CreateWindow("BUTTON",
                                          "Check for regular graph",
                                          WS_VISIBLE | WS_CHILD | WS_BORDER,
                                          70, 460, 290, 50,
                                          hWnd, (HMENU) 4, NULL, NULL);
            button_isolated_or_pendant = CreateWindow("BUTTON",
                                              "Show isolated and pendant",
                                              WS_VISIBLE | WS_CHILD | WS_BORDER,
                                              70, 520, 290, 50,
                                              hWnd, (HMENU) 5, NULL, NULL);
            button_draw_A2 = CreateWindow("BUTTON",
                                       "Draw A2 and show power",
                                       WS_VISIBLE | WS_CHILD | WS_BORDER,
                                       480, 400, 290, 50,
                                       hWnd, (HMENU) 6, NULL, NULL);
            button_steps = CreateWindow("BUTTON",
                                      "Show all 2 and 3 steps",
                                      WS_VISIBLE | WS_CHILD | WS_BORDER,
                                      480, 460, 290, 50,
                                      hWnd, (HMENU) 7, NULL, NULL);
            button_reachability = CreateWindow("BUTTON",
                                           "A2 matrix of reachability",
                                           WS_VISIBLE | WS_CHILD | WS_BORDER,
                                           480, 520, 290, 50,
                                           hWnd, (HMENU) 8, NULL, NULL);
            button_str_connected_matrix = CreateWindow("BUTTON",
                                                      "Strongly connected matrix",
                                                      WS_VISIBLE | WS_CHILD | WS_BORDER,
                                                      880, 400, 290, 50,
                                                      hWnd, (HMENU) 9, NULL, NULL);
            button_component = CreateWindow("BUTTON",
                                           "Components of str. connection",
                                           WS_VISIBLE | WS_CHILD | WS_BORDER,
                                           880, 460, 290, 50,
                                           hWnd, (HMENU) 10, NULL, NULL);
            button_condensate = CreateWindow("BUTTON",
                                           "Graph of condensate",
                                           WS_VISIBLE | WS_CHILD | WS_BORDER,
                                           880, 520, 290, 50,
                                           hWnd, (HMENU) 11, NULL, NULL);
            break;
        case WM_COMMAND:

            switch(LOWORD(wParam)){
                case 1:
                    windowUpdate(hWnd, hdc, ps, 1);
                    break;
                case 2:
                    windowUpdate(hWnd, hdc, ps, 2);
                    break;
                case 3:
                    windowUpdate(hWnd, hdc, ps, 3);
                    break;
                case 4:
                    windowUpdate(hWnd, hdc, ps, 4);
                    break;
                case 5:
                    windowUpdate(hWnd, hdc, ps, 5);
                    break;
                case 6:
                    windowUpdate(hWnd, hdc, ps, 6);
                    break;
                case 7:
                    windowUpdate(hWnd, hdc, ps, 7);
                    break;
                case 8:
                    windowUpdate(hWnd, hdc, ps, 8);
                    break;
                case 9:
                    windowUpdate(hWnd, hdc, ps, 9);
                    break;
                case 10:
                    windowUpdate(hWnd, hdc, ps, 10);
                    break;
                case 11:
                    windowUpdate(hWnd, hdc, ps, 11);
                    break;
            }
            break;
        case WM_PAINT:
            UpdateWindow(hWnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}