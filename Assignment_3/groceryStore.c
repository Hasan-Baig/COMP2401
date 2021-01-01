/*
 * Author: Hasan Baig
 * Student Number: 101032292
 * Date: November 06 2020
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_NUMBER_OF_ITEMS_IN_CART  100
#define MAX_WEIGHT_OF_BAG            5.0
#define MAX_NUMBER_OF_ITEMS_IN_BAG   25

typedef struct {
    char  *name;
    float  itemPrice;
    float  itemWeight;
    float  itemPerish; //refrigrated or frozen
} GroceryItem;

typedef struct {
    GroceryItem  bagItems[MAX_NUMBER_OF_ITEMS_IN_BAG];
    int          countItemsInBag;
    float        bagWeight;
} Bag;

typedef struct {
    GroceryItem  cartItems[MAX_NUMBER_OF_ITEMS_IN_CART];
    int countItemsInCart;
    Bag cartBags;
    int countBagsInCart;
} Cart;



//Function prototypes (forward reference)
void addItemToCart(GroceryItem *item, Cart *cart);
char addItemToBag(GroceryItem *item, Bag bag);
char removeItemFromBag(GroceryItem *item, Bag bag);
void displayItem(GroceryItem *item);                //DONE
void displayBag(Bag *bag, int numBag);              //DONE
void displayCart(Cart *cart);
void packItemsIntoBags(Cart *cart);
void removePerishableItemFromCart(Cart *cart);


int main() {
    
    GroceryItem testItems[12];

    char *sampleItemNames[] = {
        "Smart-Ones Frozen Entrees", "SnackPack Pudding",
        "Breyers Chocolate Icecream", "Nabob Coffee", "Gold Seal Salmon",  
        "Ocean Spray Cranberry Cocktail", "Heinz Beans Original", "Lean Ground Beef", 
        "5-Alive Frozen Juice", "Coca-Cola 12-pack", "Toilet Paper - 48 pack", "Crate of milk"
    };

    float sampleItemPrices[] = {1.99, 0.99, 2.99, 3.99, 1.99, 2.99, 0.79, 4.94, 0.75, 3.49, 40.96, 12.99};
    float sampleItemWeights[] = {0.311, 0.396, 2.27, 0.326, 0.213, 2.26, 0.477, 0.75, 0.426, 5.112, 10.89f, 6.18f};
    float sampleItemPerish[] = {1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1};

    // populate the testItems array with the data above to initialize all the items
    for(int i=0; i<(sizeof(testItems)/sizeof(GroceryItem)); i++){ 
        testItems[i].name = sampleItemNames[i];
        testItems[i].itemPrice = sampleItemPrices[i];
        testItems[i].itemWeight = sampleItemWeights[i];
        testItems[i].itemPerish = sampleItemPerish[i];
    }

    // GroceryItem *testItemsPtr[12];

    // for(int i=0; i<12; i++){
    //     testItemsPtr[i] = &testItems[i];
    // }

    // add random items to shoppingCart from testItems
    Cart shoppingCart;
    shoppingCart.countBagsInCart=0;
    shoppingCart.countItemsInCart=0;
    srand(time(NULL));
    for (int i=0; i<20; i++) {
        addItemToCart(&testItems[(int)(rand()/(double)RAND_MAX*12)], &shoppingCart);
    }

    //*********************************************************
    // TEST
    // for(int i=0; i<12; i++){
    //     displayItem(testItemsPtr[i]);
    // }
    // Bag bag[2];
    // Bag *bagPtr[2];

    // bag[0].bagItems[0].name = "iahksjd";
    // bag[0].bagItems[0].itemPrice = 1.99;
    // bag[0].bagItems[0].itemWeight = 0.311;
    // bag[0].bagItems[1].name = "erwer";
    // bag[0].bagItems[1].itemPrice = 2.99;
    // bag[0].bagItems[1].itemWeight = 0.750;
    // bag[0].bagItems[2].name = "dsfsdf";
    // bag[0].bagItems[2].itemPrice = 3.99;
    // bag[0].bagItems[2].itemWeight = 0.650;
    // bag[1].bagItems[0].name = "iahksjd";
    // bag[1].bagItems[0].itemPrice = 1.99;
    // bag[1].bagItems[0].itemWeight = 0.411;
    // bag[1].bagItems[1].name = "erwer";
    // bag[1].bagItems[1].itemPrice = 2.99;
    // bag[1].bagItems[1].itemWeight = 0.850;
    // bag[1].bagItems[2].name = "dsfsdf";
    // bag[1].bagItems[2].itemPrice = 3.99;
    // bag[1].bagItems[2].itemWeight = 0.250;

    // for(int i=0; i<2; i++){
    //     bagPtr[i] = &bag[i];
    // }
    // for(int i=0; i<2; i++){
    //     displayBag(bagPtr[i], i+1);
    // }
    //*********************************************************
    

    //Shopping Cart Information
    printf("\n------------------------------------------\n");
    printf("CART BEFORE PACKING BAGS\n\n");
    displayCart(&shoppingCart);
    printf("\n\n");
    printf("------------------------------------------\n\n");
    
    packItemsIntoBags(&shoppingCart);

    printf("------------------------------------------\n");
    printf("CART AFTER PACKING BAGS\n\n");
    displayCart(&shoppingCart);
    printf("\n\n");
    printf("------------------------------------------\n\n");

    removePerishableItemFromCart(&shoppingCart);

    printf("------------------------------------------\n");
    printf("CART AFTER REMOVING PERISHABLES\n\n");
    displayCart(&shoppingCart);
    printf("\n\n");
    printf("------------------------------------------\n");

    // free memory
    // for (int i=0; i<(shoppingCart.countBagsInCart); i++){
    //     free(shoppingCart.cartBags[i]);
    // }
    // free(shoppingCart.cartBags);

    return 0;
}

/*************************************************************
 * Purpose: adds grocery item to a shopping cart
 * Input: pointer to item and pointer to cart
 *************************************************************/
