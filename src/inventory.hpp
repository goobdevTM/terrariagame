extern bool inventory_open;

extern int selected_slot;
extern int selected_block_type;

extern int hotbar[10]; 
extern int hotbar_count[10]; 
extern int inventory[40];
extern int inventory_count[40]; 

extern void open_inventory();

extern void close_inventory();

extern void draw_inventory();
extern void draw_hotbar();

extern void init_hotbar_and_inventory();

extern void reduce_count_in_slot(int slot, int reduce_by);

extern void add_item_to_inventory(int id, int amount);

extern void fix_problems();