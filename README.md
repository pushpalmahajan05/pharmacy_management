# Pharmacy Management System

## Overview
This project is a Pharmacy Management System implemented in C. It provides essential functionalities to manage a pharmacy, including reading and managing medicine data, utilizing trie data structures for efficient searching, and handling input/output operations. The project is organized with modular C files and a `Makefile` for compilation.

## Features
- Medicine database management
- Efficient search using Trie data structures
- Command-line interface for managing pharmacy operations
- Modular code structure for better maintainability

## Installation
### Prerequisites
- A C compiler (e.g., GCC)
- Make utility

### Steps to Install
1. Clone or download the repository.
2. Navigate to the extracted folder:
    cd final
  
3. Compile the project using `make`:
    make
  
5. Run the executable:
  ./pharmacy_mgmt
   

## File Structure

final/
│── main.c          # Entry point of the program
│── utils.c         # Utility functions
│── io.c            # Handles input and output
│── trie.c          # Trie data structure implementation
│── read_bat.c      # Reads batch-related information
│── read_med.c      # Reads medicine-related data
│── include.h       # Header file for declarations
│── trie.h          # Header file for Trie functions
│── Makefile        # Compilation instructions


## Usage
- The system will prompt users for medicine-related operations.
- Users can search for medicines efficiently using the trie-based search functionality.
- Medicine data is read from files and managed dynamically.

## Contributing
Feel free to contribute by submitting issues or pull requests. Ensure your code is well-documented and adheres to best practices.


