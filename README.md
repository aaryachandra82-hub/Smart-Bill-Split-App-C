# 💰 Smart Bill Splitter (Expense Splitter in C)

A simple **C program** that helps groups of friends easily **split expenses**, **track balances**, and **settle debts** with minimal transactions.

---

## 🧠 Project Overview
This project implements a **command-line Smart Bill Splitter** — a tool designed to fairly divide shared expenses among multiple friends.  
It automatically calculates who owes whom and by how much, making expense management simple and accurate.

---

## ⚙️ Features
✅ Add and list friends dynamically  
✅ Record shared expenses (split equally)  
✅ Automatically calculate each person’s balance  
✅ Suggest minimal settlement transactions using a **greedy algorithm**  
✅ Dynamic memory allocation (no fixed friend limit)  
✅ Clean, menu-driven interface  

---

## 🧩 Data Structures Used

**1. `struct Friend`** – stores each friend's name and net balance  
**2. `struct FriendList`** – dynamic array of `Friend` structures  
**3. Dynamic memory allocation** via `malloc()` and `realloc()` for scalability  

---

## 🔍 Core Functions
| Function | Purpose |
|-----------|----------|
| `init_list()` | Initializes the friend list and allocates memory |
| `ensure_capacity()` | Expands memory when friend list grows |
| `add_friend()` | Adds a new friend to the list |
| `add_expense()` | Records an expense and splits the amount equally |
| `show_balances()` | Displays each person’s net balance |
| `settle_debts()` | Uses a greedy algorithm to minimize settlements |
| `prompt_menu()` | Displays interactive CLI menu |

---

## 💻 How to Compile & Run

### **Compile:**
```bash
gcc -std=c11 -O2 -Wall DS_miniproject.c -o DS_miniproject


