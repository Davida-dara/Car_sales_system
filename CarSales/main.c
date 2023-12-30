#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <errno.h>

/*Global Constants */
#define MAX_CAR_BRANDS 5
#define DISCOUNT 0.3f
#define INITIAL_AVAILABILITY 2

/*Menu constants*/
#define VIEW_CARS 'a'
#define BUY_CAR  'b'
#define VIEW_SALES  'c'
#define CUSTOMER_FEEDBACK 'd'
#define EXIT 'x'

#define FILER "Car Sales.csv"

/* Global Variables */
// Structures
typedef struct {
    short saleNumber;
    char name[51];
    char purchasedCar[12];
    short yearManufactured;
    short amountPurchased;
    short age;
    char gotDiscount[7];
    float finalePrice;
} saleTransactions;

typedef struct {
    char carName[10];
    unsigned short carPrice;
    unsigned short amountAvailable;
    short yearOfManufacture;
} car;

typedef struct {
    char CustomerName[201];
    int happinessRating;
    char customerFeedback[351];
    char Time[11];
} information;

//Array of Struct Cars

car carBrandsArr[] = {{"Tesla",  45000, INITIAL_AVAILABILITY, 2019},
                      {"Lexus",  23490, INITIAL_AVAILABILITY, 2022},
                      {"BMW",    19712, INITIAL_AVAILABILITY, 2021},
                      {"Suzuki", 16749, INITIAL_AVAILABILITY, 2023},
                      {"Ford",   16470, INITIAL_AVAILABILITY, 2020}
};

//Array of Struct saleTrancactions
saleTransactions salesArr[MAX_CAR_BRANDS * INITIAL_AVAILABILITY];

//Array of customer feedback
information feedback[MAX_CAR_BRANDS * INITIAL_AVAILABILITY];

//

/*short*/
unsigned short transactionIndex = 0, totalAvailableCars, totalCarsBought = 0, feedbackCounter = 0;
float totalSalesValue = 0; //consider changing this to a float variable

char discountString[7] = "False", sortingMessage[201];

FILE *dataFile;
int errorNumber;
bool fileOpen = false;

/*Working with files*/

FILE *createDataFile(char *fileName) {
    dataFile = fopen(fileName, "w");
    //close file immediately if datafile was created successfully
    if (dataFile != NULL) {
        fclose(dataFile);
    }
    return dataFile;
}

void closeDataFile() {
    fclose(dataFile);
}

void populateDataFile() {
    /* fprintf(dataFile,
             "\nTransaction index,Customer name,Customer age,Car purchased,Number of cars,Value of purchase,Discount given\n");*/
    for (int i = 0; i < transactionIndex; ++i) {
        fprintf(dataFile, "%hd,%s,%hd,%s,%hd,%hd,%.2f, %s\n",
                salesArr[i].saleNumber, salesArr[i].name, salesArr[i].age, salesArr[i].purchasedCar,
                salesArr[i].yearManufactured,
                salesArr[i].amountPurchased, salesArr[i].finalePrice, salesArr[i].gotDiscount);
    }
    //Close the datafile
    closeDataFile();
    fileOpen = false;
}

void printFileErrorMessage(char *fileName) {
    perror("Error message");
    printf("The program has encountered an error which prevents it from opening the %s file: %s", fileName,
           strerror(errorNumber));
}

bool openDataFile(char *fileName, char *mode) {
    dataFile = fopen(fileName, mode);

    if (dataFile == NULL) {

        //try to create the file on demand and check to see if file was opened successfully
        if (createDataFile(fileName) == NULL) {

            //store errno (contains the numeric value that represents the error that just took place) in our errorNumber.
            errorNumber = errno;
            printFileErrorMessage(fileName);
            return false;
        }

    } else return true;
}

void updateDataFile() {

    //Open the CSV file
    fileOpen = openDataFile(FILER, "w");

    if (fileOpen) {
        populateDataFile();
    }
}

/*End of file functions*/

void initiateExitProcedure() {
    printf("\n\n...press enter to exit the program");
    printf("\nThank you for using our Car Sales program. Good-bye!\n");
    getchar();

    updateDataFile();
    exit(0);
}

void clearScreen() {
    // resets/clears the users screen
    system("cls");
}

void greeting() {
    printf("\nHello!, Welcome to the Ruskin Fundraising Carsales office\n");
}

