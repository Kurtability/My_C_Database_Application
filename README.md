# My_C_Database_Application

## Comands
BYE
HELP

LIST KEYS
LIST ENTRIES
LIST SNAPSHOTS

GET <KEY>
DEL <KEY>
PURGE <KEY>
  

SET <KEY>
PUSH <KEY>
APPEND <KEY>
  
PICK <KEY>
PLUCK <KEY>
POP <KEY>
  
-



- Simple entry (strictly integer values)

The simple entry is a key value store, where only integers are used as values. The following defines and initialises an entry with key identifier a with values:
- > SET a 1 2 3 
- ok
- > GET a [1 2 3]

The values can be updated with other commands.
- > SET a 1 2 3 
- ok
- > PUSH a 5 
- ok
- > GET a 
- [5 1 2 3]
- > APPEND a 7 
- ok
- > GET a
- [5 1 2 3 7]
- > SET a 9 8 7 
- ok
- > GET a 
- [9 8 7]

## An entry is termed simple if it’s values are only integers. It is otherwise general and described later.

- General entry (mixed integer and key values)

## A general entry contains at least one value that is a key to another entry in the database.
- > SET a 1 2 
- ok
- > SET b 4 a 6 
- ok
- > LIST ENTRIES 
- B[4a6] A[12]
- > SUM a 
- 3
- > SUM b 
- 13
