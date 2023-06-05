#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <windows.h>
#include<conio.h>

#define RADIUS 35
#define DTX 5
#define EXTENT 15
#define GRAPH_STEP 330
#define SEED 2112
#define ROWS 3
#define COLS 4
#define X_DISTANCE 130
#define Y_DISTANCE 100
#define ELLIPSE_WIDTH 44
#define ELLIPSE_HEIGHT 22
#define OFFSET 22
#define DTX 5

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

HWND button_bfs, button_dfs, button_next_bfs, button_next_dfs;
bool waiting_button_bfs = true, waiting_button_dfs = true;

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

float** random_matrix(int N)
{
    srand(SEED);
    float** res = (float**)malloc(N * sizeof(float*));
    for(int i = 0; i < N; i++){
        res[i] = (float*)malloc(N * sizeof(float));
        for(int j = 0; j < N; j++){
            res[i][j] = ((float)rand() / (float)RAND_MAX) * 2.0;
        }
    }

    return res;
}

float** mult_matrix(float c, float** matrix, int N)
{
    float** res = (float**)malloc(N * sizeof(float*));
    for(int i = 0; i < N; i++){
        res[i] = (float*)malloc(N * sizeof(float));
        for(int j = 0; j < N; j++){
            res[i][j] = floor(matrix[i][j] * c);
        }
    }

    return res;
}