void printMenu() {
    //Displays the menu options to the user
    printf("\nMenu:\n");
    printf("%c. View car stock\n", VIEW_CARS);
    printf("%c. Purchase a car\n", BUY_CAR);
    printf("%c. View car Sales\n", VIEW_SALES);
    printf("%c. View customer feedback\n", CUSTOMER_FEEDBACK);
    printf("%c. Exit program\n", EXIT);
}

void displayCarStats() {
    //display sortingMessage
    puts(sortingMessage);

    //Display the cars data
    printf("No.\tCar\tPrice\tAvailable  Year of Manufacture\n");
    short i;
    for (i = 0; i < MAX_CAR_BRANDS; i++) {
        printf("%hd\t%s\t%d\t %d \t\t%hd\n", i + 1, carBrandsArr[i].carName, carBrandsArr[i].carPrice,
               carBrandsArr[i].amountAvailable, carBrandsArr[i].yearOfManufacture);
    }
}

void updateAmountAvailable(short position, short reduction) {
    //Update the available amount
    carBrandsArr[position].amountAvailable -= reduction;
}

void updateTotalCarsAvailable() {
    totalAvailableCars = 0;
    for (int i = 0; i < MAX_CAR_BRANDS; ++i) {
        totalAvailableCars += carBrandsArr[i].amountAvailable;
    }
}

char retrieveInput(char message[]) {
    bool validated = false;
    //Displays a message prompting user to enter a char input, then receives and returns the entered char input.
    char userInput;

    do {
        puts(message);

        if ((scanf("%c", &userInput)) != 1) {
            //Inform the user an invalid input has been entered
            printf("You have entered an invalid input. please enter a valid letter option.");
            //update the validation flag
            validated = false;
        } else {
            //update the validation flag
            validated = true;
        }

        //clear the input buffer
        while (getchar() != '\n');
    ///    getchar();
    } while (!validated);
// allow user to end program at any point in time
    if (userInput == EXIT) {
        initiateExitProcedure();
    }

    return userInput;
}

void retrieveStringInput(char *message, char *retrievedStr) {
    puts(message);
    scanf("\n%[^\n]", retrievedStr);
    getchar();

    // allow user to end program at any point in time
    if (retrievedStr[0] == EXIT) {
        initiateExitProcedure();
    }
}

unsigned short retrieveShortInput(char message[]) {
    /*Local variables*/
    unsigned short userInput;
 //   char choice;
    bool validated = true;

    do {
        puts(message);

        if (scanf("%hd", &userInput) != 1) {

            //Inform the user an invalid input has been entered
            printf("\nYou have entered an invalid input. Please enter a valid integer value!\n");

            //update the validation flag
            validated = false;

        } else {
            //update the validation flag
            validated = true;
        }
        //clear the input buffer
        getchar();
    } while (!validated);
//allows the user to exit the program by pressing 'x'.

    return userInput;
}

void sortAccordingToPrice() {
    car temp;
    for (int m = 0; m < MAX_CAR_BRANDS - 1; m++) {
        for (int n = m + 1; n < MAX_CAR_BRANDS; n++) {
            if (carBrandsArr[m].carPrice < carBrandsArr[n].carPrice) {
                temp = carBrandsArr[m];
                carBrandsArr[m] = carBrandsArr[n];
                carBrandsArr[n] = temp;
            }
        }
    }
}

void sortAccordingToYoM() {
    car temp;
    for (int m = 0; m < MAX_CAR_BRANDS - 1; m++) {
        for (int n = m + 1; n < MAX_CAR_BRANDS; n++) {
            if (carBrandsArr[m].yearOfManufacture < carBrandsArr[n].yearOfManufacture) {
                temp = carBrandsArr[m];
                carBrandsArr[m] = carBrandsArr[n];
                carBrandsArr[n] = temp;
            }
        }
    }
}

void setSortMessage(char attribute) {
    if (attribute == 'P') {
        strcpy(sortingMessage,
               "\nHere's a list of car brands we have available for you sorted in descending order according to their prices.\n");
    } else if (attribute == 'M') {
        strcpy(sortingMessage,
               "\nHere's a list of the car brands we have available for you sorted in descending order according to their Year of manufacture.\n");
    }
}

void sortCars() {
    char sort;
    bool validFlag = false;

    //display prompt message
    do {
        puts("\nWould you like to view our car stock sorted according to the price or year of manufacture of each car.");
        sort = toupper(retrieveInput("Please enter P for price or M for year of Manufacture\nEnter: "));

        if (sort == 'P') {
            //call function to sort cars according to their Prices
            sortAccordingToPrice();
            validFlag = true;
        } else if (sort == 'M') {
            //call function to sort cars according to their Year of Manufacture
            sortAccordingToYoM();
            validFlag = true;
        } else {
            puts("You have entered an invalid input.\n");
            validFlag = false;
        }
    } while (!validFlag);

    setSortMessage(sort);
}

