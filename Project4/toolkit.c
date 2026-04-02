#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 100

typedef struct {
    double *data;
    int size;
} Dataset;

typedef void (*OperationFunc)(Dataset *);
typedef int (*FilterFunc)(double);
typedef double (*TransformFunc)(double);
typedef int (*CompareFunc)(double, double);

Dataset dataset = {NULL, 0};

void freeDataset() {
    if (dataset.data != NULL) {
        free(dataset.data);
        dataset.data = NULL;
        dataset.size = 0;
    }
}

int isEmpty() {
    if (dataset.size == 0) {
        printf("Dataset is empty!\n");
        return 1;
    }
    return 0;
}

void createDataset() {
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("Invalid size!\n");
        return;
    }

    freeDataset();

    dataset.data = (double *)malloc(n * sizeof(double));
    if (!dataset.data) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    dataset.size = n;

    printf("Enter %d values:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%lf", &dataset.data[i]);
    }
}

void displayDataset(Dataset *d) {
    if (isEmpty()) return;

    printf("Dataset: ");
    for (int i = 0; i < d->size; i++) {
        printf("%.2lf ", d->data[i]);
    }
    printf("\n");
}

void computeSumAvg(Dataset *d) {
    if (isEmpty()) return;

    double sum = 0;
    for (int i = 0; i < d->size; i++) {
        sum += d->data[i];
    }

    printf("Sum = %.2lf\n", sum);
    printf("Average = %.2lf\n", sum / d->size);
}

void findMinMax(Dataset *d) {
    if (isEmpty()) return;

    double min = d->data[0];
    double max = d->data[0];

    for (int i = 1; i < d->size; i++) {
        if (d->data[i] < min) min = d->data[i];
        if (d->data[i] > max) max = d->data[i];
    }

    printf("Min = %.2lf\n", min);
    printf("Max = %.2lf\n", max);
}

int greaterThanThreshold(double x) {
    double threshold;
    printf("Enter threshold: ");
    scanf("%lf", &threshold);
    return x > threshold;
}

void filterDataset(Dataset *d, FilterFunc func) {
    if (isEmpty()) return;

    double *newData = (double *)malloc(d->size * sizeof(double));
    int newSize = 0;

    for (int i = 0; i < d->size; i++) {
        if (func(d->data[i])) {
            newData[newSize++] = d->data[i];
        }
    }

    free(d->data);
    d->data = newData;
    d->size = newSize;

    printf("Filtering complete.\n");
}

double multiplyByFactor(double x) {
    double factor;
    printf("Enter multiplication factor: ");
    scanf("%lf", &factor);
    return x * factor;
}

void transformDataset(Dataset *d, TransformFunc func) {
    if (isEmpty()) return;

    for (int i = 0; i < d->size; i++) {
        d->data[i] = func(d->data[i]);
    }

    printf("Transformation complete.\n");
}

int ascending(double a, double b) {
    return a > b;
}

int descending(double a, double b) {
    return a < b;
}

void sortDataset(Dataset *d, CompareFunc cmp) {
    if (isEmpty()) return;

    for (int i = 0; i < d->size - 1; i++) {
        for (int j = 0; j < d->size - i - 1; j++) {
            if (cmp(d->data[j], d->data[j + 1])) {
                double temp = d->data[j];
                d->data[j] = d->data[j + 1];
                d->data[j + 1] = temp;
            }
        }
    }

    printf("Sorting complete.\n");
}

void searchValue(Dataset *d) {
    if (isEmpty()) return;

    double target;
    printf("Enter value to search: ");
    scanf("%lf", &target);

    for (int i = 0; i < d->size; i++) {
        if (d->data[i] == target) {
            printf("Found at index %d\n", i);
            return;
        }
    }

    printf("Value not found.\n");
}

void saveToFile(Dataset *d) {
    if (isEmpty()) return;

    FILE *f = fopen("data.txt", "w");
    if (!f) {
        printf("File error!\n");
        return;
    }

    for (int i = 0; i < d->size; i++) {
        fprintf(f, "%lf\n", d->data[i]);
    }

    fclose(f);
    printf("Saved to data.txt\n");
}

void loadFromFile() {
    FILE *f = fopen("data.txt", "r");
    if (!f) {
        printf("File not found!\n");
        return;
    }

    freeDataset();

    dataset.data = NULL;
    dataset.size = 0;

    double value;
    while (fscanf(f, "%lf", &value) == 1) {
        dataset.data = realloc(dataset.data, (dataset.size + 1) * sizeof(double));
        dataset.data[dataset.size++] = value;
    }

    fclose(f);
    printf("Data loaded from file.\n");
}

typedef struct {
    int choice;
    OperationFunc func;
    char name[MAX_NAME];
} MenuOption;

void exitProgram(Dataset *d) {
    freeDataset();
    printf("Exiting...\n");
    exit(0);
}

int main() {

    MenuOption menu[] = {
        {1, displayDataset, "Display Dataset"},
        {2, computeSumAvg, "Compute Sum & Average"},
        {3, findMinMax, "Find Min & Max"},
        {0, NULL, ""}
    };

    int choice;

    while (1) {
        printf("\n===== DATA ANALYSIS TOOLKIT =====\n");
        printf("1. Create Dataset\n");
        printf("2. Load Dataset from File\n");
        printf("3. Display Dataset\n");
        printf("4. Sum & Average\n");
        printf("5. Min & Max\n");
        printf("6. Filter Dataset\n");
        printf("7. Transform Dataset\n");
        printf("8. Sort Dataset\n");
        printf("9. Search Value\n");
        printf("10. Save to File\n");
        printf("11. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                createDataset();
                break;
            case 2:
                loadFromFile();
                break;
            case 3:
                displayDataset(&dataset);
                break;
            case 4:
                computeSumAvg(&dataset);
                break;
            case 5:
                findMinMax(&dataset);
                break;
            case 6:
                filterDataset(&dataset, greaterThanThreshold);
                break;
            case 7:
                transformDataset(&dataset, multiplyByFactor);
                break;
            case 8: {
                int opt;
                printf("1. Ascending\n2. Descending\nChoice: ");
                scanf("%d", &opt);
                if (opt == 1)
                    sortDataset(&dataset, ascending);
                else
                    sortDataset(&dataset, descending);
                break;
            }
            case 9:
                searchValue(&dataset);
                break;
            case 10:
                saveToFile(&dataset);
                break;
            case 11:
                exitProgram(&dataset);
                break;
            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}