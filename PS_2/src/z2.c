#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "functions.h" // Swish funkcia a pomocne funkcie -- NEMENIT !!!
#include "data.h" // Makra, vahy a bias hodnoty -- NEMENIT !!!

void rezim_1();
void rezim_2();
void rezim_3();
void rezim_4();
void rezim_5();
void rezim_6();
void rezim_7();
void soft_max(double z[NUMBER_OF_NEURONS], double output[NUMBER_OF_NEURONS]);

int main(void) {
    int rezim;

    scanf ("%d", &rezim);
    if (rezim == 1) {
        rezim_1();
    }
    else if (rezim == 2) {
        rezim_2();
    }
    else if (rezim == 3) {
        rezim_3();
    }
    else if (rezim == 4) {
        rezim_4();
    }
    else if (rezim == 5) {
        rezim_5();
    }
    else if (rezim == 6) {
        rezim_6();
    }
    else if (rezim == 7) {
        rezim_7();
    }
    return 0;
}

void rezim_1() {
    double image[IMAGE_SIZE];
    load_image(image, IMAGE_SIZE);
    print_image(image, IMAGE_WIDTH, IMAGE_HEIGHT);
    printf("\n");
}

void rezim_2() {
    int index, row, col;
    scanf ("%d %d %d", &index, &row, &col);
    int location = (index * IMAGE_SIZE) + (row * IMAGE_WIDTH) + col;
    printf("%.2lf", weights[location]);
}

void rezim_3() {
    double image[IMAGE_SIZE];
    load_image(image, IMAGE_SIZE);

    double weights_for[NUMBER_OF_NEURONS][IMAGE_SIZE];

    for (int neuron = 0; neuron < NUMBER_OF_NEURONS ; neuron++) {

        for (int m = 0; m < IMAGE_SIZE; m++) {
        weights_for[neuron][m] = weights[neuron * IMAGE_SIZE + m];
        }

        double sucet = 0.0;
        printf("%8d", neuron);

        for (int i = 0; i < IMAGE_SIZE; i++) {
            sucet += weights_for[neuron][i] * image[i];
        }
        sucet += bias[neuron];
        printf("%8.2lf\n", sucet);

    }
}
void rezim_4() {
    double z[NUMBER_OF_NEURONS], output[NUMBER_OF_NEURONS];
    for (int i = 0; i < NUMBER_OF_NEURONS ; i++) {
        double hodnota;
        scanf ("%lf", &hodnota);
        z[i] = hodnota;
    }

    soft_max(z, output);
    printf("%.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf %.2lf",
        output[0], output[1], output[2], output[3], output[4], output[5], output[6], output[7], output[8], output[9]);
    printf("\n");

}

void soft_max(double z[NUMBER_OF_NEURONS], double output[NUMBER_OF_NEURONS]) {
    double max = z[0];

    for (int k = 1; k < NUMBER_OF_NEURONS; k++) {
        if (z[k] > max) {
            max = z[k];
        }
    }

    for (int i = 0; i < NUMBER_OF_NEURONS; i++) {
        double sum = 0;

        for (int j = 0; j < NUMBER_OF_NEURONS; j++) {
            sum += exp(z[j] - max);
        }

        double number = exp(z[i] - max)/sum;
        output[i] = number;
    }

}

void rezim_5() {
    double input[NUMBER_OF_NEURONS];
    for (int i = 0; i < NUMBER_OF_NEURONS ; i++) {
        double hodnota;
        scanf ("%lf", &hodnota);
        input[i] = hodnota;
    }

    double max = input[0];
    int index = 0;
    for (int k = 1; k < NUMBER_OF_NEURONS; k++) {
        if (input[k] > max) {
            max = input[k];
            index = k;
        }
    }
    printf("%d", index);
}

void rezim_6() {
    double image[IMAGE_SIZE];
    load_image(image, IMAGE_SIZE);

    double weights_for_neuron[NUMBER_OF_NEURONS][IMAGE_SIZE];
    double wheight_sum[NUMBER_OF_NEURONS];
    double output[NUMBER_OF_NEURONS];

    for (int neuron = 0; neuron < NUMBER_OF_NEURONS ; neuron++) {

        for (int m = 0; m < IMAGE_SIZE; m++) {
            weights_for_neuron[neuron][m] = weights[neuron * IMAGE_SIZE + m];
        }

        double sucet = 0;

        for (int i = 0; i < IMAGE_SIZE; i++) {
            sucet += weights_for_neuron[neuron][i] * image[i];
        }
        sucet += bias[neuron];

        wheight_sum[neuron] = sucet;
    }

    soft_max(wheight_sum, output);

    double max = output[0];
    int number = 0;
    for (int k = 1; k < NUMBER_OF_NEURONS; k++) {
        if (output[k] > max) {
            max = output[k];
            number = k;
        }
    }
    printf("%d\n", number);
}

void rezim_7() {
    int count, correct = 0;
    scanf("%d", &count);

    int input[count];

    for (int i = 0; i < count; i++) {
        scanf ("%d", &input[i]);
    }

    double images[count][IMAGE_SIZE];
    int output[count];

    double weights_for_neuron[NUMBER_OF_NEURONS][IMAGE_SIZE];
    double wheight_sum[NUMBER_OF_NEURONS];

    for (int i = 0; i < count; i++) {

        for (int j = 0; j < IMAGE_SIZE; j++) {
            scanf("%lf", &images[i][j]);
        }

        for (int neuron = 0; neuron < NUMBER_OF_NEURONS ; neuron++) {

            for (int m = 0; m < IMAGE_SIZE; m++) {
                weights_for_neuron[neuron][m] = weights[neuron * IMAGE_SIZE + m];
            }

            double sucet = 0;

            for (int k = 0; k < IMAGE_SIZE; k++) {
                sucet += weights_for_neuron[neuron][k] * images[i][k];
            }
            sucet += bias[neuron];

            wheight_sum[neuron] = sucet;
        }
        double softmax_output[NUMBER_OF_NEURONS];
        soft_max(wheight_sum, softmax_output);

        double max = softmax_output[0];
        int number = 0;
        for (int k = 1; k < NUMBER_OF_NEURONS; k++) {
            if (softmax_output[k] > max) {
                max = softmax_output[k];
                number = k;
            }
        }
        output[i] = number;
    }
    for (int i = 0; i < count; i++) {
        if (output[i] == input[i]) {
            correct++;
        }
    }
    double accuracy = (double)correct / count * 100.0;

    printf("%.2lf", accuracy);
    for (int i = 0; i < count; i++) {
        printf(" %d-%d-%d", i, input[i], output[i]);
    }
    printf("\n");
}
