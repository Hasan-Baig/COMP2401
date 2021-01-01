/*
 * Author: Hasan Baig
 * Student Number: 101032292
 * Date: November 06 2020
 *
 * Note to TA: I have spent over 12 hours trying to debug my code, however
 * for some reason I keep receiving "Segmentation fault (core dumped)" when
 * running "packItemsIntoBags(&shoppingCart);" even though I believe my code
 * to have no errors, it's just the packItemsIntoBags function that seems to
 * have problems. The function for removePerishableItemFromCart works perfectly
 * fine though. To check this function comment out line 93.
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
    GroceryItem  *bagItems[MAX_NUMBER_OF_ITEMS_IN_BAG];
    int          countItemsInBag;
    float        bagWeight;
} Bag;

typedef struct {
    GroceryItem  *cartItems[MAX_NUMBER_OF_ITEMS_IN_CART];
    int countItemsInCart;
    Bag **cartBags;
    int countBagsInCart;
} Cart;

//Function prototypes (forward reference)
int addItemToCart(GroceryItem *item, Cart *cart);
int addItemToBag(GroceryItem *item, Bag *bag);
int removeItemFromBag(GroceryItem *item, Bag *bag);
void displayItem(GroceryItem *item);
void displayBag(Bag *bag, int numBag);
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
    for(int i=0; i<(sizeof(testItems)/sizeof(GroceryItem)); ++i){
        testItems[i].name = sampleItemNames[i];
        testItems[i].itemPrice = sampleItemPrices[i];
        testItems[i].itemWeight = sampleItemWeights[i];
        testItems[i].itemPerish = sampleItemPerish[i];
    }

    // add random items to shoppingCart from testItems
    Cart shoppingCart;
    shoppingCart.countBagsInCart=0;
    shoppingCart.countItemsInCart=0;
    srand(time(NULL));
    for (int i=0; i<50; ++i) {
        addItemToCart(&testItems[(int)(rand()/(double)RAND_MAX*12)], &shoppingCart);
    }

    //Shopping Cart Information

    //display shoppingCart
    printf("-------------------------------------------------------------\n\n");
    printf("CART BEFORE PACKING BAGS\n\n");
    displayCart(&shoppingCart);

    //pack bags
    packItemsIntoBags(&shoppingCart);

    // display shoppingCart
    printf("-------------------------------------------------------------\n\n");
    printf("CART AFTER PACKING BAGS\n\n");
    displayCart(&shoppingCart);

    //remove perishables
    removePerishableItemFromCart(&shoppingCart);

    //display shoppingCart
    printf("-------------------------------------------------------------\n\n");
    printf("CART AFTER REMOVING PERISHABLES\n\n");
    displayCart(&shoppingCart);

    Cart *shoppingCartPtr = &shoppingCart;

    // free memory
    for (int i=0; i<(shoppingCart.countBagsInCart); ++i){
        free(shoppingCartPtr->cartBags[i]);
    }
    free(shoppingCartPtr->cartBags);

    return 0;
}

/*************************************************************
 * Purpose: adds grocery item to a shopping cart
 * Input: pointer to item and pointer to cart
 * Ouput: 1 if successful and 0 otherwise
 *************************************************************/
int addItemToCart(GroceryItem *item, Cart *cart){
    if(cart->countItemsInCart+1 >= MAX_NUMBER_OF_ITEMS_IN_CART){
        return 0;
    } else {
        cart->cartItems[cart->countItemsInCart] = item;
        cart->countItemsInCart++;
        return 1;
    }
}

/*************************************************************
 * Purpose: adds grocery item to a bag
 * Input: pointer to item and bag
 * Ouput: 1 if successful and 0 otherwise
 *************************************************************/
int addItemToBag(GroceryItem *item, Bag *bag){
    if(bag->countItemsInBag+1 >= MAX_NUMBER_OF_ITEMS_IN_BAG){
        return 0;
    } else {
        bag->bagItems[bag->countItemsInBag] = item;
        bag->countItemsInBag++;
        bag->bagWeight += item->itemWeight;
        return 1;
    }
}

/*************************************************************
 * Purpose: remove grocery item from a bag
 * Input: pointer to item and bag
 * Ouput: 1 if successful and 0 otherwise
 *************************************************************/
