#include <defs.h>
#include <x86.h>
#include <stdio.h>
#include <string.h>
#include <swap.h>
#include <swap_fifo.h>
#include <list.h>
list_entry_t pra_list_head_LRU;
static int
_LRU_init_mm(struct mm_struct *mm)
{     
     list_init(&pra_list_head_LRU);
     mm->sm_priv = &pra_list_head_LRU;
     //cprintf(" mm->sm_priv %x in fifo_init_mm\n",mm->sm_priv);
     return 0;
}
static int
_LRU_map_swappable(struct mm_struct *mm, uintptr_t addr, struct Page *page, int swap_in)
{
    list_entry_t *head=(list_entry_t*) mm->sm_priv;
    list_entry_t *entry=&(page->pra_page_link);
 
    assert(entry != NULL && head != NULL);
    //record the page access situlation
    /*LAB3 EXERCISE 2: YOUR CODE*/ 
    //(1)link the most recent arrival page at the back of the pra_list_head qeueue.
    //head是当前正在维护的队列，entry是现在要添加到维护队列中的一个链表对象，
    list_entry_t *le=head;
    while ((le = list_next(le)) != head){
        if(le==entry){
            list_del(le);
            break;
        }
    }
    list_add(head, entry);
    return 0;
}
static int
_LRU_swap_out_victim(struct mm_struct *mm, struct Page ** ptr_page, int in_tick)
{
     list_entry_t *head=(list_entry_t*) mm->sm_priv;
         assert(head != NULL);
     assert(in_tick==0);
     /* Select the victim */
     /*LAB3 EXERCISE 2: YOUR CODE*/ 
     //(1)  unlink the  earliest arrival page in front of pra_list_head qeueue
     //(2)  assign the value of *ptr_page to the addr of this page
     list_entry_t *le = head->prev;
     assert(head!=le);
     struct Page *p = le2page(le, pra_page_link);
     list_del(le);
     assert(p !=NULL);
     *ptr_page = p;
     return 0;
}
static int
_LRU_init(void)
{
    return 0;
}
static int
_LRU_set_unswappable(struct mm_struct *mm, uintptr_t addr)
{
    return 0;
}
static int
_LRU_tick_event(struct mm_struct *mm)
{ return 0; }
static int
_LRU_check_swap(void) {
    cprintf("write Virt Page c in fifo_check_swap\n");
    *(unsigned char *)0x3000 = 0x0c;
    //assert(pgfault_num==4);
    cprintf("write Virt Page a in fifo_check_swap\n");
    *(unsigned char *)0x1000 = 0x0a;
    //assert(pgfault_num==4);
    cprintf("write Virt Page d in fifo_check_swap\n");
    *(unsigned char *)0x4000 = 0x0d;
    //assert(pgfault_num==4);
    cprintf("write Virt Page b in fifo_check_swap\n");
    *(unsigned char *)0x2000 = 0x0b;
    //assert(pgfault_num==4);
    cprintf("write Virt Page e in fifo_check_swap\n");
    *(unsigned char *)0x5000 = 0x0e;
    //assert(pgfault_num==5);
    cprintf("write Virt Page b in fifo_check_swap\n");
    *(unsigned char *)0x2000 = 0x0b;
    //assert(pgfault_num==5);
    cprintf("write Virt Page a in fifo_check_swap\n");
    *(unsigned char *)0x1000 = 0x0a;
    //assert(pgfault_num==6);
    cprintf("write Virt Page b in fifo_check_swap\n");
    *(unsigned char *)0x2000 = 0x0b;
    //assert(pgfault_num==7);
    cprintf("write Virt Page c in fifo_check_swap\n");
    *(unsigned char *)0x3000 = 0x0c;
    //assert(pgfault_num==8);
    cprintf("write Virt Page d in fifo_check_swap\n");
    *(unsigned char *)0x4000 = 0x0d;
    //assert(pgfault_num==9);
    cprintf("write Virt Page e in fifo_check_swap\n");
    *(unsigned char *)0x5000 = 0x0e;
    //assert(pgfault_num==10);
    cprintf("write Virt Page a in fifo_check_swap\n");
    //assert(*(unsigned char *)0x1000 == 0x0a);
    *(unsigned char *)0x1000 = 0x0a;
    //assert(pgfault_num==11);
    return 0;
}
struct swap_manager swap_manager_LRU =
{
     .name            = "LRU swap manager",
     .init            = &_LRU_init,
     .init_mm         = &_LRU_init_mm,
     .tick_event      = &_LRU_tick_event,
     .map_swappable   = &_LRU_map_swappable,
     .set_unswappable = &_LRU_set_unswappable,
     .swap_out_victim = &_LRU_swap_out_victim,
     .check_swap      = &_LRU_check_swap,
};