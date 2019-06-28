#include "all.h"

typedef unsigned int mt_size_t;

/* ----------------------- moontu sys API ------------------------ */
void mt_malloc(mt_size_t size) {

}

void mt_free(void *ptr) {

}

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

// 测试是否为空
static inline int mt_list_isempty(mt_list_t *l) {
	return l->next == l;
}
// 得到用户结构体指针
#define mt_list_entry(node, object, member) \
	mt_container_of(node, object, member)

// 向前遍历链表
#define mt_list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

// 向后遍历链表
#define mt_list_for_each_prev(pos, head) \
	for (pos = (head)->prev; pos != (head); pos = pos->prev)

// 得到链表的第一个成员
#define mt_list_first_entry(ptr, object, member) \
	mt_list_entry((ptr)->next, object, member)

// 得到链表最后一个成员
#define mt_list_last_entry(ptr, object, member) \
	mt_list_entry((ptr)->prev, object, member)

/* ----------------------- moontu list end ----------------------- */

struct object;
typedef void (*ds_t)(struct object *pobject);

struct object {
	ds_t display_human;
	mt_list_t l;
};

struct student {
	char *name;
	int num;
	struct object stu;
};

struct teacher {
	// todo: 后面要把char* 改为数组
	char *name;
	struct object tea;
};

struct worker {
	char *name;
	struct object wok;
};

void sch_tea_display(struct object *pobject);
void sch_stu_display(struct object *pobject);
void sch_wok_display(struct object *pobject);

struct teacher t_a;
struct teacher t_b;

struct student s_a;
struct student s_b;

struct worker w_a;

mt_list_t head;

// 不要用malloc和free，用几个全局变量就行了
void sch_tea_init(struct teacher *ptea, char *name) {
	// 这是个潜藏的bug，""的地址是在read only的，全局变量
	// 如果是局部变量，就会崩溃了
	// todo: 后面要用memcpy处理
	ptea->name = name;

	//ptea->tea.h_object = 1;
	ptea->tea.display_human = sch_tea_display;
	mt_list_init(&ptea->tea.l);
}

// 将当前老师链到list上去
void sch_tea_insert(mt_list_t *h, struct teacher *ptea) {
	mt_list_insert_after(h, &ptea->tea.l);
}

void sch_stu_init(struct student *pstu, char *name, int num) {
	pstu->name = name;
	pstu->num = num;
	pstu->stu.display_human = sch_stu_display;
	mt_list_init(&pstu->stu.l);
}

// 将当前学生链到list上去
void sch_stu_insert(mt_list_t *h, struct student *pstu) {
	mt_list_insert_after(h, &pstu->stu.l);
}

void sch_wok_init(struct worker *pwok, char *name) {
	pwok->name = name;
	pwok->wok.display_human = sch_wok_display;
	mt_list_init(&pwok->wok.l);
}

void sch_wok_insert(mt_list_t *h, struct worker *pwok) {
	mt_list_insert_after(h, &pwok->wok.l);
}

// 把所有老师学生都打出来
// 老师只打印名字，学生打名字和学号
// teacher: tuzi
// student: never, num: 1
// 这是框架函数，不需要被外部调用
static void sch_display_human(mt_list_t *h) {
	struct object *pobject;
	mt_list_t *ptmp;

	mt_list_for_each(ptmp, h) {
		pobject = mt_list_entry(ptmp, struct object, l);
		// 这是bug，初始化里定义display_human，但从头到尾没有使用，而是用了传参的方法调用专用打印函数
		//display_human(pobject);
		// 这才叫使用定义了的函数指针
		pobject->display_human(pobject);
	}
}

void sch_tea_display(struct object *pobject) {
	struct teacher *p;

	p = mt_list_entry(pobject, struct teacher, tea);
	printf("teacher: %s\n", p->name);
}

void sch_stu_display(struct object *pobject) {
	struct student *p;

	p = mt_list_entry(pobject, struct student, stu);
	printf("student: %s, num: %d\n", p->name, p->num);
}

void sch_wok_display(struct object *pobject) {
	struct worker *p;

	p = mt_list_entry(pobject, struct worker, wok);
	printf("worker: %s\n", p->name);
}

void main(void) {
	int i;
	int sum = 0;

	for (i = 1; i <= 100; i++) {
		sum += i;
	}
	printf("1 + 2 + ... + 100 = %d\n", sum);
	printf("hello world!\n");

	mt_list_init(&head);
	sch_tea_init(&t_a, "aaa");
	sch_tea_init(&t_b, "bbb");

	sch_tea_insert(&head, &t_a);
	sch_tea_insert(&head, &t_b);

	sch_stu_init(&s_a, "tuzi", 1);
	sch_stu_init(&s_b, "never", 2);

	sch_stu_insert(&head, &s_a);
	sch_stu_insert(&head, &s_b);

	sch_wok_init(&w_a, "zhangyi");
	sch_wok_insert(&head, &w_a);

	sch_display_human(&head);
}
