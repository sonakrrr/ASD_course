#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

#define EXTENT 15
#define SEED 2112
#define ROWS 3
#define COLS 4
#define X_DISTANCE 130
#define Y_DISTANCE 100
#define OFFSET 30
#define ARROW_LENGTH 35
#define ARROW_OFFSET 50
#define ELLIPSE_WIDTH 44
#define ELLIPSE_HEIGHT 22

// n1 = 2, n2 = 1, n3 = 1, n4 = 2
// Число вершин - 11
// Розміщення вершин - прямокутником(квадратом)
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
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
float** mult_matrix(float coef, float** matrix, int n)
{
    float** result = (float**)malloc(n * sizeof(float*));
    for(int i = 0; i < n; i++){
        result[i] = (float*)malloc(n * sizeof(float));
        for(int j = 0; j < n; j++){
            result[i][j] = floor(matrix[i][j] * coef);
        }
    }
    return result;
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
char** create_symbol_array(int N)
{
    char** array = malloc(N * sizeof(char*));
    for(int i = 0; i < N; i++) {
        array[i] = malloc(3 * sizeof(char));
        sprintf(array[i], "%d", i+1);
    }
    return array;
}
void array_x(int N, int* array)
{
    int index = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            array[index] = 100 + j * X_DISTANCE;
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
void draw_undirected_graph(HWND hWnd, HDC hdc, int n, char** nn, int nx[], int
ny[], float** A)
{
    int edge_сeil = ceil(n / 4.0);
    int step = (edge_сeil + 2) * 100;
    for(int i = 0; i < n; i++){
        nx[i] = nx[i] + step;
    }
    printf("undirected A:\n");
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(A[j][i] == 1 || A[i][j] == 1){
                printf("1 ");
            } else printf("%.0f ", A[i][j]);
        }
        printf("\n");
    }
    int dist_x = 22, dist_y = 22, dtx = 5;
    HPEN KPen = CreatePen(PS_SOLID, 1, RGB(20, 20, 5));
    SelectObject(hdc, KPen);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(A[i][j] == 1 && abs(i-j) >=2 && (nx[i] == nx[j] || ny[i] == ny[j])){
                if(nx[i] == nx[j]){
                    MoveToEx(hdc, nx[i], ny[i], NULL);
                    LineTo(hdc, nx[j]+OFFSET, ny[i]-(ny[i]-ny[j])/2);
                    MoveToEx(hdc, nx[j]+OFFSET, ny[i]-(ny[i]-ny[j])/2, NULL);
                    LineTo(hdc, nx[j], ny[j]);
                } else{
                    MoveToEx(hdc, nx[i], ny[i], NULL);
                    LineTo(hdc, nx[j]+(nx[i]-nx[j])/2, ny[i]-OFFSET);
                    MoveToEx(hdc, nx[j]+(nx[i]-nx[j])/2, ny[i]-OFFSET, NULL);
                    LineTo(hdc, nx[j], ny[j]);
                }
            }
            else if(A[i][j] == 1){
                if(i == j){
                    int dir = (int) ceil((i+1)/(float) edge_сeil);
                    if(dir%2 == 0){
                        if(dir > edge_сeil){
                            Ellipse(hdc, nx[i]-ELLIPSE_WIDTH, ny[i]-ELLIPSE_HEIGHT,
                                    nx[i], ny[i]+ELLIPSE_HEIGHT);
                        } else{
                            Ellipse(hdc, nx[i]+ELLIPSE_WIDTH, ny[i]-ELLIPSE_HEIGHT,
                                    nx[i], ny[i]+ELLIPSE_HEIGHT);
                        }
                    } else{
                        if(dir >= edge_сeil){
                            Ellipse(hdc, nx[i]-ELLIPSE_HEIGHT, ny[i]+ELLIPSE_WIDTH,
                                    nx[i]+ELLIPSE_HEIGHT, ny[i]);
                        } else{
                            Ellipse(hdc, nx[i]-ELLIPSE_HEIGHT, ny[i]-ELLIPSE_WIDTH,
                                    nx[i]+ELLIPSE_HEIGHT, ny[i]);
                        }
                    }
                }
                else{
                    MoveToEx(hdc, nx[i], ny[i], NULL);
                    LineTo(hdc, nx[j], ny[j]);
                }
            }
        }
    }
    HPEN RPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    SelectObject(hdc, RPen);
    for(int i = 0;i < n; i++){
        Ellipse(hdc, nx[i]-dist_x,ny[i]-
                                      dist_y,nx[i]+dist_x,ny[i]+dist_y);
        TextOut(hdc, nx[i]-dtx,ny[i]-dist_y/2, nn[i],2);
    }
}
void draw_directed_graph(HWND hWnd, HDC hdc)
{
    const int N = 11;
    int edge_ceil = ceil(N / 4.0);
    int nx[N], ny[N];
    char** nn = create_symbol_array(N);
    array_x(N, nx);
    array_y(N, ny);
    printf("Vertexes names: ");
    for(int i = 0; i < N; i++){
        printf("%s ", nn[i]);
    }
    printf("\n");
    int dist_x = 22, dist_y = 22, dtx = 5;
    float** T = random_matrix(N);
    float** A = mult_matrix(0.715, T, N);
    printf("T:\n");
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            printf("%.2f ", T[i][j]);
        }
        printf("\n");
    }
    printf("directed A:\n");
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            printf("%.0f ", A[i][j]);
        }
        printf("\n");
    }
    A[0][9] = 1;
    int last_row = 2;
    int last_col = 3;
    int tempX = nx[10];
    int tempY = ny[10];
    nx[10] = nx[last_row * 4 + last_col];
    ny[10] = ny[last_row * 4 + last_col];
    nx[last_row * 4 + last_col] = tempX;
    ny[last_row * 4 + last_col] = tempY;
    A[9][0] = 0;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(A[i][j] == 1){
                if(abs(i-j) >=2 && (nx[i] == nx[j] || ny[i] == ny[j])){
                    if(nx[i] == nx[j]){
                        if(i > j){
                            MoveToEx(hdc, nx[i], ny[i], NULL);
                            LineTo(hdc, nx[j]+OFFSET, ny[i]-(ny[i]-ny[j])/2);
                            MoveToEx(hdc, nx[j]+OFFSET, ny[i]-(ny[i]-ny[j])/2, NULL);
                            LineTo(hdc, nx[j], ny[j]);
                            draw_arrow(nx[j]+OFFSET, ny[i]-(ny[i]-ny[j])/2, nx[j], ny[j],
                                       dist_x, hdc);
                        } else{
                            MoveToEx(hdc, nx[i], ny[i], NULL);
                            LineTo(hdc, nx[j]-OFFSET, ny[i]-(ny[i]-ny[j])/2);
                            MoveToEx(hdc, nx[j]-OFFSET, ny[i]-(ny[i]-ny[j])/2, NULL);
                            LineTo(hdc, nx[j], ny[j]);
                            draw_arrow(nx[j]-OFFSET, ny[i]-(ny[i]-ny[j])/2, nx[j], ny[j],
                                       dist_x, hdc);
                        }
                    } else{
                        if(i > j){
                            MoveToEx(hdc, nx[i], ny[i], NULL);
                            LineTo(hdc, nx[j]+(nx[i]-nx[j])/2, ny[i]+OFFSET);
                            MoveToEx(hdc, nx[j]+(nx[i]-nx[j])/2, ny[i]+OFFSET, NULL);
                            LineTo(hdc, nx[j], ny[j]);
                            draw_arrow(nx[j]+(nx[i]-nx[j])/2, ny[i]+OFFSET, nx[j], ny[j],
                                       dist_x, hdc);
                        } else{
                            MoveToEx(hdc, nx[i], ny[i], NULL);
                            LineTo(hdc, nx[j]+(nx[i]-nx[j])/2, ny[i]-OFFSET);
                            MoveToEx(hdc, nx[j]+(nx[i]-nx[j])/2, ny[i]-OFFSET, NULL);
                            LineTo(hdc, nx[j], ny[j]);
                            draw_arrow(nx[j]+(nx[i]-nx[j])/2, ny[i]-OFFSET, nx[j], ny[j],
                                       dist_x, hdc);
                        }
                    }
                }
                if(i == j){
                    int dir = (int) ceil((i+1)/(float) edge_ceil);
                    if(dir%2 == 0){
                        if(dir > edge_ceil){
                            Ellipse(hdc, nx[i]-ELLIPSE_WIDTH, ny[i]-ELLIPSE_HEIGHT,
                                    nx[i], ny[i]+ELLIPSE_HEIGHT);
                            draw_arrow(nx[i]-ARROW_LENGTH, ny[i]+ARROW_OFFSET,
                                       nx[j], ny[j], dist_x, hdc);
                        } else{
                            Ellipse(hdc, nx[i]+ELLIPSE_WIDTH, ny[i]-ELLIPSE_HEIGHT,
                                    nx[i], ny[i]+ELLIPSE_HEIGHT);
                            draw_arrow(nx[i]+ARROW_LENGTH, ny[i]-ARROW_OFFSET,
                                       nx[j], ny[j], dist_x, hdc);
                        }
                    } else{
                        if(dir >= edge_ceil){
                            Ellipse(hdc, nx[i]-ELLIPSE_HEIGHT, ny[i]+ELLIPSE_WIDTH,
                                    nx[i]+ELLIPSE_HEIGHT, ny[i]);
                            draw_arrow(nx[i]+ARROW_OFFSET, ny[i]+ARROW_OFFSET,
                                       nx[j], ny[j], dist_x, hdc);
                        } else{
                            Ellipse(hdc, nx[i]-ELLIPSE_HEIGHT, ny[i]-ELLIPSE_WIDTH,
                                    nx[i]+ELLIPSE_HEIGHT, ny[i]);
                            draw_arrow(nx[i]-ARROW_OFFSET, ny[i]-ARROW_OFFSET,
                                       nx[j], ny[j], dist_x, hdc);
                        }
                    }
                }
            }
        }
    }
    int summary = 0;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(A[i][j] == 1) {
                summary++;
                if (!(abs(i - j) >= 2 && (nx[i] == nx[j] || ny[i] == ny[j])) && i!=j){
                    if (i > j && A[j][i] == 1 && A[i][j] == 1) {
                        MoveToEx(hdc, nx[i], ny[i], NULL);
                        LineTo(hdc, (nx[i] + nx[j]) / 2 + OFFSET, (ny[i] + ny[j]) / 2);
                        MoveToEx(hdc, (nx[i] + nx[j]) / 2 + OFFSET, (ny[i] + ny[j]) / 2,
                                 NULL);
                        LineTo(hdc, nx[j], ny[j]);
                        draw_arrow((nx[i] + nx[j]) / 2 + OFFSET, (ny[i] + ny[j]) / 2, nx[j],
                                   ny[j], dist_x, hdc);
                    }
                    else if(j > i && A[i][j] == 1 && A[j][i] == 1) {
                        MoveToEx(hdc, nx[i], ny[i], NULL);
                        LineTo(hdc, (nx[i] + nx[j]) / 2 - OFFSET, (ny[i] + ny[j]) / 2);
                        MoveToEx(hdc, (nx[i] + nx[j]) / 2 - OFFSET, (ny[i] + ny[j]) / 2,
                                 NULL);
                        LineTo(hdc, nx[j], ny[j]);
                        draw_arrow((nx[i] + nx[j]) / 2 - OFFSET, (ny[i] + ny[j]) / 2, nx[j],
                                   ny[j], dist_x, hdc);
                    }
                    else {
                        MoveToEx(hdc, nx[i], ny[i], NULL);
                        LineTo(hdc, nx[j], ny[j]);
                        draw_arrow(nx[i], ny[i], nx[j], ny[j], dist_x, hdc);
                    }
                }
            }
        }
    }
    HPEN RPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    SelectObject(hdc, RPen);
    for(int i = 0;i < N; i++){
        Ellipse(hdc, nx[i]-dist_x,ny[i]-
                                      dist_y,nx[i]+dist_x,ny[i]+dist_y);
        TextOut(hdc, nx[i]-dtx,ny[i]-dist_y/2, nn[i],2);
    }
    draw_undirected_graph (hWnd, hdc, N, nn, nx, ny, A);
    for(int i = 0; i < N; i++){
        free(T[i]);
        free(A[i]);
    }
    free(T);
    free(A);
    free(nn);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpszCmdLine, int nCmdShow)
{
    WNDCLASS window;
    window.lpszClassName = "Lab 3";
    window.hInstance = hInstance;
    window.lpfnWndProc = WindowProc;
    window.hCursor = LoadCursor(NULL, IDC_ARROW);
    window.hIcon = 0;
    window.lpszMenuName = 0;
    window.hbrBackground = WHITE_BRUSH;
    window.style = CS_HREDRAW | CS_VREDRAW;
    window.cbClsExtra = 0;
    window.cbWndExtra = 0;
    if (!RegisterClass(&window))
    {
        printf("Error! Window was not registered!");
        return 0;
    }
    HWND hWnd;
    MSG lpMsg;
    hWnd = CreateWindow("Lab 3",
                        "Lab 3 by Sofiia Krytska IM-21",
                        WS_OVERLAPPEDWINDOW,
                        100,
                        100,
                        1200,
                        700,
                        (HWND)NULL,
                        (HMENU)NULL,
                        (HINSTANCE)hInstance,
                        (HINSTANCE)NULL);
    ShowWindow(hWnd, nCmdShow);
    while(GetMessage(&lpMsg, hWnd, 0, 0)){
        TranslateMessage(&lpMsg);
        DispatchMessage(&lpMsg);
    }
    return(lpMsg.wParam);
}
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM
wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    switch(message){
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            draw_directed_graph(hWnd, hdc);
            EndPaint(hWnd, &ps);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}