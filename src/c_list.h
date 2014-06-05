#ifndef C_LIST_H_
#define C_LIST_H_

/* Node, List, and Iterator are the only data structures used currently. */

/*
 * ����ڵ�
 */
typedef struct listNode {

    // ǰ��ڵ�
    struct listNode *prev;

    // ��̽ڵ�
    struct listNode *next;

    // ֵ
    void *value;

} listNode;

/*
 * ��������
 */
typedef struct listIter {

    // ��һ�ڵ�
    listNode *next;

    // �����
    int direction;

} listIter;

/*
 * ����
 */
typedef struct list {

    // ��ͷָ��
    listNode *head;

    // ��βָ��
    listNode *tail;

    // �ڵ�����
    unsigned long len;

    // ���ƺ���
    void *(*dup)(void *ptr);
    // �ͷź���
    void (*free)(void *ptr);
    // �ȶԺ���
    int (*match)(void *ptr, void *key);
} list;

/* Functions implemented as macros */
// ��������Ľڵ�����
#define listLength(l) ((l)->len)
// ��������ı�ͷ�ڵ�
#define listFirst(l) ((l)->head)
// ��������ı�β�ڵ�
#define listLast(l) ((l)->tail)
// ���ظ�ڵ��ǰһ���ڵ�
#define listPrevNode(n) ((n)->prev)
// ���ظ�ڵ�ĺ�һ���ڵ�
#define listNextNode(n) ((n)->next)
// ���ظ�ڵ��ֵ
#define listNodeValue(n) ((n)->value)

#define listSetDupMethod(l,m) ((l)->dup = (m))
#define listSetFreeMethod(l,m) ((l)->free = (m))
#define listSetMatchMethod(l,m) ((l)->match = (m))

#define listGetDupMethod(l) ((l)->dup)
#define listGetFree(l) ((l)->free)
#define listGetMatchMethod(l) ((l)->match)

/* Prototypes */
list *listCreate(void);
void listRelease(list *list);
list *listAddNodeHead(list *list, void *value);
list *listAddNodeTail(list *list, void *value);
list *listInsertNode(list *list, listNode *old_node, void *value, int after);
void listDelNode(list *list, listNode *node);
listIter *listGetIterator(list *list, int direction);
listNode *listNext(listIter *iter);
void listReleaseIterator(listIter *iter);
list *listDup(list *orig);
listNode *listSearchKey(list *list, void *key);
listNode *listIndex(list *list, long index);
void listRewind(list *list, listIter *li);
void listRewindTail(list *list, listIter *li);
void listRotate(list *list);

/* Directions for iterators */
#define AL_START_HEAD 0
#define AL_START_TAIL 1

#endif /* C_LIST_H_ */
