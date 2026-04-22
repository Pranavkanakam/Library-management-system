# Library Management System

This project is an interactive Library Management System written in C. It is designed to handle common library operations such as adding books, registering users, borrowing and returning books, managing requests, tracking fines, and undoing the most recent change. Some demo datasets are already added.

## How to Compile

```bash
make
```

## How to Run

```bash
./proj
```

## Project Overview

The system runs in the terminal and provides a simple numbered menu for different library operations. It stores data using linked lists and saves that data into text files so the information is available again when the program is reopened.

The project manages four main types of records:

- `Book`
- `User`
- `Transaction`
- `Request`

## Features

- View a user's issued books, return history, payable fine, and current overdue amount
- Pay outstanding fines
- Add users
- Add books
- Delete books
- Search books by title
- Borrow books
- Return books
- Request unavailable books
- View pending book requests
- View users
- View books
- View transactions
- Track fines for late returns
- Show current overdue fines for books that are still not returned
- Undo last operation
- Save and load data from files
- Protect admin actions with a password
- Verify the admin password using secure `crypt()`-based hashing

## How the System Works

When the program starts, it loads data from the text files into linked lists. The user then chooses operations from the menu.

- A new user can be registered and receives a system-generated user ID.
- An admin can add new books or increase the copies of an existing book.
- An admin can delete a book entirely or reduce its available copies.
- A user can borrow a book if copies are available.
- When a book is returned, the system checks how many days it was kept.
- If the book is returned after 30 days, a fine of Rs 5 per extra day is added.
- Users can pay fines partially or fully from the menu.
- If a book is not available, the user can place a request for it.
- The latest data-changing action can be undone before another change is made.
- Admin authentication is checked against the hashed password stored in `Admin.txt`.
- All records can be saved back into text files.

## Data Structures Used

The project uses linked lists to store data dynamically during execution.

- `Book` stores book ID, title, total copies, and available copies.
- `User` stores user ID, name, and fine amount.
- The system also calculates each user's live overdue amount from currently borrowed books.
- `Transaction` stores transaction ID, user ID, book ID, issue time, return time, return status, and fine.
- `Request` stores user ID and requested book title.

Linked lists were chosen because the number of books, users, and transactions can grow during runtime without requiring a fixed size in advance.

## File Storage

The project uses text files for persistence.

- `Books.txt` stores book data
- `Users.txt` stores user data
- `Transactions.txt` stores borrowing and return history
- `Requests.txt` stores book requests
- `Admin.txt` stores the hashed admin password


## File Description

- `main.c` - contains the main menu and program flow
- `lib.c` - contains the implementation of library functions
- `lib.h` - contains structure, function declarations
- `Books.txt` - saved book records
- `Users.txt` - saved user records
- `Transactions.txt` - saved transaction records
- `Requests.txt` - saved request records
- `Admin.txt` - hashed admin password file
- `makefile` - build instructions
