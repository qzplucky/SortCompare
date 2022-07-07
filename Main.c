#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define SIZE1 40000000
#define SORT_METHOD_NUMBER 8
#define MAX 2147483647

int origin_list[SIZE1];
int sort_list[SIZE1];
int prime_list[SIZE1];
int tree_list[SIZE1 * 2 + 2];
int merge_list[SIZE1];
int low_stack[SIZE1];
int high_stack[SIZE1];
int SIZE;
int INIT_METHOD;
int SHOW_ORIGIN;
int SHOW_RESULT;
double time_list[SORT_METHOD_NUMBER];
long long compare_list[SORT_METHOD_NUMBER];
long long move_list[SORT_METHOD_NUMBER];
char sort_methods[SORT_METHOD_NUMBER][30] = {
        {"straight_insert_sort "},
        {"binary_insert_sort   "},
        {"shell_sort           "},
        {"bullet_sort          "},
        {"simple_selection_sort"},
        {"quick_sort           "},
        {"pile_sort            "},
        {"merge_sort           "}, };

void init1()
{

    for (int i = 0; i < SIZE; i++) {
        origin_list[i] = rand() % SIZE;
    }
}

void init2() {
    for (int i = 0; i < SIZE; i++) {
        origin_list[i] = i;
    }
}

void init3()
{
    for (int i = 0; i < SIZE; i++)
    {
        origin_list[i] = SIZE - i;
    }
}

void show(int list[]) {
    for (int i = 0; i < SIZE; i++) {
        printf("%d\t", list[i]);
    }
    printf("\n");
}

void create_tree(void) {
    for (int i = 0; i < SIZE; i++) {
        tree_list[i + 1] = origin_list[i];
    }
    for (int i = SIZE + 1; i <= 2 * SIZE + 1; i++) {
        tree_list[i] = MAX;
    }
}

int compare(int* a, int* b, int method_index) {
    compare_list[method_index]++;
    if (*a <= *b) {
        return 1;
    }
    else {
        return 0;
    }
}

void move(int* a, int* b, int method_index) {
    move_list[method_index]++;
    *a = *b;
}

void exchange(int* a, int* b, int method_index) {
    move_list[method_index] += 3;
    int temp = *a;
    *a = *b;
    *b = temp;
}

void reduction(int a[], int b[])
{
    for (int i = 0; i < SIZE; i++) {
        a[i] = b[i];
    }
}

double operate(void(*sort)(int), int method_index) {
    clock_t begin = clock();
    reduction(sort_list, origin_list);

    sort(method_index);

    clock_t end = clock();
    double time_consumption = ((double)(end - begin) / CLOCKS_PER_SEC) * 1000;

    int flag = SHOW_RESULT;
    if (flag != 0) {
        show(sort_list);
    }
    time_list[method_index] = time_consumption;
}

void straight_insert_sort(int method_index)
{
    for (int i = 1; i < SIZE; i++) {
        for (int j = i; j > 0; j--) {
            if (!compare(&sort_list[j - 1], &sort_list[j], method_index)) {
                exchange(&sort_list[j - 1], &sort_list[j], method_index);
            }
            else {
                break;
            }
        }
    }
}

void binary_insert_sort(int method_index) {
    int low;
    int high;
    int mid;
    int temp;
    for (int i = 1; i < SIZE; i++) {
        low = -1;
        high = i - 1;
        mid = 0;
        while (low <= high) {
            mid = (low + high) / 2;
            if (compare(&sort_list[mid], &sort_list[i], method_index)) {
                if (low == mid) {
                    low = mid + 1;
                }
                else {
                    low = mid;
                }
            }
            else {
                high = mid - 1;
            }
        }
        move(&temp, &sort_list[i], method_index);
        for (int j = i - 1; j > high; j--) {
            move(&sort_list[j + 1], &sort_list[j], method_index);
        }
        move(&sort_list[high + 1], &temp, method_index);
    }
}

void shell_sort(method_index)
{
    for (int i = SIZE / 2; i > 1; i /= 2) {
        for (int j = 0; j < i; j++) {
            for (int i1 = j + i; i1 < SIZE; i1 = i1 + i) {
                for (int j1 = i1; j1 > j; j1 = j1 - i) {
                    if (!compare(&sort_list[j1 - i], &sort_list[j1], method_index)) {
                        exchange(&sort_list[j1 - i], &sort_list[j1], method_index);
                    }
                    else {
                        break;
                    }
                }
            }
        }
    }

    for (int j = 0; j < 1; j++) {
        for (int i1 = j + 1; i1 < SIZE; i1 = i1 + 1) {
            for (int j1 = i1; j1 > j; j1 = j1 - 1) {
                if (!compare(&sort_list[j1 - 1], &sort_list[j1], method_index)) {
                    exchange(&sort_list[j1 - 1], &sort_list[j1], method_index);
                }
                else {
                    break;
                }
            }
        }
    }
}

