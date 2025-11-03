#include <iostream>
#include <iomanip>      // For setw
#include <string>
#include <vector>       // <-- NEW: For dynamic order arrays
#include <map>          // <-- NEW: To store the menu (replaces array)
#include <limits>       // <-- NEW: For input validation

using namespace std;

// --- Member 1 Refactor: Upgraded Structures ---

// Structure to store menu item details
// (Code is now the 'key' in the map, not part of the struct)
struct MenuItem {
    string name;
    float price;
};

// Structure to store order details
// (Uses vectors for dynamic sizing)
struct Order {
    string customerName;
    vector<int> itemCodes;   // <-- NEW: Replaced int itemCode[10]
    vector<int> quantities; // <-- NEW: Replaced int quantity[10]
    float totalBill;        // <-- NEW: Added to store the final bill amount
};

// --- Global Variable: Order History (Used by Member 2's features) ---
vector<Order> orderHistory; // <-- NEW: Stores all completed orders for this session

// --- Utility Function: For Safe Numeric Input (Member 1 / 2) ---
int getNumericInput() {
    int choice;
    while (!(cin >> choice)) {
        cout << "  [!] Invalid input. Please enter a number: ";
        cin.clear(); // Clear the error flag on cin
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard bad input
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard any extra characters
    return choice;
}

// --- Member 1 Refactor: Core Functions ---

// Function to display the menu (now iterates over a map)
void displayMenu(map<int, MenuItem>& menu) {
    cout << "\n=========== CANTEEN MENU ===========" << endl;
    cout << left << setw(10) << "Code" << setw(20) << "Item" << setw(10) << "Price" << endl;
    cout << "------------------------------------" << endl;

    // <-- NEW: Modern C++ way to loop over a map
    for (auto const& [code, item] : menu) {
        cout << left << setw(10) << code << setw(20) << item.name
             << "Rs. " << item.price << endl;
    }
    cout << "------------------------------------" << endl;
}

// Function to generate and print bill (now uses map)
// <-- NEW: It now also takes 'orderHistory' to log the order
void generateBill(Order& order, map<int, MenuItem>& menu) {
    float total = 0;
    cout << "\n============= BILL RECEIPT =============" << endl;
    cout << "Customer Name: " << order.customerName << endl;
    cout << "----------------------------------------" << endl;
    cout << left << setw(20) << "Item" << setw(10) << "Qty" << setw(10) << "Amount" << endl;
    cout << "----------------------------------------" << endl;

    // <-- NEW: Loops over vector, not a fixed array
    for (int i = 0; i < order.itemCodes.size(); i++) {
        int code = order.itemCodes[i];

        // <-- NEW: Efficiently checks if item exists using .count()
        if (menu.count(code)) {
            MenuItem& item = menu[code]; // Get item directly from map
            float amount = item.price * order.quantities[i];
            cout << left << setw(20) << item.name << setw(10) << order.quantities[i]
                 << "Rs. " << amount << endl;
            total += amount;
        } else {
            cout << "  [!] Invalid Item Code: " << code << " (Ignored)" << endl;
        }
    }

    float gst = total * 0.05;
    float grandTotal = total + gst;
    order.totalBill = grandTotal; // <-- NEW: Save total to the order object

    cout << "----------------------------------------" << endl;
    cout << "Total Amount: Rs. " << total << endl;
    cout << "GST (5%):     Rs. " << gst << endl;
    cout << "----------------------------------------" << endl;
    cout << "Grand Total:  Rs. " << grandTotal << endl;
    cout << "========================================" << endl;
    cout << "Thank You! Visit Again.\n" << endl;

    // <-- NEW: (Member 2's feature) Log this order to the history
    orderHistory.push_back(order);
}

// --- Member 2 Feature: Customer Order Function ---
void placeOrder(map<int, MenuItem>& menu) {
    Order newOrder;
    cout << "\nEnter Customer Name: ";
    getline(cin, newOrder.customerName);

    cout << "Enter number of *different* items: ";
    int itemCount = getNumericInput();

    for (int i = 0; i < itemCount; i++) {
        cout << "\n--- Item " << (i + 1) << " ---" << endl;
        cout << "  Enter Item Code: ";
        int code = getNumericInput();

        // <-- NEW: Check if code is valid before asking for quantity
        if (menu.count(code) == 0) {
            cout << "  [!] Item code " << code << " does not exist. Skipping this item." << endl;
            i--; // Retry this item number
            continue;
        }

        cout << "  Enter Quantity: ";
        int qty = getNumericInput();

        if (qty <= 0) {
             cout << "  [!] Quantity must be at least 1. Skipping this item." << endl;
             i--; // Retry this item number
             continue;
        }

        newOrder.itemCodes.push_back(code);
        newOrder.quantities.push_back(qty);
    }

    if (newOrder.itemCodes.size() > 0) {
        generateBill(newOrder, menu);
    } else {
        cout << "\nOrder cancelled as no valid items were added." << endl;
    }
}


// --- Member 2 Feature: Admin Mode Functions ---

void addMenuItem(map<int, MenuItem>& menu) {
    cout << "\n--- Add New Menu Item ---" << endl;
    cout << "Enter new Item Code: ";
    int code = getNumericInput();

    if (menu.count(code)) {
        cout << "  [!] Item code " << code << " already exists!" << endl;
        return;
    }

    cout << "Enter Item Name: ";
    string name;
    getline(cin, name);

    cout << "Enter Item Price: Rs. ";
    float price;
    while (!(cin >> price) || price < 0) {
        cout << "  [!] Invalid price. Enter a positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    menu[code] = {name, price};
    cout << "  [*] Item '" << name << "' added successfully!" << endl;
}

void removeMenuItem(map<int, MenuItem>& menu) {
    cout << "\n--- Remove Menu Item ---" << endl;
    displayMenu(menu);
    cout << "Enter Item Code to remove: ";
    int code = getNumericInput();

    if (menu.count(code)) {
        string name = menu[code].name;
        menu.erase(code); // <-- NEW: Simple map erase
        cout << "  [*] Item '" << name << "' removed successfully!" << endl;
    } else {
        cout << "  [!] Item code " << code << " not found." << endl;
    }
}

void updateItemPrice(map<int, MenuItem>& menu) {
     cout << "\n--- Update Item Price ---" << endl;
    displayMenu(menu);
    cout << "Enter Item Code to update: ";
    int code = getNumericInput();

    if (menu.count(code)) {
        cout << "  Current Price for '" << menu[code].name << "' is Rs. " << menu[code].price << endl;
        cout << "  Enter new Price: Rs. ";
        float price;
        while (!(cin >> price) || price < 0) {
            cout << "  [!] Invalid price. Enter a positive number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        menu[code].price = price;
        cout << "  [*] Price updated successfully!" << endl;
    } else {
        cout << "  [!] Item code " << code << " not found." << endl;
    }
}

void viewOrderHistory(map<int, MenuItem>& menu) {
    cout << "\n--- Session Order History ---" << endl;
    if (orderHistory.empty()) {
        cout << "  No orders placed in this session yet." << endl;
        return;
    }

    float totalSales = 0;
    for (int i = 0; i < orderHistory.size(); ++i) {
        cout << "----------------------------------------" << endl;
        cout << "Order #" << (i + 1) << " | Customer: " << orderHistory[i].customerName << endl;
        cout << "  Items:" << endl;
        for (int j = 0; j < orderHistory[i].itemCodes.size(); ++j) {
            int code = orderHistory[i].itemCodes[j];
            string name = menu.count(code) ? menu[code].name : "DELETED ITEM";
            cout << "    - " << left << setw(20) << name
                 << " (Qty: " << orderHistory[i].quantities[j] << ")" << endl;
        }
        cout << "  Total Bill: Rs. " << orderHistory[i].totalBill << endl;
        totalSales += orderHistory[i].totalBill;
    }
    cout << "========================================" << endl;
    cout << "  Total Session Sales: Rs. " << totalSales << endl;
    cout << "========================================" << endl;
}

// Main Admin Menu loop
void adminMenu(map<int, MenuItem>& menu) {
    cout << "\n--- Admin Login ---" << endl;
    cout << "Enter Admin Password: ";
    string pass;
    getline(cin, pass);

    if (pass != "admin123") { // Simple hardcoded password
        cout << "  [!] Incorrect Password. Returning to main menu." << endl;
        return;
    }

    cout << "\nWelcome, Admin!" << endl;
    
    while (true) {
        cout << "\n======= ADMIN MENU =======" << endl;
        cout << "  1. Add Menu Item" << endl;
        cout << "  2. Remove Menu Item" << endl;
        cout << "  3. Update Item Price" << endl;
        cout << "  4. View Session Order History" << endl;
        cout << "  5. View Customer Menu" << endl;
        cout << "  6. Return to Main Menu" << endl;
        cout << "--------------------------" << endl;
        cout << "Enter your choice: ";
        int choice = getNumericInput();

        if (choice == 1) {
            addMenuItem(menu);
        } else if (choice == 2) {
            removeMenuItem(menu);
        } else if (choice == 3) {
            updateItemPrice(menu);
        } else if (choice == 4) {
            viewOrderHistory(menu);
        } else if (choice == 5) {
             displayMenu(menu);
        } else if (choice == 6) {
            cout << "  Returning to Main Menu..." << endl;
            break; // Exit Admin loop
        } else {
            cout << "  [!] Invalid choice. Try again." << endl;
        }
    }
}


// --- Main Function ---

int main() {
    // --- Member 1 Refactor: Menu initialized as a map ---
    map<int, MenuItem> menu;
    menu[101] = {"Burger🍔", 50};
    menu[102] = {"Pizza🍕", 120};
    menu[103] = {"Cold Drink🥤", 40};
    menu[104] = {"Sandwich🥪", 60};
    menu[static_cast<int>(105)] = {"Pasta🍝", 80}; // Using static_cast for clarity, though not required
    menu[106] = {"Coffee☕️", 30};

    cout << "\n===== Welcome to Canteen Management System =====" << endl;

    while (true) {
        cout << "\n========= MAIN MENU =========" << endl;
        cout << "  1. Show Menu" << endl;
        cout << "  2. Place Order" << endl;
        cout << "  3. Admin Mode" << endl; // <-- NEW
        cout << "  4. Exit" << endl;       // <-- NEW
        cout << "---------------------------" << endl;
        cout << "Enter your choice: ";
        
        int choice = getNumericInput(); // <-- NEW: Using safe input function

        if (choice == 1) {
            displayMenu(menu);
        } else if (choice == 2) {
            placeOrder(menu); // <-- NEW: Function to handle all order logic
        } else if (choice == 3) {
            adminMenu(menu); // <-- NEW: (Member 2's feature)
        } else if (choice == 4) {
            cout << "\nThank you for using Canteen Management System!\n";
            break;
        } else {
            cout << "  [!] Invalid choice. Try again." << endl;
        }
    }
    return 0;
}