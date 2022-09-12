#ifndef YMIRDB_H
#define YMIRDB_H

#define MAX_KEY 16
#define MAX_LINE 1024

#include <stddef.h>
#include <sys/types.h>

enum item_type {
    INTEGER=0,
    ENTRY=1
};

typedef struct element element;
typedef struct entry entry;
typedef struct snapshot snapshot;

struct element {
  enum item_type type;
  union {
    int value;
    struct entry *entry;
  }item_value;
};

struct entry {
  char key[MAX_KEY];
  char is_simple; // 0 general entry, 1 simple entry
  element * values;
  size_t length;

  entry* next;
  entry* prev;
  
  size_t forward_size; 
  size_t forward_max; 
  entry** forward;  // this entry depends on these
    
  size_t backward_size; 
  size_t backward_max; 
  entry** backward; // these entries depend on this
};

struct snapshot {
  int id;
  entry** entries; 
  int entry_num;
  snapshot* next;
  snapshot* prev;
};


const char* HELP =
	"BYE   clear database and exit\n"
	"HELP  display this help message\n"
	"\n"
	"LIST KEYS       displays all keys in current state\n"
	"LIST ENTRIES    displays all entries in current state\n"
	"LIST SNAPSHOTS  displays all snapshots in the database\n"
	"\n"
	"GET <key>    displays entry values\n"
	"DEL <key>    deletes entry from current state\n"
	"PURGE <key>  deletes entry from current state and snapshots\n"
	"\n"
	"SET <key> <value ...>     sets entry values\n"
	"PUSH <key> <value ...>    pushes values to the front\n"
	"APPEND <key> <value ...>  appends values to the back\n"
	"\n"
	"PICK <key> <index>   displays value at index\n"
	"PLUCK <key> <index>  displays and removes value at index\n"
	"POP <key>            displays and removes the front value\n"
	"\n"
	"DROP <id>      deletes snapshot\n"
	"ROLLBACK <id>  restores to snapshot and deletes newer snapshots\n"
	"CHECKOUT <id>  replaces current state with a copy of snapshot\n"
	"SNAPSHOT       saves the current state as a snapshot\n"
	"\n"
	"MIN <key>  displays minimum value\n"
	"MAX <key>  displays maximum value\n"
	"SUM <key>  displays sum of values\n"
	"LEN <key>  displays number of values\n"
	"\n"
	"REV <key>   reverses order of values (simple entry only)\n"
	"UNIQ <key>  removes repeated adjacent values (simple entry only)\n"
	"SORT <key>  sorts values in ascending order (simple entry only)\n"
	"\n"
	"FORWARD <key> lists all the forward references of this key\n"
	"BACKWARD <key> lists all the backward references of this key\n"
	"TYPE <key> displays if the entry of this key is simple or general\n";

	void command_help();

	void command_bye();

	entry * command_set(char * line, entry** entries, int entry_num);

	void command_del(char * key, entry ** entries, int entry_num);

	void command_get(entry** entries, int entry_num, char * key);

	void command_list_keys(entry** entries, int entry_num);

	void command_list_entries(entry** entries, int entry_num);

	void command_list_snapshots(snapshot** snapshots, int snapshot_num);

	void command_pop(entry** entries, int entry_num, char * key);

	void command_pick(entry** entries, int entry_num, char * key, int index);

	void command_push(entry** entries, int entry_num, char * key, int* values, int values_length);

	void command_append(entry** entries, int entry_num, char * key, int* values, int values_length);

	void command_pluck(entry** entries, int entry_num, char * key, int index);

	void command_purge(char * key, entry ** entries, int entry_num);

	void command_sort(entry** entries, int entry_num, char * key);

	int int_cmp(const void *a, const void *b);

	void command_unique(entry** entries, int entry_num, char * key);

	snapshot * command_snapshot(entry** entries, int entry_num, snapshot** snapshots, int snapshot_num);

	void command_checkout(snapshot** snapshots, int snapshot_num, int checkout_num);




#endif