void bullet_sort(method_index) {
    int flag = 0;
    for (int i = 1; i < SIZE; i++) {
        flag = 1;
        for (int j = 0; j < SIZE - i; j++) {
            if (!compare(&sort_list[j], &sort_list[j + 1], method_index)) {
                exchange(&sort_list[j], &sort_list[j + 1], method_index);
                flag = 0;
            }
        }
        if (flag == 1) {
            break;
        }
    }
}

void simple_selection_sort(int method_index) {
    int min_index = 0;
    int min = 0;
    for (int i = 0; i < SIZE - 1; i++) {
        min = sort_list[i];
        min_index = i;
        for (int j = i + 1; j < SIZE; j++) {
            if (!compare(&min, &sort_list[j], method_index))
            {
                move(&min, &sort_list[j], method_index);
                move(&min_index, &j, method_index);
            }
        }
        exchange(&sort_list[i], &sort_list[min_index], method_index);
    }
}

int quick_sort_once(int low, int high, int method_index) {
    int target = sort_list[low];
    int flag = 0;
    while (low < high) {
        if (flag == 0) {
            if (!compare(&target, &sort_list[high], method_index)) {
                move(&sort_list[low], &sort_list[high], method_index);
                flag = 1;
                high++;
            }
            high--;
        }
        else {
            if (!compare(&sort_list[low], &target, method_index)) {
                move(&sort_list[high], &sort_list[low], method_index);
                flag = 0;
                low--;
            }
            low++;
        }
    }
    move(&sort_list[low], &target, method_index);
    return low;
}

void quick_sort(int method_index) {
    int stack_top = 0;
    stack_top++;
    low_stack[stack_top] = 0;
    high_stack[stack_top] = SIZE - 1;
    int low;
    int high;
    int mid;
    while (stack_top != 0) {
        low = low_stack[stack_top];
        high = high_stack[stack_top];
        stack_top--;
        if (low < high) {
            mid = quick_sort_once(low, high, method_index);
            stack_top++;
            low_stack[stack_top] = low;
            high_stack[stack_top] = mid - 1;
            stack_top++;
            low_stack[stack_top] = mid + 1;
            high_stack[stack_top] = high;
        }
    }
}

void create_small_pile(method_index) {
    for (int i = 1; 2 * i <= SIZE; i *= 2) {
        for (int j = SIZE; j >= i; j--) {
            if (compare(&tree_list[2 * j], &tree_list[2 * j + 1], method_index)) {
                if (compare(&tree_list[2 * j], &tree_list[j], method_index)) {
                    exchange(&tree_list[2 * j], &tree_list[j], method_index);
                }
            }
            else {
                if (compare(&tree_list[2 * j + 1], &tree_list[j], method_index)) {
                    exchange(&tree_list[2 * j + 1], &tree_list[j], method_index);
                }
            }
        }
    }
}

void pile_sort(int method_index) {
    create_tree();
    create_small_pile(method_index);
    for (int i = SIZE; i >= 1; i--) {
        int node = 1;
        while (node <= i) {
            if (2 * node + 1 <= i) {

                if (compare(&tree_list[2 * node], &tree_list[2 * node + 1], method_index)) {
                    if (!compare(&tree_list[node], &tree_list[2 * node], method_index)) {
                        exchange(&tree_list[node], &tree_list[2 * node], method_index);
                        node = 2 * node;
                    }
                    else {
                        break;
                    }
                }
                else {
                    if (!compare(&tree_list[node], &tree_list[2 * node + 1], method_index)) {
                        exchange(&tree_list[node], &tree_list[2 * node + 1], method_index);
                        node = 2 * node + 1;
                    }
                    else {
                        break;
                    }
                }
            }
            else if (2 * node <= i) {
                if (!compare(&tree_list[node], &tree_list[2 * node], method_index)) {
                    exchange(&tree_list[node], &tree_list[2 * node], method_index);
                    node = 2 * node;
                }
                else {
                    break;
                }
            }
            else {
                break;
            }
        }
        sort_list[SIZE - i] = tree_list[1];
        exchange(&tree_list[1], &tree_list[i], method_index);
    }
}

