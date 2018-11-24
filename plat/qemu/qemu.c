#include "all.h"

/* ---------------------- moontu list start ---------------------- */

#define mt_offset_of(type, member) ((int)&(((type *)0)->member))

#define mt_container_of(ptr, type, member) ({ \
		const typeof(((type *)0)->member) *__ptr = (ptr);\
		(type *)((char *)__ptr - mt_offset_of(type, member));})

struct mt_list_node {
	struct mt_list_node *next;
	struct mt_list_node *prev;
};
typedef struct mt_list_node mt_list_t;

static inline void mt_list_init(mt_list_t *l) {
	l->next = l;
	l->prev = l;
}

// 在节点l后，插入节点n
static inline void mt_list_insert_after(mt_list_t *l, mt_list_t *n) {
	l->next->prev = n;
	n->next = l->next;

	l->next = n;
	n->prev = l;
}

// 在节点l前，插入节点n
static inline void mt_list_insert_before(mt_list_t *l, mt_list_t *n) {
	l->prev->next = n;
	n->prev = l->prev;

	l->prev = n;
	n->next = l;
}

// 移除节点n
static inline void mt_list_remove(mt_list_t *n) {
	n->next->prev = n->prev;
	n->prev->next = n->next;

	n->next = n;
	n->prev = n;
}

// 得到用户结构体指针
#define mt_list_entry(node, type, member) \
	mt_container_of(node, type, member)

// 向前遍历链表
#define mt_list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

// 向后遍历链表
#define mt_list_for_each_prev(pos, head) \
	for (pos = (head)->prev; pos != (head); pos = pos->prev)

// 遍历链表安全移除...
#define mt_list_for_each_safe(pos, n, head)

// 遍历特定类型
#define mt_list_for_each_entry()

// 

// 得到链表的第一个成员
#define mt_list_first_entry(ptr, type, member)

// 得到链表最后一个成员
#define mt_list_last_entry(ptr, type, member)

/* ----------------------- moontu list end ----------------------- */

// 学生信息
struct stu {
	char *name;
	unsigned int num;
	mt_list_t l;
};

static struct stu *__create() {}
static void __destroy() {}

struct stu *stu_reg() {}
void stu_unreg() {}
void stu_find() {}
void stu_print() {}

void main(void) {
	int i;
	int sum = 0;
	struct stu a;

	for (i = 1; i <= 100; i++) {
		sum += i;
	}
	//printf("1 + 2 + ... + 100 = %d\n", sum);
	//printf("hello world!\n");

	int offset = mt_offset_of(struct stu, name);
	//printf("offset: %d\n", offset);
	offset = mt_offset_of(struct stu, num);
	//printf("offset: %d\n", offset);
	struct stu *p = mt_container_of(&(a.num), struct stu, num);
	//printf("%x\n", p);

}

