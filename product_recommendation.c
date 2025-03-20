#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> // For fmax function


sqlite3 *db;
int rc;

#define MAX_PRODUCTS 100
#define MAX_BUDGET 1000

./main()
Product products[MAX_PRODUCTS];
double dp[MAX_PRODUCTS][MAX_BUDGET];


typedef struct {
    int id;
    char name[50];
    double price;
    double satisfaction;
    int rating;
    double shipping_cost;
    int in_stock;
} Product;

// Function to open the database
void openDatabase() {
    rc = sqlite3_open("products.db", &db);
    if (rc != SQLITE_OK) {
        printf("Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
    } else {
        printf("Opened database successfully.\n");
    }
}


void createProductsTable() {
    char *sql = "CREATE TABLE IF NOT EXISTS Products ("
                "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                "Name TEXT NOT NULL, "
                "Price REAL, "
                "Satisfaction REAL, "
                "Rating INTEGER, "
                "ShippingCost REAL, "
                "InStock INTEGER);";
    
    char *err_msg = 0;
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        printf("SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        printf("Table created successfully.\n");
    }
}


void insertProduct(char *name, double price, double satisfaction, int rating, double shipping_cost, int in_stock) {
    char sql[512];
    sprintf(sql, "INSERT INTO Products (Name, Price, Satisfaction, Rating, ShippingCost, InStock) "
                 "VALUES ('%s', %.2f, %.2f, %d, %.2f, %d);", name, price, satisfaction, rating, shipping_cost, in_stock);

    char *err_msg = 0;
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        printf("Error inserting product: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        return1;
    }
}

void displayProducts() {
    sqlite3_stmt *stmt;
    char *sql = "SELECT * FROM Products;";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Failed to fetch products: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("ID\tName\tPrice\tSatisfaction\tRating\tShippingCost\tInStock\n");
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        printf("%d\t%s\t%.2f\t%.2f\t%d\t%.2f\t%d\n",
               sqlite3_column_int(stmt, 0),
               sqlite3_column_text(stmt, 1),
               sqlite3_column_double(stmt, 2),
               sqlite3_column_double(stmt, 3),
               sqlite3_column_int(stmt, 4),
               sqlite3_column_double(stmt, 5),
               sqlite3_column_int(stmt, 6));
    }
    sqlite3_finalize(stmt);
}

// Function to find optimal products based on budget
void findOptimalProducts(int budget) {
    int n = 0;  // Number of products

    // Fetch products from the database and fill the products array
    sqlite3_stmt *stmt;
    char *sql = "SELECT * FROM Products WHERE InStock = 1;"; // Only consider products that are in stock

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        printf("Failed to fetch products: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Fill the products array
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        products[n].id = sqlite3_column_int(stmt, 0);
        strcpy(products[n].name, (char *)sqlite3_column_text(stmt, 1));
        products[n].price = sqlite3_column_double(stmt, 2);
        products[n].satisfaction = sqlite3_column_double(stmt, 3);
        products[n].rating = sqlite3_column_int(stmt, 4);
        products[n].shipping_cost = sqlite3_column_double(stmt, 5);
        products[n].in_stock = sqlite3_column_int(stmt, 6);
        n++;
    }
    sqlite3_finalize(stmt);

    // Implement the DP algorithm (0/1 Knapsack)
    for (int i = 0; i <= budget; i++) {
        dp[0][i] = 0; // Initialize the first row
    }

    for (int i = 1; i <= n; i++) {
        for (int b = 0; b <= budget; b++) {
            if (products[i - 1].price <= b) {
                dp[i][b] = fmax(dp[i - 1][b], dp[i - 1][b - (int)products[i - 1].price] + products[i - 1].satisfaction);
            } else {
                dp[i][b] = dp[i - 1][b];
            }
        }
    }

    // Find selected products
    printf("Maximum Satisfaction: %.2f\n", dp[n][budget]);
    printf("Recommended products based on your budget:\n");
    
    int b = budget;
    for (int i = n; i > 0 && b > 0; i--) {
        if (dp[i][b] != dp[i - 1][b]) { // Product was included
            printf("%s, Price: %.2f, Satisfaction: %.2f\n", products[i - 1].name, products[i - 1].price, products[i - 1].satisfaction);
            b -= (int)products[i - 1].price; // Reduce the budget
        }
    }
}

int main(){

openDatabase();
createProductsTable();


insertProduct("Product1", 50.0, 4.5, 5, 5.0, 1);
insertProduct("Product2", 100.0, 3.8, 3, 10.0, 0);
insertProduct("Product3", 10.0, 3, 2, 5.50, 1);
insertProduct("Product4", 45.0, 4, 4, 5.0, 0);
insertProduct("Product5", 70.0, 3.3, 4, 10.0, 1);
insertProduct("Product6", 25.0, 2.8, 3, 5.50, 1);
insertProduct("Product7", 99.0, 4, 4, 10.0, 0);
insertProduct("Product8", 50.0, 3, 3, 5.0, 1);
insertProduct("Product9", 60.0, 2.7, 3, 5.0, 0);
insertProduct("Product10", 30.0, 4.9, 5, 5.50, 0);


displayProducts() 

int budget;
printf("Enter your Budget: ");
scanf("%d", &budget);

findOptimalProducts(budget);

sqlite3_close(db);
return 0;

}
