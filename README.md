# Library Management System

This project is a interactive Library Management System written in C. It is designed to handle common library operations such as adding books, registering users, borrowing and returning books, managing requests, and tracking fines.

## Project Overview

The system runs in the terminal and provides a simple numbered menu for different library operations. It stores data using linked lists and saves that data into text files so the information is available again when the program is reopened.

The project manages four main types of records:

- `Book`
- `User`
- `Transaction`
- `Request`

## Features

- Add users
- Add books
- Delete books
- Search books by title
- Borrow books
- Return books
- Request unavailable books
- View users
- View books
- View transactions
- Track fines for late returns
- Save and load data from files
- Protect admin actions with a password

## How the System Works

When the program starts, it loads data from the text files into linked lists. The user then chooses operations from the menu.

- A new user can be registered and receives a system-generated user ID.
- An admin can add new books or increase the copies of an existing book.
- A user can borrow a book if copies are available.
- When a book is returned, the system checks how many days it was kept.
- If the book is returned after 30 days, a fine of Rs 5 per extra day is added.
- If a book is not available, the user can place a request for it.
- All records can be saved back into text files.

## Data Structures Used

The project uses linked lists to store data dynamically during execution.

- `Book` stores book ID, title, total copies, and available copies.
- `User` stores user ID, name, and fine amount.
- `Transaction` stores transaction ID, user ID, book ID, issue time, return time, return status, and fine.
- `Request` stores user ID and requested book title.

Linked lists were chosen because the number of books, users, and transactions can grow during runtime without requiring a fixed size in advance.

## File Storage

The project uses text files for persistence.

- `Books.txt` stores book data
- `Users.txt` stores user data
- `Transactions.txt` stores borrowing and return history
- `Requests.txt` stores book requests
- `Admin.txt` stores the admin password


## File Description

- `main.c` - contains the main menu and program flow
- `lib.c` - contains the implementation of library functions
- `lib.h` - contains structure, function declarations
- `Books.txt` - saved book records
- `Users.txt` - saved user records
- `Transactions.txt` - saved transaction records
- `Requests.txt` - saved request records
- `Admin.txt` - admin password file
- `makefile` - build instructions

## How to Compile

```bash
make
```

## How to Run

```bash
./proj
```
