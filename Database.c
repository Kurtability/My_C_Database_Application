/**
 * comp2017 - assignment 2
 * <your name> Kurt Lu
 * <your unikey> falu5003
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <ctype.h>
#include "ymirdb.h"

//bye
void command_bye() {
	printf("bye\n");
}

//help
void command_help() {
	printf("%s\n", HELP);
}

//set
entry * command_set(char * line, entry** entries, int entry_num){
    entry * new_entry = malloc(sizeof(entry)); // set as a pointer so i can use ->
    new_entry->is_simple = 0;
    new_entry->length = 0;
    new_entry->values = NULL;

    char * tok = strtok(line, " ");  // tick out 'set'

	// tok is now the entry key
    tok = strtok(NULL, " ");
    strcpy(new_entry->key, tok);  // update the entry key

    // tok is now the first entry element
    tok = strtok(NULL, " ");
    while (tok != NULL)  // process values, !malloc new element!
    {
        element entry_element;

        for (int i = 0; i < strlen(tok); i++) {

			// if any element is not a number, it means general entry
            if (!isdigit(tok[i])) {
                new_entry->is_simple = 0; // general entry
            }else{
				new_entry->is_simple = 1; // simple entry
			}
        }
        if (new_entry->is_simple){  
            entry_element.type = 0; // simple entry
            entry_element.item_value.value = atoi(tok); // use atoi to convert to interger
                new_entry->length++;
                new_entry->values = realloc(new_entry->values, sizeof(element)*new_entry->length);
			    // update the new_entry values 
                memcpy(new_entry->values + (new_entry->length - 1), &entry_element, sizeof(element));
        }
        else{
            new_entry->is_simple = 0;// general entry
            entry_element.type = 1;
            entry_element.item_value.entry = NULL;
			//find the corresponded entry pointer from entries

            memcpy(new_entry->values + (new_entry->length - 1), &entry_element, sizeof(element));
        }

		// get the next entry value
        tok = strtok(NULL, " ");
    }
    
    return new_entry;
}

// del, delete entry from current state
void command_del(char * key, entry ** entries, int entry_num){

    for (int i = 0; i < entry_num; i++){
        if (strcmp(entries[i]->key, key) == 0){
            memmove(entries + i, entries + i + 1, sizeof(entry *) * (entry_num - 1 - i));
            printf("ok\n");
        }
        
    }
    // key doesn't exist is handled in main()

}

// PURGE, delete entry from current state and snapshot 
void command_purge(char * key, entry ** entries, int entry_num){
    for (int i = 0; i < entry_num; i++){
        if (strcmp(entries[i]->key, key) == 0){
            memmove(entries + i, entries + i + 1, sizeof(entry *) * (entry_num - 1 - i));
            // printf("ok\n");
        }
        
    }
    // not sure if I need to handle key doesnt exist yet
}


// get
void command_get(entry ** entries, int entry_num, char * key){

	bool key_exists = 0;

    for (int i = 0; i < entry_num; i++){
        if (strcmp(entries[i]->key, key) == 0){

            if (entries[i]->length >= 1){
                printf("[");
                for (int j = 0; j < entries[i]->length -1; j++){
                    printf("%d ", entries[i]->values[j].item_value.value);
                }

                printf("%d]\n", entries[i]->values[entries[i]->length - 1].item_value.value);
            }
            else if (entries[i]->length == 0){
                printf("[]\n");
            }
			key_exists = 1;
        }
    }

	if (!key_exists){
		printf("no such key\n");
	}

    // general entry, when key dosen't exist

}

// list keys
void command_list_keys(entry** entries, int entry_num){
    if (entry_num == 0){
        printf("no keys\n");
    }
    for (int i = entry_num -1; i >= 0; i--){
        printf("%s\n", entries[i]->key);
    }
}

// list entries
void command_list_entries(entry** entries, int entry_num){
    if (entry_num == 0){
        printf("no entries\n");
    }

    for (int i = entry_num -1; i >= 0; i--){

        if (entries[i]->length > 0){
            printf("%s [", entries[i]->key);
            for (int j = 0; j < entries[i]->length -1; j++){
                    printf("%d ", entries[i]->values[j].item_value.value);
            }
            printf("%d]\n", entries[i]->values[entries[i]->length - 1].item_value.value);
        }
        else if (entries[i]->length == 0){
            printf("%s []\n", entries[i]->key);
        }
    }
}

//list snapshots
void command_list_snapshots(snapshot** snapshots, int snapshot_num){
    if (snapshot_num == 0){
        printf("no snapshots\n");
    }
}


// pop
void command_pop(entry** entries, int entry_num, char * key){
    bool key_exists = 0;

    for (int i = 0; i < entry_num; i++){
        if (strcmp(entries[i]->key, key) == 0){

            if (entries[i]->length > 1){
                printf("%d\n",entries[i]->values[0].item_value.value);

                memmove(entries[i]->values, entries[i]->values + 1, sizeof(element) * (entries[i]->length - 1));

                // entries[i]->length gotta -1 after popping the first value to avoid segmentation fault
                entries[i]->length --;

            }
            else if (entries[i]->length == 1){

                printf("%d\n",entries[i]->values[0].item_value.value);

                entries[i]->length --;
                // entry length is now 0, printing [] is handled in 'set'
            }
            else if (entries[i]->length == 0){
                printf("nil\n");
            }
            key_exists = 1; 
        }
    }

    if (key_exists == 0){
        printf("no such key\n");
    }

}

// PICK
void command_pick(entry** entries, int entry_num, char * key, int index){
    bool key_exists = 0;
    for (int i = 0; i < entry_num; i++){
        if (strcmp(entries[i]->key, key) == 0){
            key_exists = 1;

            if (index >0 && index < entries[i]->length + 1){
                printf("%d\n", entries[i]->values[index-1].item_value.value);
            }else{
                printf("index out of range\n");
            }
        }
    }

    if (key_exists == 0){
        printf("no such key\n");
    }
}

// pluck 
void command_pluck(entry** entries, int entry_num, char * key, int index){
    bool key_exists = 0;
    for (int i = 0; i < entry_num; i++){
        if (strcmp(entries[i]->key, key) == 0){
            key_exists = 1;

            if (index >0 && index < entries[i]->length + 1){
                printf("%d\n", entries[i]->values[index-1].item_value.value);

                memmove(entries[i]->values + index - 1, entries[i]->values + index, (entries[i]->length - index)  *sizeof(element));

                entries[i]->length --;

            }else{
                printf("index out of range\n");
            }
        }
    }

    if (key_exists == 0){
        printf("no such key\n");
    }
}

// push
void command_push(entry** entries, int entry_num, char* key, int* values, int values_length){

    element temp_element;

    for (int i = 0; i < entry_num; i++){
        if (strcmp(entries[i]->key, key) == 0){

            if (values_length > 0 && values_length < 2){ // when pushing only 1 element
                // alr know its a simple entry 
                temp_element.type = 0;
                temp_element.item_value.value = values[0];

                entries[i]->length ++;
                entries[i]->values = realloc(entries[i]->values, sizeof(element)*entries[i]->length);

                memmove(entries[i]->values + 1, entries[i]->values, sizeof(element) * (entries[i]->length - 1));

                memcpy(entries[i]->values, &temp_element, sizeof(element));
                
                printf("ok\n");
            }
            else if(values_length > 1){ // when pushing more than 1 element 

                entries[i]->length = entries[i]->length + values_length;
                entries[i]->values = realloc(entries[i]->values, sizeof(element)*entries[i]->length);

                memmove(entries[i]->values + values_length, entries[i]->values, sizeof(element) * (entries[i]->length - values_length));

                // values_length > original entry length
                if (values_length > entries[i]->length - values_length){
                    for (int j = entries[i]->length - (entries[i]->length - values_length) - 1; j >=0 ; j--){
                    temp_element.type = 0;
                    temp_element.item_value.value = values[(entries[i]->length - (entries[i]->length - values_length))  - j - 1];

                    memcpy(entries[i]->values + j, &temp_element, sizeof(element));
                    }
                }
                // values_length == original entry length
                else if(values_length == entries[i]->length - values_length){
                    for (int j = entries[i]->length - values_length - 1; j >=0 ; j--){
                    temp_element.type = 0;
                    temp_element.item_value.value = values[(entries[i]->length - values_length) - j - 1];

                    memcpy(entries[i]->values + j, &temp_element, sizeof(element));
                    }
                }
                // values_length < original entry length
                else{
                    for (int j = entries[i]->length - (entries[i]->length - values_length) - 1; j >=0 ; j--){
                    temp_element.type = 0;
                    temp_element.item_value.value = values[(entries[i]->length - (entries[i]->length - values_length))  - j - 1];

                    memcpy(entries[i]->values + j, &temp_element, sizeof(element));
                    }
                }
                
                printf("ok\n");
            }
        }
    }
// key doesnt exist is handled in main()
}

void command_append(entry** entries, int entry_num, char * key, int* values, int values_length){
    element temp_element;
    for (int i = 0; i < entry_num; i++){
        if (strcmp(entries[i]->key, key) == 0){
            
            for ( int j = 0; j < values_length; j++){
                // alr know its a simple entry 
                temp_element.type = 0;
                temp_element.item_value.value = values[j];

                entries[i]->length ++;
                entries[i]->values = realloc(entries[i]->values, sizeof(element)*entries[i]->length);
                memcpy(entries[i]->values + entries[i]->length - 1, &temp_element, sizeof(element));
            }
            printf("ok\n");
        }
    }
}


// sort
// reference: https://www.includehelp.com/c-programs/sort-integer-array-using-qsort-with-a-function-pointer.aspx
int int_cmp(const void *a, const void *b) { 
    const int *ia = (const int *)a; // casting pointer types 
    const int *ib = (const int *)b;
    return *ia  - *ib; 
	//integer comparison: returns negative if b > a and positive if a > b 
} 
void command_sort(entry** entries, int entry_num, char * key){
    bool key_exists = 0;

    for (int i = 0; i < entry_num; i++){
        if (strcmp(entries[i]->key, key) == 0){
            int *arr; 
            arr = (int*)malloc(entries[i]->length * sizeof(int));

            for (int j = 0; j < entries[i]->length; j++){
                arr[j] = entries[i]->values[j].item_value.value;
            }

            qsort(arr, entries[i]->length, sizeof(int), int_cmp);

            for (int j = 0; j < entries[i]->length; j++){
                entries[i]->values[j].item_value.value = arr[j];
            }

            printf("ok\n");
            free(arr);
			key_exists = 1;
        }
    }

	if (!key_exists){
		printf("no such key\n");
	}
}


// unique
void command_unique(entry** entries, int entry_num, char * key){
    bool key_exists = 0;

    for (int i = 0; i < entry_num; i++){
        if (strcmp(entries[i]->key, key) == 0){
            int *arr; 
            arr = (int*)malloc(entries[i]->length * sizeof(int));

            for (int j = 0; j < entries[i]->length; j++){
                arr[j] = entries[i]->values[j].item_value.value;
            }
            
            int count_to_minus = 0;
            for(int j = 1; j < entries[i]->length; j++){
                if ( arr[j] == arr[j-1] ){

                    count_to_minus ++;
                    if( arr[j + 1] != arr[j]){
                        memmove(arr + j - count_to_minus, arr + j, sizeof(int) * (entries[i]->length - j));
                        entries[i]->length = entries[i]->length - count_to_minus;
                        count_to_minus = 0;
                    }
                    else{ // the case of just 1 1 1 
                        entries[i]->length = 1;
                    }
                }
            }

            for (int j = 0; j < entries[i]->length; j++){
                entries[i]->values[j].item_value.value = arr[j];
            }

            printf("ok\n");

            free(arr);
			key_exists = 1;
        }
    }
	if (!key_exists){
		printf("no such key\n");
	}
}

// snapshot
snapshot * command_snapshot(entry ** entries, int entry_num, snapshot ** snapshots, int snapshot_num){

    snapshot * snapshot = malloc(sizeof(snapshot));

    if (snapshot_num > 0){

        snapshot->prev = snapshots[snapshot_num - 1];
        snapshots[snapshot_num - 1]->next = snapshot;

    }
    // else{

    //     snapshot->prev = NULL;
    // }

    snapshot->id = snapshot_num++;
    // snapshot->entries = malloc(sizeof(entry *) * entry_num);
    // memcpy(snapshot->entries, entries, sizeof(entry *) * entry_num);
    // snapshot->entry_num = entry_num;

    // snapshot->next = NULL;

    return snapshot;
}

// CHECKOUT
void command_checkout(snapshot** snapshots, int snapshot_num, int checkout_num){
    printf("you are trying to checkout snapshot%d\n",checkout_num);
}


int main(void) {

	char line[MAX_LINE];

	entry ** entries = NULL;
	int entry_num = 0;
    snapshot ** snapshots = NULL;
	int snapshot_num = 0;
    int checkout_num = 0;

	while (true) {
		printf("> ");

		fgets(line, MAX_LINE, stdin);
        line[strlen(line)-1] = '\0'; // change 'return' to null byte

		if (strncasecmp(line, "HELP", 4) == 0){
			command_help();
		}

		if (strncasecmp(line, "BYE", 3) == 0){
			command_bye();
			break;
        }

		if (strncasecmp(line, "SET", 3) == 0){
            entry * new_entry = command_set(line, entries, entry_num);
			printf("ok\n");
			printf("\n");

            entry_num++;
            entries = realloc(entries, sizeof(entry *) * entry_num);
            entries[entry_num - 1] = new_entry;

            //if the new_entry_key alr exists, move the memory of the new entry to the old
            char * new_entry_key = new_entry -> key;
            for (int i = 0; i < entry_num - 1; i++){
                if (strcmp(new_entry_key, entries[i]->key) == 0){

                    memmove(entries[i], entries[entry_num-1],sizeof(entry));
                    entry_num --;
                }
            }
        }

        if (strncasecmp(line, "del", 3) == 0){
            strtok(line, " "); // tick out command
            char * key = strtok(NULL, " ");
            
            bool key_exists = 0;

            for (int i = 0; i < entry_num; i++){
                if (strcmp(entries[i]->key, key) == 0){
                    key_exists = 1;
                    command_del(key, entries, entry_num);
                    entry_num--;
                }
            }
            if (key_exists == 0){
                printf("no such key\n");
            }

			printf("\n");
        }

        if (strncasecmp(line, "purge", 5) == 0){
            strtok(line, " "); // tick out command
            char * key = strtok(NULL, " ");
            

            for (int i = 0; i < entry_num; i++){
                if (strcmp(entries[i]->key, key) == 0){
                    command_purge(key, entries, entry_num);
                    entry_num--;
                }
            }
            printf("ok\n");
			printf("\n");
        }

		if (strncasecmp(line, "get", 3) == 0){
            if (checkout_num == 0){
                strtok(line, " "); // tick out command
                char * key = strtok(NULL, " ");
                command_get(entries, entry_num, key);
			    printf("\n");
            }
            
        }

		if (strncasecmp(line, "list keys", 9) == 0){
			// printf("listing keys\n");
            command_list_keys(entries, entry_num);
            printf("\n");
		}

		if (strncasecmp(line, "list entries", 12) == 0){
			// printf("listing entries\n");
            command_list_entries(entries, entry_num);
            printf("\n");
		}

        if (strncasecmp(line, "list snapshots", 14) == 0){
            command_list_snapshots(snapshots, snapshot_num);
            printf("\n");
		}


        if (strncasecmp(line, "pop", 3) == 0){
            strtok(line, " "); // tick out command
            char * key = strtok(NULL, " ");
            command_pop(entries, entry_num, key);
			printf("\n");
        }

        if (strncasecmp(line, "pick", 4) == 0){
            strtok(line, " "); // tick out command
            char * key = strtok(NULL, " ");
            int index = atoi(strtok(NULL," "));
            command_pick(entries, entry_num, key, index);

			printf("\n");
        }

        if (strncasecmp(line, "pluck", 5) == 0){
            strtok(line, " "); // tick out command
            char * key = strtok(NULL, " ");
            int index = atoi(strtok(NULL," "));
            command_pluck(entries, entry_num, key, index);

			printf("\n");
        }

        if (strncasecmp(line, "push", 4) == 0){
            strtok(line, " "); // tick out command, 'line' now prints out 'push'
            char * key = strtok(NULL, " ");

            bool key_exists = 0;
            for (int i = 0; i < entry_num; i++){
                if (strcmp(entries[i]->key, key) == 0){
                    key_exists = 1;
                }
            }
            if (key_exists == 0){
                printf("no such key\n");
            }


            int push_entry_length = 0;
            int * arr = NULL;

            while ( key != NULL){
                char * token = strtok(NULL, " ");
                if (token == NULL){
                    break;
                }
                push_entry_length ++;
                arr = (int*)realloc(arr, push_entry_length * sizeof(int));
                arr[push_entry_length - 1] = atoi(token);
            }

            command_push(entries, entry_num, key, arr, push_entry_length);
            free(arr);

			printf("\n");
        }

        if (strncasecmp(line, "append", 6) == 0){
            strtok(line, " "); // tick out command, 'line' now prints out 'push'
            char * key = strtok(NULL, " ");

            bool key_exists = 0;
            for (int i = 0; i < entry_num; i++){
                if (strcmp(entries[i]->key, key) == 0){
                    key_exists = 1;
                }
            }
            if (key_exists == 0){
                printf("no such key\n");
            }


            int push_entry_length = 0;
            int * arr = NULL;

            while ( key != NULL){
                char * token = strtok(NULL, " ");
                if (token == NULL){
                    break;
                }
                push_entry_length ++;
                arr = (int*)realloc(arr, push_entry_length * sizeof(int));
                arr[push_entry_length - 1] = atoi(token);
            }

            command_append(entries, entry_num, key, arr, push_entry_length);
            free(arr);

			printf("\n");
        }

        if (strncasecmp(line, "sort", 4) == 0){
            strtok(line, " "); // tick out command
            char * key = strtok(NULL, " ");
            command_sort(entries, entry_num, key);
			printf("\n");
        }

        if (strncasecmp(line, "uniq", 4) == 0){
            strtok(line, " "); // tick out command
            char * key = strtok(NULL, " ");
            command_unique(entries, entry_num, key);
			printf("\n");
        }

        if (strncasecmp(line, "snapshot", 8) == 0){
            snapshot * new_snapshot = command_snapshot(entries, entry_num, snapshots, snapshot_num);
            snapshot_num++;
            
            snapshots = realloc(snapshots, sizeof(snapshot *) * snapshot_num);
            snapshots[snapshot_num - 1] = new_snapshot;
            printf("saved as snapshot %d\n", snapshot_num);
        }

        if (strncasecmp(line, "checkout", 8) == 0){
            strtok(line, " "); // tick out command
            int checkout_num = atoi(strtok(NULL, " "));

            command_checkout(snapshots, snapshot_num, checkout_num);
			printf("\n");
        }

  	}

    free(snapshots);
    free(entries);
	return 0;
}