void addItemToCart(GroceryItem *item, Cart *cart){
    for(int i=0; i<(sizeof(cart->cartItems)/sizeof(GroceryItem)); i++){
        if(cart->cartItems[i].name != NULL){
            continue;
        } else {
            cart->cartItems[i] = *item;
            break;
        }
    }
}

/*************************************************************
 * Purpose: remove grocery item from a shopping cart
 * Input: pointer to item and pointer to cart
 *************************************************************/
// void removeItemFromCart(GroceryItem *item, Cart *cart){
//     for(int i=(sizeof(cart->cartItems)/sizeof(GroceryItem)); i>0; i--){
//         if(cart->cartItems[i].name == NULL){
//             continue;
//         } else {
//             cart->cartItems[i] = cart->cartItems[i+1];
//             break;
//         } 
//     }
// }    

/*************************************************************
 * Purpose: adds grocery item to a bag
 * Input: pointer to item and bag
 * Ouput: 1 if successful and 0 otherwise
 *************************************************************/
char addItemToBag(GroceryItem *item, Bag bag){

    for(int i=0; i<(sizeof(bag.bagItems)/sizeof(GroceryItem)); i++){
        if(bag.bagItems[i].name != NULL){
            continue;
        } else {
            bag.bagItems[i] = *item;
            return 1;
        }
    }
    return 0;
}

/*************************************************************
 * Purpose: remove grocery item from a bag
 * Input: pointer to item and bag
 * Ouput: 1 if successful and 0 otherwise
 *************************************************************/
char removeItemFromBag(GroceryItem *item, Bag bag){
    for(int i=(sizeof(bag.bagItems)/sizeof(GroceryItem)); i>0; i--){
        if(bag.bagItems[i].name == NULL){
            continue;
        } else {
            bag.bagItems[i] = bag.bagItems[i+1];
            return 1;
        } 
    }
    return 0;
}

/*************************************************************
 * Purpose: displays variable within GroceryItem struct
 * Input: pointer to item
 *************************************************************/
void displayItem(GroceryItem *item) {
    printf("%s weighing %4.3fkg with price $%3.2f\n", item->name, item->itemWeight, item->itemPrice);
}

/*************************************************************
 * Purpose: displays items within a bag
 * Input: pointer to bag and bag number
 *************************************************************/
