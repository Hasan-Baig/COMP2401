#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/*
 * Author: Hasan Baig
 * Student Number: 101032292
 * Date: September 28 2020
 */

int main() {

    const int NUM_PACKAGES = 24;

    //box dimensions
    const int MAX_NUM_BOXES = 6;
    const int BOX_WIDTH = 4;
    const int BOX_HEIGHT = 5;
    const char EMPTY_CHAR = ' ';
    const char SMALL_CHAR = 'S';
    const char MEDIUM_CHAR = 'M';
    const char LARGE_CHAR = 'L';

    //package costs
    const double SMALL_PACKAGE_COST = 5.99;
    const double MEDIUM_PACKAGE_COST = 10.99;
    const double LARGE_PACKAGE_COST = 19.99;
    const double SHIPPING_PER_BOX = 1.49;
    const double TAX = 0.13;


    //number of packages (total = 24)
    srand(time(NULL));
    int numLargePackage = (rand() % NUM_PACKAGES+1);
    int numMediumPackage = (rand() % (NUM_PACKAGES+1 - numLargePackage));
    int numSmallPackage = NUM_PACKAGES - numLargePackage - numMediumPackage;

    //counter for adding packages to boxes
    int remainingSmall = numSmallPackage;
    int remainingMedium = numMediumPackage;
    int remainingLarge = numLargePackage;
    int numBox = 0;

    //creating a string to hold all boxes
    char boxString[(BOX_WIDTH * (BOX_HEIGHT+1)) * MAX_NUM_BOXES];

    //continue making new boxes and populating until all packages are packed and upto 6 boxes are used
    while (numBox <= MAX_NUM_BOXES && (remainingSmall+remainingMedium+remainingLarge)>0) {

        // make a new empty box
        char box[5][4];
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 4; ++j) {
                box[i][j] = EMPTY_CHAR;
            }
        }

        //populate new empty box in order of large to small
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 4; ++j) {

                //fit in large packages
                if (remainingLarge>0 && box[i][j] == EMPTY_CHAR && box[i+1][j] == EMPTY_CHAR && box[i][j+1] == EMPTY_CHAR && box[i+1][j+1] == EMPTY_CHAR && (i+1)<5 && (j+1)<4) {
                    box[i][j] = LARGE_CHAR;
                    box[i+1][j] = LARGE_CHAR;
                    box[i][j+1] = LARGE_CHAR;
                    box[i+1][j+1] = LARGE_CHAR;
                    remainingLarge--;
                }

                //fit in medium packages vertically
                if (remainingMedium>0 && box[i][j] == EMPTY_CHAR && box[i+1][j] == EMPTY_CHAR && (i+1)<5) {
                    box[i][j] = MEDIUM_CHAR;
                    box[i+1][j] = MEDIUM_CHAR;
                    remainingMedium--;
                }

                //fit in medium packages horizontally
                if (remainingMedium>0 && box[i][j] == EMPTY_CHAR && box[i][j+1] == EMPTY_CHAR && (j+1)<4) {
                    box[i][j] = MEDIUM_CHAR;
                    box[i][j+1] = MEDIUM_CHAR;
                    remainingMedium--;
                }

                //fit in small packages
                if (remainingSmall>0 && box[i][j] == EMPTY_CHAR) {
                    box[i][j] = SMALL_CHAR;
                    remainingSmall--;
                }
            }
        }

        // add box to boxString
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 4; j++) {
                strncat(boxString, &box[i][j], 1);
            }
            strcat(boxString, "\n");
        }
        strcat(boxString, "\n");

        //increase number of boxes by 1
        numBox++;
    }

    //cost of packages
    double smallCost = numSmallPackage * SMALL_PACKAGE_COST;
    double mediumCost = numMediumPackage * MEDIUM_PACKAGE_COST;
    double largeCost = numLargePackage * LARGE_PACKAGE_COST;
    double total = smallCost + mediumCost + largeCost;
    double shipping = numBox * SHIPPING_PER_BOX;
    double hst = total * TAX;
    double amountCharged = total + shipping + hst;

    //Order Information
    printf("Order:\tLarge=%d, Medium=%d, Small=%d\n\n", numLargePackage, numMediumPackage, numSmallPackage);
    printf("Small (%02d units)\t$ %.2f\n", numSmallPackage, smallCost);
    printf("Medium (%02d units)\t$ %.2f\n", numMediumPackage, mediumCost);
    printf("Large (%02d units)\t$ %.2f\n", numLargePackage, largeCost);
    printf("--------------------------------\n");
    printf("Total\t\t\t$ %.2f\n", total);
    printf("Shipping (%d boxes)\t$ %.2f\n", numBox, shipping);
    printf("HST (13%%)\t\t$ %.2f\n", hst);
    printf("--------------------------------\n");
    printf("Amount Charged\t\t$ %.2f\n\n", amountCharged);
    printf("Individual boxes are packed as follows:\n\n");
    printf("%s", boxString);

    return 0;
}