bool responseValidation(char response) {
    if (response == 'y') {
        return true;
    } else if (response == 'n') {
        return true;
    } else {
        printf("\nPlease enter either \'y\' for yes or \'n\' for no.\n");
        //   validated = false;
        return false;
    }
}

bool checkAvailability(int position) {
    if (carBrandsArr[position].amountAvailable == 0) {
        printf("Sorry, there are no more %s cars available for purchase. Please select another car option.\n\n",
               carBrandsArr[position].carName);
        return false;
    }
    return true;
}

bool checkSufficiency(int position, int userInput) {
    if (userInput > carBrandsArr[position].amountAvailable) {
        printf("\nSorry, there are fewer cars available than the amount you require.\n");

        if (carBrandsArr[position].amountAvailable == 1) {
            printf("There is only %d %s car available.\n\n", carBrandsArr[position].amountAvailable,
                   carBrandsArr[position].carName);
        } else {
            printf("There are only %hd %s cars available.\n\n", carBrandsArr[position].amountAvailable,
                   carBrandsArr[position].carName);
        }

        return false;
    }
    return true;
}

bool checkValidShortEntered(int userInput, int Max) {
    if (userInput > 0 && userInput <= Max) {
        return true;
    } else {
        printf("\nYou have entered an invalid car number. please enter between 1 and %d\n", Max);
    }
    return false;
}

bool giveDiscount(int age) {
    //determines weather or not the user qualifies for a discount.
    short maxDiscountAge = 22, minDiscountAge = 17;
    char student;
    bool validated = true;

    if ((age > maxDiscountAge) || (age < minDiscountAge)) {
        do {
            student = tolower(retrieveInput("Are you a student? [y = yes, n = no]\nAnswer: "));
            //consider using a switch statement in here
            validated = responseValidation(student);
            /*   if (student == 'y') {
                   validated = true;
                   return true;
               } else if (student == 'n') {
                   validated = true;
                   return false;
               } else {
                   printf("\nYou have entered an invalid input. please enter either \'y\' for yes or \'n\' for no.\n");
                   validated = false;
                   //   return false;
               } */

        } while (!validated);

        if (student == 'y') {
            return true;
        } else if (student == 'n') {
            return false;
        }

    }
    return true;
}

float implementDiscount(short position) {
    //Claculate and return the new price after the 30% discount has been applied
    return carBrandsArr[position].carPrice * (1 - DISCOUNT);
}

void displaySalesStats() {
    //Reset total cars bought and total sales value
    totalCarsBought = 0;
    totalSalesValue = 0;

    //Displays Sales Stat
    for (short i = 0; i < transactionIndex; i++) {
        printf("\nTransaction index: %hd | Customer name: %s | Customer age: %hd | Car purchased: %s | Car year of manufacture: %hd "
               "\n| Number of cars: %hd | Value of purchase(GBP): %.2f | Discount given: %s\n",
               salesArr[i].saleNumber, salesArr[i].name, salesArr[i].age, salesArr[i].purchasedCar,
               salesArr[i].yearManufactured,
               salesArr[i].amountPurchased, salesArr[i].finalePrice, salesArr[i].gotDiscount);
        totalCarsBought += salesArr[i].amountPurchased;
        totalSalesValue += salesArr[i].finalePrice;
    }

    //dispaly overall statistics
    if (totalCarsBought == 1){
        printf("\n%hd car has been sold with a total value of %f GBP. There are %hd cars unsold.\n",
               totalCarsBought, totalSalesValue, totalAvailableCars);
    } else {
        printf("\n%hd cars have been sold with a total value of %f GBP. There are %hd cars unsold.\n",
               totalCarsBought, totalSalesValue, totalAvailableCars);
    }
}

void populateSalesArr(short index, char fullName[201], short position, short customerAge, short req, char discounted[7],
                      float charged) {
// charged : how mush the user was charged
//position : the position of the car chosen in the carBrandsArr structure e.g 0 for Tesla
//req : the amount of cars the user purchased

    salesArr[transactionIndex].saleNumber = index;
    strcpy(salesArr[transactionIndex].name, fullName);
    strcpy(salesArr[transactionIndex].purchasedCar, carBrandsArr[position].carName);
    salesArr[transactionIndex].yearManufactured = carBrandsArr[position].yearOfManufacture;
    salesArr[transactionIndex].age = customerAge;
    salesArr[transactionIndex].amountPurchased = req;
    strcpy(salesArr[transactionIndex].gotDiscount, discounted);
    salesArr[transactionIndex].finalePrice = charged;

}