void displayBag(Bag *bag, int numBag){
    float totalWeight;
    //calculate total weight
    for (int a=0; a<(sizeof(bag->bagItems)/sizeof(GroceryItem)); a++){
        totalWeight+= bag->bagItems[a].itemWeight;
    } 

    printf("BAG %d (Total Weight = %4.3fkg)\n", numBag, totalWeight);

    float heaviestWeight = 0;
    //find heaviest weigth
    for(int i=0; i<(sizeof(bag->bagItems)/sizeof(GroceryItem)); i++){
        if(bag->bagItems[i].itemWeight > heaviestWeight){
            heaviestWeight = bag->bagItems[i].itemWeight;
        }
    }

    //print bag
    for(int j=0; j<(sizeof(bag->bagItems)/sizeof(GroceryItem)); j++){
        if(bag->bagItems[j].name == NULL){
           continue; 
        }
        printf("\t");
        if (bag->bagItems[j].itemWeight == heaviestWeight){
            printf("*");
        } else {
            printf(" ");
        }
        displayItem(&(bag->bagItems[j]));
    }

    printf("\n");
}

/*************************************************************
 * Purpose: displays unpacked items within a cart and packed items in bags in a cart
 * Input: pointer to cart
 *************************************************************/
void displayCart(Cart *cart){

    for(int i=0; i<(sizeof(cart->cartItems)/sizeof(GroceryItem)); i++){
        if(cart->cartItems[i].name == NULL){
           continue; 
        } else {
            displayItem(&(cart->cartItems[i]));
        }
    }

    printf("\n");

    // for(int j=0; j<(sizeof(cart->cartBags)/sizeof(Bag)); j++){
    //     displayBag(&cart->cartBags[j], j+1);
    // }

    Bag *cartBagPtr;
    
    for(int i=0; i<(sizeof(cart->cartBags)/sizeof(Bag)); i++){
        cartBagPtr[i] = cart->cartBags;
    }

    for(int i=0; i<(sizeof(cart->cartBags)/sizeof(Bag)); i++){
        displayBag(&cartBagPtr[i], i+1);
    }

    printf("\n");
}


/*************************************************************
 * Purpose: packs all items in cart into bags
 * Input: pointer to cart
 *************************************************************/
void packItemsIntoBags(Cart *cart){
    float tempBagWeight = 0;
    cart->countBagsInCart = 0;
    //determines exact number of bags needed
    for(int i=0; i<(sizeof(cart->cartItems)/sizeof(GroceryItem)); i++){
        // ignore items with weight>5kg
        if((cart->cartItems[i].itemWeight > MAX_WEIGHT_OF_BAG)){
           continue; 
        } else {
            //if max weight in bag reached, add 1 bag and set tempbag to prev weight
            if(tempBagWeight > MAX_WEIGHT_OF_BAG){
                cart->countBagsInCart++;
                tempBagWeight = cart->cartItems[i-1].itemWeight;
            }
            tempBagWeight += cart->cartItems[i].itemWeight;
            //add extra bag for last items if total weight less than 5kg
            if(cart->cartItems[i].name == NULL && (tempBagWeight!=0)){
                cart->countBagsInCart++;
                break;
            }
        }
    }

    Bag *b = &cart->cartBags;
    
    //dynamically allocate number of bags needed
    b = (struct Bag *)malloc(sizeof(Bag) * cart->countBagsInCart);
	// check if allocation was successful
	if (b == NULL) {
		printf("error: could not allocate memory for the array \n");
		return(1);
	}

    //pack into bags
    for(int j=0; j<(cart->countBagsInCart); j++){
        // ignore items with weight>5kg
        if((cart->cartItems[j].itemWeight > MAX_WEIGHT_OF_BAG)){
           continue; 
        } else {
            addItemToBag(&cart->cartItems[j], b[j]);


            for(int j=0; j<(cart->countBagsInCart); j++){

            }



            //if max weight in bag reached, add 1 bag and set tempbag to prev weight
            if(tempBagWeight > MAX_WEIGHT_OF_BAG){
                cart->countBagsInCart++;
                tempBagWeight = cart->cartItems[i-1].itemWeight;
            }
            tempBagWeight += cart->cartItems[i].itemWeight;
            //add extra bag for last items if total weight less than 5kg
            if(cart->cartItems[i].name == NULL && (tempBagWeight!=0)){
                cart->countBagsInCart++;
                break;
            }
        }
    }
}