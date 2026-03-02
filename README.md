# CS300
Reflection
What problem was I solving?

In these projects, I developed an advising assistance program for the Computer Science department at ABC University (ABCU). The goal was to design and implement a system that could efficiently load course data from a file, store it using appropriate data structures, and allow advisors to retrieve course information and prerequisites. The primary challenge was selecting and evaluating the most appropriate data structure to support fast lookups while maintaining the ability to print courses in sorted, alphanumeric order.

How did I approach the problem?

I approached the problem by first analyzing the data requirements and access patterns. Since advisors needed fast course lookups by course number, I evaluated three data structures: a vector, a hash table, and a binary search tree. Understanding data structures was critical because each structure has different runtime trade-offs for insertion, search, and sorting.

--A vector required sorting before printing and linear search for lookups.

--A binary search tree provided ordered traversal but could degrade in performance.

--A hash table offered constant-time average lookup performance, which made it ideal for retrieving course information quickly.

After performing Big-O analysis, I selected a hash table for implementation because it best met the advisor’s needs for fast lookups while still allowing sorting when necessary.

How did I overcome roadblocks?

Throughout the project, I encountered challenges such as validating file input, handling duplicate course entries, ensuring prerequisite integrity, and resolving compilation and indexing issues in the development environment. I overcame these by debugging incrementally, validating input thoroughly, testing edge cases, and carefully tracing logic errors. Breaking the problem into smaller, testable components allowed me to isolate issues and resolve them systematically.

How has this project expanded my approach to software design?

This project strengthened my understanding of algorithm efficiency and reinforced the importance of choosing the correct data structure before writing code. Instead of focusing solely on functionality, I considered scalability, runtime complexity, and memory trade-offs. I now approach problems by analyzing constraints and performance requirements first, rather than immediately implementing a solution.

How has this project evolved the way I write programs?

Working on these projects improved the way I structure programs for maintainability and clarity. I focused on:

--Separating concerns into clearly defined functions

--Using meaningful variable and function names

--Validating user input and handling errors gracefully

--Writing modular code that can be extended or modified easily

I now prioritize readability, structure, and defensive programming practices. This ensures that my programs are adaptable, easier to debug, and maintainable over time.