/*More file functions*/
void retrieveFileData() {
    int lineCounter = 0;
    int lineNumber;

    while (1) {
        char tempName[201], tempdiscount[7], tempCar[12];
        short tempAge, tempManufactured, tempCarsPurcahsed;
        unsigned int tempCharge;

        int scanfResult = fscanf(dataFile, "%hd,%[^\,],%hd,%[^\,],%hd,%hd,%.2f,%s", &lineNumber, &tempName, &tempAge,
                                 &tempCar, &tempManufactured, &tempCarsPurcahsed, &tempCharge, &tempdiscount);

        if (scanfResult == EOF) {
            break;
        }

        //
        salesArr[lineCounter].saleNumber = lineNumber;
        strcpy(salesArr[lineCounter].name, tempName);
        salesArr[lineCounter].age = tempAge;
        strcpy(salesArr[lineCounter].purchasedCar, tempCar);
        salesArr[lineCounter].yearManufactured = tempManufactured;
        salesArr[lineCounter].amountPurchased = tempCarsPurcahsed;
        salesArr[lineCounter].finalePrice = tempCharge;
        strcpy(salesArr[lineCounter].gotDiscount, tempdiscount);

        printf("%hd", salesArr[lineCounter].saleNumber);

        //reset the number of cars available per brand according to previous sales transactions
        for (int i = 0; i < MAX_CAR_BRANDS; i++) {
            if (strcmp(salesArr[lineCounter].purchasedCar, carBrandsArr[i].carName) == 0) {
                carBrandsArr[i].amountAvailable -= salesArr[lineCounter].amountPurchased;
            }
        }

        //increment line counter as you move to the next line in the file
        lineCounter++;
    }

    transactionIndex = lineCounter;

    //count the number of cars available
    updateTotalCarsAvailable();
}

void readSalesDataFromFile(char *fileName) {
    fileOpen = openDataFile(FILER, "r");

    if (fileOpen) {
        retrieveFileData();
        closeDataFile();
        fileOpen = false;
    } else {
        printFileErrorMessage(fileName);
        return;
    }
}
/* End of file functions*/

void collectFeedback() {
    unsigned short happinessScale;
    char userComment[351];
    char commenting;
    bool validResp = true;

    do {
        happinessScale = retrieveShortInput(
                "Before you leave please rate your experiences with working with us on a scale of 1 - 5\n"
                "(1 being terrible and 5 being Excellent)\n");

        //input validation for when an invalid car number has been entered e.g.6
        validResp = checkValidShortEntered(happinessScale, 5);

    } while (!validResp);

            do {commenting = tolower(
                        retrieveInput("Would you like to leave us a short comment as well? [y = yes, n = no]\nAnswer: "));
                //validate input entered
                validResp = responseValidation(commenting);
            } while (!validResp);




    if (commenting == 'y') {
        retrieveStringInput("Please enter your comment below:", userComment);
    } else if (commenting == 'n') {
        strcpy(userComment, "no comment made.");
    }

    feedback[feedbackCounter].happinessRating = happinessScale;
    strcpy(feedback[feedbackCounter].customerFeedback, userComment);
}

