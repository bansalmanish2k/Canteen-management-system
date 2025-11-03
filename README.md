Canteen Management System (C++)
📋 Project Overview

The Canteen Management System is a simple, menu-driven C++ console application designed to manage a canteen’s daily operations.
It allows customers to view the menu, place orders, and receive an automated bill.
It also provides an admin interface for managing menu items, updating prices, and viewing total sales history during a session.

🧩 Features
👤 Customer Features

View the canteen menu with item codes and prices

Place orders for multiple items dynamically

Automatic bill generation with GST calculation

Input validation to prevent invalid entries

🔐 Admin Features

Password-protected admin access (admin123)

Add, remove, or update menu items

View full session order history and total sales

Display the current customer menu

⚙️ Technologies Used

Language: C++

Libraries:

<iostream> – Input/Output operations

<iomanip> – Output formatting

<map> – For dynamic menu storage

<vector> – For handling multiple orders dynamically

<limits> – For safe input validation

<string> – For string handling

🚀 How to Run the Program

Clone or Download the repository from GitHub.

Open the file canteen_management.cpp in any C++ IDE or text editor.

Compile the program using a C++ compiler (e.g., g++):

g++ canteen_management.cpp -o canteen


Run the executable:

./canteen


Follow the on-screen menu to use Customer Mode or Admin Mode.

🔑 Admin Credentials

Username: (not required)

Password: admin123

💾 Example Menu
Code	Item	Price (Rs.)
101	Burger 🍔	50
102	Pizza 🍕	120
103	Cold Drink 🥤	40
104	Sandwich 🥪	60
105	Pasta 🍝	80
106	Coffee ☕️	30
🧠 Concepts Demonstrated

Object-Oriented Programming (Structures)

STL Containers (map, vector)

Input validation using exception handling techniques

Modular and reusable function design

🧑‍💻 Contributors

Member 1: Menu refactoring, input validation, and data structures

Member 2: Admin mode, billing system, and order history features