char** create_symbol_array(int N)
{
    char** res = malloc(N * sizeof(char*));
    for(int i = 0; i < N; i++) {
        res[i] = malloc(3 * sizeof(char));
        sprintf(res[i], "%d", i+1);
    }
    return res;
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

void print_matrix(int N, int** matrix)
{
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
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

    printf("Number lines: %d\n", sumOfLines);

    HPEN RPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    HPEN KPen = CreatePen(PS_SOLID, 1, RGB(20, 20, 5));

    SelectObject(hdc, RPen);
    for(int i = 0;i < N; i++){
        Ellipse(hdc, nx[i] - OFFSET, ny[i] - OFFSET, nx[i] + OFFSET, ny[i] + OFFSET);
        TextOut(hdc, nx[i] - DTX, ny[i] - OFFSET / 2, nn[i], 2);
    }
}

void draw_edging(HDC hdc, int x, int y, int radius, char state, int vertex_count)
{
    HBRUSH hBrush;
    COLORREF color;
    char label[10];

    switch (state) {
        case 'b':
            color = RGB(0, 255, 255);
            sprintf(label, "%d", vertex_count + 1);
            break;
        case 'e':
            color = RGB(0, 0, 128);
            sprintf(label, "%d", vertex_count + 1);
            break;
        case 'm':
            color = RGB(50, 0, 255);
            sprintf(label, "%d", vertex_count + 1);
            break;
        default:
            color = RGB(255, 255, 255);
            sprintf(label, "%d", vertex_count + 1);
            break;
    }

    hBrush = CreateSolidBrush(color);
    SelectObject(hdc, hBrush);

    int left = x - radius;
    int top = y - radius;
    int right = x + radius;
    int bottom = y + radius;

    Ellipse(hdc, left, top, right, bottom);
    FloodFill(hdc, x, y, color);

    SIZE text_size;
    GetTextExtentPoint32(hdc, label, strlen(label), &text_size);
    int text_x = x - text_size.cx / 2;
    int text_y = y - text_size.cy / 2;
    TextOut(hdc, text_x, text_y, label, strlen(label));
    DeleteObject(hBrush);
}

void draw_same_nx(int i, int j, HDC hdc, int* nx, int* ny, int radius)
{
    if (i > j) {
        MoveToEx(hdc, nx[i], ny[i], NULL);
        LineTo(hdc, nx[j] + RADIUS, ny[i] - (ny[i] - ny[j]) / 2);
        MoveToEx(hdc, nx[j] + RADIUS, ny[i] - (ny[i] - ny[j]) / 2, NULL);
        LineTo(hdc, nx[j], ny[j]);
        draw_arrow(nx[j] + RADIUS, ny[i] - (ny[i] - ny[j]) / 2, nx[j], ny[j], radius, hdc);
        draw_edging(hdc, nx[j], ny[j], radius, 'm', j);
        draw_edging(hdc, nx[i], ny[i], radius, 'b', i);
    }
    else {
        MoveToEx(hdc, nx[i], ny[i], NULL);
        LineTo(hdc, nx[j] - RADIUS, ny[i] - (ny[i] - ny[j]) / 2);
        MoveToEx(hdc, nx[j] - RADIUS, ny[i] - (ny[i] - ny[j]) / 2, NULL);
        LineTo(hdc, nx[j], ny[j]);
        draw_arrow(nx[j] - RADIUS, ny[i] - (ny[i] - ny[j]) / 2, nx[j], ny[j], radius, hdc);
        draw_edging(hdc, nx[j], ny[j], radius, 'm', j);
        draw_edging(hdc, nx[i], ny[i], radius, 'b', i);
    }
}

void draw_not_same_nx(int i, int j, HDC hdc, int* nx, int* ny, int radius)
{
    if (i > j) {
        MoveToEx(hdc, nx[i], ny[i], NULL);
        LineTo(hdc, nx[j] + (nx[i] - nx[j]) / 2, ny[i] + RADIUS);
        MoveToEx(hdc, nx[j] + (nx[i] - nx[j]) / 2, ny[i] + RADIUS, NULL);
        LineTo(hdc, nx[j], ny[j]);
        draw_arrow(nx[j] + (nx[i] - nx[j]) / 2, ny[i] + RADIUS, nx[j], ny[j], radius, hdc);
        draw_edging(hdc, nx[j], ny[j], radius, 'm', j);
        draw_edging(hdc, nx[i], ny[i], radius, 'b', i);
    }
    else {
        MoveToEx(hdc, nx[i], ny[i], NULL);
        LineTo(hdc, nx[j] + (nx[i] - nx[j]) / 2, ny[i] - RADIUS);
        MoveToEx(hdc, nx[j] + (nx[i] - nx[j]) / 2, ny[i] - RADIUS, NULL);
        LineTo(hdc, nx[j], ny[j]);
        draw_arrow(nx[j] + (nx[i] - nx[j]) / 2, ny[i] - RADIUS, nx[j], ny[j], radius, hdc);
        draw_edging(hdc, nx[j], ny[j], radius, 'm', j);
        draw_edging(hdc, nx[i], ny[i], radius, 'b', i);
    }
}

void draw_lines_transition(int i, int j, HDC hdc, int* nx, int* ny, int radius)
{
    MoveToEx(hdc, nx[i], ny[i], NULL);
    LineTo(hdc, nx[j], ny[j]);
    draw_arrow(nx[i], ny[i], nx[j], ny[j], radius, hdc);
    MoveToEx(hdc, nx[i], ny[i], NULL);
    draw_edging(hdc, nx[j], ny[j], radius, 'm', j);
    draw_edging(hdc, nx[i], ny[i], radius, 'b', i);
}

void draw_transition(HDC hdc, int N, int i, int j, int nx[], int ny[], int last, int first, int counter, int visited[])
{
    int edge_ceil = ceil(N / 4.0);
    int radius = 22;
    COLORREF color = RGB(0, 0, 128);
    int width = 2;
    HPEN hPen = CreatePen(PS_SOLID, width, color);
    SelectObject(hdc, hPen);

    if (i != first) {
        draw_edging(hdc, nx[last], ny[last], radius, 'm', last);
    }

    bool condition = ((abs(i - j) >= 2 && abs(i - j) <= edge_ceil) || abs(i - j) >= 3 * edge_ceil) && (nx[i] == nx[j] || ny[i] == ny[j]);

    if (condition) {
        if (nx[i] == nx[j]) {
            draw_same_nx(i,j,hdc,nx,ny,radius);
        }
        else {
            draw_not_same_nx(i,j,hdc,nx,ny,radius);
        }
    }
    else {
        draw_lines_transition(i,j,hdc,nx,ny,radius);
    }

    if (i != last) {
        draw_edging(hdc, nx[last], ny[last], radius, 'e', last);
    }

    DeleteObject(hPen);
}

void bfs(float** adj_matrix, int n, int first_node, HDC hdc, int nx[], int ny[], int* visited, int** tree_matrix)
{

    int* queue = (int*)malloc(n * sizeof(int));
    int* visited_from = (int*)malloc(n * sizeof(int));

    int front = 0;
    int last = first_node;
    int counter = 0;
    int index = 0;

    queue[index] = first_node;
    visited[first_node] = 1;
    visited_from[first_node] = first_node;

    while (front <= index) {
        counter++;
        int current = queue[front++];
        int from_vertex = visited_from[current];

        tree_matrix[from_vertex][current] = 1;
        printf("To vertex: %d. From vertex: %d\n", (current + 1), (from_vertex + 1));
        draw_transition(hdc, n, from_vertex, current, nx, ny, last, first_node, counter, queue);

        last = from_vertex;
        waiting_button_bfs = true;

        while (waiting_button_bfs) {
            MSG message;
            GetMessage(&message, NULL, 0, 0);
            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        for (int i = 0; i < n; i++) {
            if (adj_matrix[current][i] != 0 && visited[i] == 0) {
                queue[++index] = i;
                visited[i] = 1;
                visited_from[i] = current;
            }
        }
    }

    for(int i = 0; i < n; i++){
        if(visited[i] == 1){
            draw_edging(hdc, nx[i], ny[i], 22, 'e', i);
        }
    }

    for(int i = 0; i < n; i++){
        if(visited[i] != 1){
            bfs(adj_matrix, n, i, hdc, nx, ny, visited, tree_matrix);
        }
    }

    printf("Start numeration/ gotten numeration\n");
    for(int i = 0; i < n; i++){
        printf("\t%d / %d \n", (i+1), (queue[i]+1));
    }

    free(queue);
    free(visited_from);
}

void dfs(float** adj_matrix, int n, int first, HDC hdc, int nx[], int ny[], int* visited, int** tree_matrix)
{
    int* stack = malloc((n * 3) * sizeof(int));
    int* queue = malloc(n * sizeof(int));
    int top = -1;
    int last = first;
    int counter = 0;
    int* transition_from = malloc(n * sizeof(int));
    for(int i = 0; i < n; i++){
        transition_from[i] = first;
    }

    stack[++top] = first;

    while (top != -1) {
        counter++;
        int current = stack[top--];

        if (visited[current]) {
            continue;
        }

        visited[current] = 1;
        printf("Visited: %d ", (1 + current));
        tree_matrix[transition_from[current]][current] = 1;

        if (top >= 0) {
            printf("Transition from: %d\n", (1 + transition_from[current]));
            queue[counter-1] = (current);
        } else {
            printf("(start vertex)\n");
        }

        draw_transition(hdc, n, transition_from[current], current, nx, ny, last, first, counter, transition_from);
        last = transition_from[current];

        waiting_button_dfs = true;

        while(waiting_button_dfs){
            MSG msg;
            GetMessage(&msg, NULL, 0, 0);
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        for (int i = n - 1; i >= 0; i--) {
            if (adj_matrix[current][i] != 0 && !visited[i]) {
                stack[++top] = i;
                transition_from[i] = current;
            }

        }
    }

    for(int i = 0; i < n; i++){
        if(visited[i] == 1){
            draw_edging(hdc, nx[i], ny[i], 22, 'e', i);
        }
    }

    for(int i = 0; i < n; i++){
        if(visited[i] != 1){
            dfs(adj_matrix, n, i, hdc, nx, ny, visited, tree_matrix);
        }
    }

    printf("Start numeration/ gotten numeration\n");
    for(int i = 0; i < n; i++){
        printf("\t%d / %d \n", (i+1), (queue[i]+1));
    }


    free(queue);
    free(stack);
    free(transition_from);
}

void free_matrix(int** matrix, int N)
{
    if (matrix != NULL) {
        for (int i = 0; i < N; i++) {
            free(matrix[i]);
        }
        free(matrix);
    }
}

void main_func(int option, HWND hWnd, HDC hdc)
{
    const int N = 11;
    int nx[N], ny[N];

    int* visited = (int*)malloc(N * sizeof(int));
    int** tree_matrix = (int**)malloc(N * sizeof(int*));
    for (int i = 0; i < N; i++) {
        visited[i] = 0;
        tree_matrix[i] = (int*)malloc(N * sizeof(int));
        for(int j = 0; j < N; j++){
            tree_matrix[i][j] = 0;
        }
    }

    array_x(N, nx);
    array_y(N, ny);

    char** nn = create_symbol_array(N);
    float** rand_matrix = random_matrix(N);
    float** multifunctional_matrix = mult_matrix(0.825, rand_matrix, N);
    float** symmetric_matrix = make_symmetric_matrix(multifunctional_matrix, N);

    switch(option){
        case 1:
            draw_graph(hWnd, hdc, N, nx, ny, nn, multifunctional_matrix);
            bfs(multifunctional_matrix, N, 0, hdc, nx, ny, visited, tree_matrix);
            printf("\nMatrix tree BFS:\n");
            print_matrix(N, tree_matrix);
            break;
        case 2:
            draw_graph(hWnd, hdc, N, nx, ny, nn, multifunctional_matrix);
            dfs(multifunctional_matrix, N, 0, hdc, nx, ny, visited, tree_matrix);
            printf("\nMatrix tree DFS:\n");
            print_matrix(N, tree_matrix);
            break;
    }

    free(nn);
    free(visited);
    free_matrix(tree_matrix, N);
    free_matrix(rand_matrix, N);
    free_matrix(multifunctional_matrix, N);
    free_matrix(symmetric_matrix, N);
}

void window_update(HWND hWnd, HDC hdc, PAINTSTRUCT ps, int option)
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    WNDCLASS window;
    window.lpszClassName = "Lab 5";
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

    hWnd = CreateWindow("Lab 5",
                        "Lab 5 by Sofiia Krytska IM-21",
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

LRESULT CALLBACK WindowProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    switch (messg) {
        case WM_CREATE:

            button_bfs = CreateWindow("BUTTON",
                                      "Begin bfs",
                                      WS_VISIBLE | WS_CHILD | WS_BORDER,
                                      200, 400, 290, 50,
                                      hWnd, (HMENU) 1, NULL, NULL);
            button_dfs = CreateWindow("BUTTON",
                                      "Begin dfs",
                                      WS_VISIBLE | WS_CHILD | WS_BORDER,
                                      750, 400, 290, 50,
                                      hWnd, (HMENU) 2, NULL, NULL);
            button_next_bfs = CreateWindow("BUTTON",
                                           "Next vertex bfs",
                                           WS_VISIBLE | WS_CHILD | WS_BORDER,
                                           200, 470, 290, 50,
                                           hWnd, (HMENU) 3, NULL, NULL);
            button_next_dfs = CreateWindow("BUTTON",
                                           "Next vertex dfs",
                                           WS_VISIBLE | WS_CHILD | WS_BORDER,
                                           750, 470, 290, 50,
                                           hWnd, (HMENU) 4, NULL, NULL);
            break;
        case WM_COMMAND:

            switch (LOWORD(wParam)) {
                case 1:
                    window_update(hWnd, hdc, ps, 1);
                    break;
                case 2:
                    window_update(hWnd, hdc, ps, 2);
                    break;
                case 3:
                    waiting_button_bfs = false;
                    break;
                case 4:
                    waiting_button_dfs = false;
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
            return DefWindowProc(hWnd, messg, wParam, lParam);
    }

    return 0;
}