//Main menu action functions
void buyingCars() {

    unsigned short amountReq, userAge, selectedCar;

    // monitor the position car that a transaction is taking place on.
    short carBrandIndex;

    float finaleCost;
    bool discount, carAvailable;

    char customerName[201];

    clearScreen();

    printf("Purchase car:\n");

    //display car types
    displayCarStats();

    //Ask for the username
//    printf("What is your name?\nName: ");
    //Retrieve user name
    retrieveStringInput("What is your name?\nName: ", customerName);
 //   scanf("\n%[^\n]s", customerName);
//    getchar();
//    strcpy(transactions[transactionIndex]->names, customerName);


    do {
        // Ask which car the user would like to purchase... and retrieve response...
        selectedCar = retrieveShortInput("\nWhich Car would you like to purchase?"
                                         " (Enter the number of the car you would like to purchase)\nNumber: ");

        //input validation for when an invalid car number has been entered e.g.6
        carAvailable = checkValidShortEntered(selectedCar, MAX_CAR_BRANDS);

        if (carAvailable) {

            carBrandIndex = selectedCar - 1;

            //Input validation to check car availabilty - if there are no cars available for a particular car or if there are less cars available than the amount requested.
            carAvailable = checkAvailability(carBrandIndex);

        }

    } while (carAvailable == false);

    do {
        // Ask how many cars the user would like to purchase... and retrieve response...
        amountReq = retrieveShortInput("How many cars would you like to Purchase.\nNumber: ");

        //input validation to check sufficiency
        carAvailable = checkSufficiency(carBrandIndex, amountReq);

    } while (!carAvailable);

    //ask the users age... and retrieve response...
    userAge = retrieveShortInput("How old are you?\nAge: ");

    //Determine weather or not the user recieves a discount.
    discount = giveDiscount(userAge);

    if (discount) {
        //print congratulations message
        printf("\nCongratulations! You qualify for and have received a %d percent discount.\n\n", 30);

        //calculate the total cost of the transaction to the user
        finaleCost = implementDiscount(carBrandIndex) * amountReq;

        strcpy(discountString, "True");
    } else {
        printf("You do not qualify for, and hence, have not received a discount.\n\n");

        //calculate the total cost of the transaction to the user
        finaleCost = carBrandsArr[carBrandIndex].carPrice * amountReq;

        strcpy(discountString, "False");
    }

    //display message... processing purchase (press enter to continue)
    printf("processing purchase...\n");

    //update the total amount of cars remaining
    updateAmountAvailable(carBrandIndex, amountReq);

    //update the total amount of cars available
    updateTotalCarsAvailable();

    printf("\n\n...press enter to continue");
    getchar();

    //display thank you message
    printf("\nThank you for supporting our fundraiser\n\n");

    //print transaction stats
    printf("You have suceccfully purchased %hd %s(s) at a total cost of %.2f GBP.\n", amountReq,
           carBrandsArr[carBrandIndex].carName, finaleCost);

    // print message have a nice day
    printf("\nThank you for supporting our Fundraiser\n"
           "We have %d more cars available for purchase.\n\n", totalAvailableCars);

//populate salesArr of Structures
    populateSalesArr(transactionIndex, customerName, carBrandIndex, userAge, amountReq, discountString, finaleCost);


    transactionIndex++; // counter for the number of sales/transactions.

//Captures User Feedback
    time_t current = time(NULL); //current moment.
    struct tm *now = localtime(&current);
    strcpy(feedback[feedbackCounter].CustomerName, salesArr[transactionIndex - 1].name);
    collectFeedback();
    strcpy(feedback[feedbackCounter].Time, asctime(now));
    feedbackCounter++;
    //return to the menu page.

}

void viewingSalesStats() {
    //  clearScreen();

    printf("All sales data:");

    //display the sales/Transactions made
    displaySalesStats();

}

void displayCustomerFeedback() {
    for (int i = 0; i < feedbackCounter; i++) {
        puts("_______________________________________________________________________________________");
        printf("\nCustomer Name:\t%s", feedback[i].CustomerName);
        printf("\nRating:\t\t%hd", feedback[i].happinessRating);
        printf("\nFeedback:\t%s", feedback[i].customerFeedback);
        printf("\nDate&Time:\t%s\n", feedback[i].Time);
        puts("_______________________________________________________________________________________");
    }
}


/*End of main menu functions*/

int main() {

    readSalesDataFromFile(FILER);

    /* Variablea */
    char menuChoice;
    //Program Begins

    // Clear the screen
    clearScreen();

    //greet the customer
    greeting();

    do {
        //Print Menu
        printMenu();
        //prompt user to select an action & collect user input
        menuChoice = tolower(retrieveInput("Please enter an action (a, b, c, d or x):\nEnter: "));

        switch (menuChoice) {
            case VIEW_CARS:
                sortCars();
                displayCarStats();
                break;
            case BUY_CAR:
                //Purchase Car
                buyingCars();
                printf("\n...press enter to return to the menu");
                getchar();
                break;
            case VIEW_SALES:
                viewingSalesStats();
                break;
            case CUSTOMER_FEEDBACK:
                displayCustomerFeedback();
                break;
            case EXIT:
                initiateExitProcedure();
                break;
            default:
                printf("You have entered an invalid input. Please enter only either a, b, c, d or x\n\n");
                break;
        }

    } while (menuChoice != EXIT);

    return 0;
}