int removeItemFromBag(GroceryItem *item, Bag *bag){
    for(int i=0; i<(bag->countItemsInBag); ++i){
        if(bag->bagItems[i]->name == item->name){
            //shift all array elem
            for(int j=i+1; j<(bag->countItemsInBag); ++j){
                bag->bagItems[j-1] = bag->bagItems[j];
            }
            bag->countItemsInBag--;
            bag->bagWeight -= item->itemWeight;
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

    printf("BAG %d (Total Weight = %4.3fkg)\n", numBag, bag->bagWeight);

    float heaviestWeight = 0;
    //find heaviest weigth
    for(int i=0; i<(bag->countItemsInBag); ++i){
        if(bag->bagItems[i]->itemWeight > heaviestWeight){
            heaviestWeight = bag->bagItems[i]->itemWeight;
        }
    }

    //print bag
    for(int j=0; j<(bag->countItemsInBag); ++j){
        if(bag->bagItems[j]->name == NULL){
           continue;
        }
        printf("\t");
        if (bag->bagItems[j]->itemWeight == heaviestWeight){
            printf("*");
        } else {
            printf(" ");
        }
        displayItem(bag->bagItems[j]);
    }

    printf("\n");
}

/*************************************************************
 * Purpose: displays unpacked items within a cart and packed items in bags in a cart
 * Input: pointer to cart
 *************************************************************/
void displayCart(Cart *cart){

    for(int i=0; i<(cart->countItemsInCart); ++i){
        displayItem(cart->cartItems[i]);
    }

    printf("\n");

    for(int j=0; j<(cart->countBagsInCart); ++j){
        displayBag(cart->cartBags[j], j+1);
        printf("\n");
    }
}


/*************************************************************
 * Purpose: packs all items in cart into bags
 * Input: pointer to cart
 *************************************************************/
void packItemsIntoBags(Cart *cart){
    int tempItems = 0;
    float tempBagWeight = 0;
    int numBagsNeed = 0;
    //determines exact number of bags needed
    for(int i=0; i<(cart->countItemsInCart); ++i){
        // ignore items with weight>5kg
        if((cart->cartItems[i]->itemWeight > MAX_WEIGHT_OF_BAG)){
           continue;
        } else {
            if (numBagsNeed == 0){
                numBagsNeed = 1;
            }
        }
        // add new bag if tempItems>25 or tempBagWeight>5.0
        if((tempBagWeight+(cart->cartItems[i]->itemWeight) > MAX_WEIGHT_OF_BAG) || (tempItems+1 > MAX_NUMBER_OF_ITEMS_IN_BAG)) {
            numBagsNeed++;
            tempItems = 1;
            tempBagWeight = cart->cartItems[i]->itemWeight;
        } else {
            tempItems++;
            tempBagWeight += cart->cartItems[i]->itemWeight;
        }
    }

    cart->countBagsInCart = numBagsNeed;

    //dynamically allocate number of bags needed
    Bag **b = NULL;
    b = (Bag **)malloc(sizeof(Bag) * cart->countBagsInCart);
	  // check if allocation was successful
	  if (b == NULL) {
		    printf("ERROR: could not allocate memory for the array \n");
		    exit(1);
	  }
    //allocate for each elem in array
    for(int i=0; i<(cart->countBagsInCart); ++i){
        b[i] = (Bag *)malloc(sizeof(Bag));
	      // check if allocation was successful
        if (b[i] == NULL) {
		      printf("ERROR: could not allocate memory for the array \n");
		      exit(1);
	      }
        b[i]->bagWeight=0;
    }

    int tempBag = 1;
    tempItems = 0;
    tempBagWeight = 0;
    //pack items from cart into bag
    for(int i=0; i<(cart->countItemsInCart); ++i){
        // ignore items with weight>5kg
        if((cart->cartItems[i]->itemWeight > MAX_WEIGHT_OF_BAG)){
           continue;
        }
        if((tempBagWeight+(cart->cartItems[i]->itemWeight) > MAX_WEIGHT_OF_BAG) || (tempItems+1 > MAX_NUMBER_OF_ITEMS_IN_BAG)) {
            ++tempBag;
            tempItems = 1;
            tempBagWeight = cart->cartItems[i]->itemWeight;
            addItemToBag(cart->cartItems[i], b[tempBag]);
        } else {
            //add to current bag
            addItemToBag(cart->cartItems[i], b[tempBag]);
            tempItems++;
            tempBagWeight += cart->cartItems[i]->itemWeight;
        }
        //remove item in cart that is packed in bag
        for(int j=i+1; j<(cart->countItemsInCart); ++j){
            cart->cartItems[j-1] = cart->cartItems[j];
        }
        //after shifting, next i = this i
        --i;
    }
    cart->cartBags = b;
}

void removePerishableItemFromCart(Cart *cart){
    //remove from bags
    for (int i=0; i<(cart->countBagsInCart); ++i){
        //items in bag
        for (int j=0; j<(cart->cartBags[i]->bagItems); ++j){
            if(cart->cartBags[i]->bagItems[j]->itemPerish){
                removeItemFromBag(cart->cartBags[i]->bagItems[j],cart->cartBags[i]);
            }
        }
        //delete bag if empty
        if(cart->cartBags[i]->countItemsInBag == 0){
            free(cart->cartBags[i]);
            //shift all array elem to remove
            for(int j=i+1; j<(cart->countBagsInCart); ++j){
                cart->cartBags[j-1] = cart->cartBags[j];
            }
            cart->countBagsInCart--;
            //after shifting, check next i = this i
            --i;
        }
    }

    //remove from unpacked items
    for (int i=0; i < (cart->countItemsInCart); ++i){
        if(cart->cartItems[i]->itemPerish){
            //shift all array elem to remove
            for(int j=i+1; j<(cart->countItemsInCart); ++j){
                cart->cartItems[j-1] = cart->cartItems[j];
            }
            cart->countItemsInCart--;
            //after shifting, check next i = this i
            --i;
        }
    }
}