void merge_double_piece(int start_piece1, int end_piece1, int start_piece2, int end_piece2, int method_index) {
    int merge_index = start_piece1;
    int piece1_index = start_piece1;
    int piece2_index = start_piece2;
    while (merge_index <= end_piece2) {
        if (compare(&sort_list[piece1_index], &sort_list[piece2_index], method_index)) {
            move(&merge_list[merge_index++], &sort_list[piece1_index++], method_index);
            if (piece1_index > end_piece1) {
                while (merge_index <= end_piece2) {
                    move(&merge_list[merge_index++], &sort_list[piece2_index++], method_index);
                }
                break;
            }
        }
        else {
            move(&merge_list[merge_index++], &sort_list[piece2_index++], method_index);
            if (piece2_index > end_piece2) {
                while (merge_index <= end_piece2) {
                    move(&merge_list[merge_index++], &sort_list[piece1_index++], method_index);
                }
                break;
            }
        }
    }
}

void once_merge_sort(int width, int method_index) {
    int double_width = 2 * width;
    int start_piece1 = 0;
    int end_piece1 = width - 1;
    int start_piece2 = width;
    int end_piece2 = double_width - 1;
    while (end_piece2 < SIZE) {
        merge_double_piece(start_piece1, end_piece1, start_piece2, end_piece2, method_index);
        start_piece1 += double_width;
        end_piece1 += double_width;
        start_piece2 += double_width;
        end_piece2 += double_width;
    }
    if (start_piece2 < SIZE) {
        merge_double_piece(start_piece1, end_piece1, start_piece2, SIZE - 1, method_index);
    }
    reduction(sort_list, merge_list);
}

void merge_sort(int method_index) {
    int width = 1;
    while (width < SIZE) {
        once_merge_sort(width, method_index);
        width *= 2;
    }
}

void show_result(void) {
    int index;
    for (int index = 0; index < SORT_METHOD_NUMBER; index++) {
        printf("%s\t%f\t%lld\t%lld\n", sort_methods[index], time_list[index], compare_list[index], move_list[index]);
    }
}

void prepare(void) {
    for (int i = 0; i < SORT_METHOD_NUMBER; i++) {
        time_list[i] = 0.0;
        compare_list[i] = 0;
        move_list[i] = 0;
    }
}

void start(int size_parameter, int init_method_parameter, int show_origin_data_parameter, int show_result_data_parameter) {
    SIZE = size_parameter;
    INIT_METHOD = init_method_parameter;
    SHOW_ORIGIN = show_origin_data_parameter;
    SHOW_RESULT = show_result_data_parameter;
    printf("SIZE:%d\n", SIZE);
    printf("INIT_METHOD:%d\n", INIT_METHOD);
    switch (INIT_METHOD) {
        case 1: {
            init1();
            break;
        }
        case 2: {
            init2();
            break;
        }
        case 3: {
            init3();
            break;
        }
    }
    if (SHOW_ORIGIN != 0) {
        show(origin_list);
    }
    prepare();
    operate(straight_insert_sort, 0);
    operate(binary_insert_sort, 1);
    operate(shell_sort, 2);
    operate(bullet_sort, 3);
    operate(simple_selection_sort, 4);
    operate(quick_sort, 5);
    operate(pile_sort, 6);
    operate(merge_sort, 7);
    show_result();
    printf("SIZE:%d\n", SIZE);
    printf("INIT_METHOD:%d\n", INIT_METHOD);
}

int main(void) {
    int size_parameter;
    int init_method_parameter;
    int show_origin_data_parameter;
    int show_result_data_parameter;
    printf("size_parameter(0 - %d):", SIZE1);
    scanf("%d", &size_parameter);
    printf("init_method_parameter(1 - Out of order, 2 - Order, 3 - Reverse order):");
    scanf("%d", &init_method_parameter);
    printf("show_origin_data_parameter(0 - Do not display data to be sorted, 1 - Display data to be sorted):");
    scanf("%d", &show_origin_data_parameter);
    printf("show_result_data_parameter(0 - Do not display sorted data, 1 - Display sorted data):");
    scanf("%d", &show_result_data_parameter);
    start(size_parameter, init_method_parameter, show_origin_data_parameter, show_result_data_parameter);
    return 0;
}