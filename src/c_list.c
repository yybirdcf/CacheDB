#include <stdlib.h>
#include <jemalloc/jemalloc.h>

#include "c_list.h"

/*
 * ����һ�����б�
 *
 * �����ɹ�ʱ�����б?����ʧ�ܷ��� NULL
 *
 * T = O(1)
 */
list *listCreate(void)
{
    struct list *list;

    // Ϊ�б�ṹ�����ڴ�
    if ((list = malloc(sizeof(*list))) == NULL)
        return NULL;

    // ��ʼ������
    list->head = list->tail = NULL;
    list->len = 0;
    list->dup = NULL;
    list->free = NULL;
    list->match = NULL;

    return list;
}

/*
 * �ͷ�����б�(�Լ��б��Ľڵ�)
 *
 * T = O(N)��N Ϊ�б�ĳ���
 */
void listRelease(list *list)
{
    unsigned long len;
    listNode *current, *next;

    current = list->head;
    len = list->len;
    while(len--) {
        next = current->next;
        // ����б����Դ�� free ��������ô�ȶԽڵ�ֵ������
        if (list->free) list->free(current->value);
        // ֮�����ͷŽڵ�
        free(current);
        current = next;
    }
    free(list);
}

/*
 * �½�һ����� value �Ľڵ㣬��������뵽�б�ı�ͷ
 *
 * ����ʱ������ NULL ����ִ�ж�����
 * �ɹ�ʱ�����ش�����б�
 *
 * T = O(1)
 */
list *listAddNodeHead(list *list, void *value)
{
    listNode *node;

    if ((node = malloc(sizeof(*node))) == NULL)
        return NULL;

    node->value = value;

    if (list->len == 0) {
        // ��һ���ڵ�
        list->head = list->tail = node;
        node->prev = node->next = NULL;
    } else {
        // ���ǵ�һ���ڵ�
        node->prev = NULL;
        node->next = list->head;
        list->head->prev = node;
        list->head = node;
    }

    list->len++;

    return list;
}

/*
 * �½�һ����� value �Ľڵ㣬��������뵽�б�ı�β
 *
 * ����ʱ������ NULL ����ִ�ж�����
 * �ɹ�ʱ�����ش�����б�
 *
 * T = O(1)
 */
list *listAddNodeTail(list *list, void *value)
{
    listNode *node;

    if ((node = malloc(sizeof(*node))) == NULL)
        return NULL;

    node->value = value;

    if (list->len == 0) {
        // ��һ���ڵ�
        list->head = list->tail = node;
        node->prev = node->next = NULL;
    } else {
        // ���ǵ�һ���ڵ�
        node->prev = list->tail;
        node->next = NULL;
        list->tail->next = node;
        list->tail = node;
    }

    list->len++;

    return list;
}

/*
 * ����һ����ֵ value �Ľڵ�
 * ����� after �����ָʾ�����½ڵ���뵽 old_node ��֮ǰ����֮��
 *
 * T = O(1)
 */
list *listInsertNode(list *list, listNode *old_node, void *value, int after) {
    listNode *node;

    if ((node = malloc(sizeof(*node))) == NULL)
        return NULL;

    node->value = value;

    if (after) {
        // ���뵽 old_node ֮��
        node->prev = old_node;
        node->next = old_node->next;
        // �����β�ڵ�
        if (list->tail == old_node) {
            list->tail = node;
        }
    } else {
        // ���뵽 old_node ֮ǰ
        node->next = old_node;
        node->prev = old_node->prev;
        // �����ͷ�ڵ�
        if (list->head == old_node) {
            list->head = node;
        }
    }

    // ����ǰ�ýڵ�ͺ�̽ڵ��ָ��
    if (node->prev != NULL) {
        node->prev->next = node;
    }
    if (node->next != NULL) {
        node->next->prev = node;
    }

    // �����б�ڵ�����
    list->len++;

    return list;
}

/*
 * �ͷ��б��и�Ľڵ�
 * ���ڵ�˽��ֵ(private value)�Ĺ����ɵ��������
 *
 * T = O(1)
 */
void listDelNode(list *list, listNode *node)
{
    // ����ǰ��ڵ��ָ��
    if (node->prev)
        node->prev->next = node->next;
    else
        list->head = node->next;

    // �����̽ڵ��ָ��
    if (node->next)
        node->next->prev = node->prev;
    else
        list->tail = node->prev;

    // �ͷŽڵ�ֵ
    if (list->free) list->free(node->value);

    // �ͷŽڵ�
    free(node);

    // �����б�ڵ�����
    list->len--;
}

/*
* �����б� list ��һ���������������ɲ��� direction ����
*
* ÿ�ζԵ�������� listNext() ��������ͷ����б����һ���ڵ�
*
* ���������ʧ������
*
* T = O(1)
*/
listIter *listGetIterator(list *list, int direction)
{
    listIter *iter;

    if ((iter = malloc(sizeof(*iter))) == NULL) return NULL;

    // ��ݵ��ķ��򣬽��������ָ��ָ���ͷ���߱�β
    if (direction == AL_START_HEAD)
        iter->next = list->head;
    else
        iter->next = list->tail;

    // ��¼����
    iter->direction = direction;

    return iter;
}

/*
 * �ͷŵ���� iter
 *
 * T = O(1)
 */
void listReleaseIterator(listIter *iter) {
    free(iter);
}

/*
 * ������� iter �ĵ��ָ�뵹�� list �ı�ͷ
 *
 * T = O(1)
 */
void listRewind(list *list, listIter *li) {
    li->next = list->head;
    li->direction = AL_START_HEAD;
}

/*
 * ������� iter �ĵ��ָ�뵹�� list �ı�β
 *
 * T = O(1)
 */
void listRewindTail(list *list, listIter *li) {
    li->next = list->tail;
    li->direction = AL_START_TAIL;
}

/*
 * ���ص�����ĵ�ǰ�ڵ�
 *
 * ����ʹ�� listDelNode() ɾ��ǰ�ڵ㣬���ǲ�����ɾ������ڵ㡣
 *
 * ����Ҫô���ص�ǰ�ڵ㣬Ҫô���� NULL ����ˣ�������÷��ǣ�
 *
 * iter = listGetIterator(list,<direction>);
 * while ((node = listNext(iter)) != NULL) {
 *     doSomethingWith(listNodeValue(node));
 * }
 *
 * T = O(1)
 */
listNode *listNext(listIter *iter)
{
    listNode *current = iter->next;

    if (current != NULL) {
        // ��ݵ����ѡ��ڵ�
        if (iter->direction == AL_START_HEAD)
            iter->next = current->next;
        else
            iter->next = current->prev;
    }

    return current;
}

/*
 * ��������б?�ɹ������б�ĸ������ڴ治���ʧ��ʱ���� NULL ��
 *
 * ���۸����ǳɹ���ʧ�ܣ������б?���ᱻ�޸ġ�
 *
 * T = O(N)��N Ϊ orig �б�ĳ���
 */
list *listDup(list *orig)
{
    list *copy;
    listIter *iter;
    listNode *node;

    if ((copy = listCreate()) == NULL)
        return NULL;

    // ��������
    copy->dup = orig->dup;
    copy->free = orig->free;
    copy->match = orig->match;

    // ���ƽڵ�
    iter = listGetIterator(orig, AL_START_HEAD);
    while((node = listNext(iter)) != NULL) {

        // ���ƽڵ�ֵ
        void *value;

        if (copy->dup) {
            value = copy->dup(node->value);
            if (value == NULL) {
                listRelease(copy);
                listReleaseIterator(iter);
                return NULL;
            }
        } else
            value = node->value;

        // ���½ڵ���ӵ����б�ĩβ
        if (listAddNodeTail(copy, value) == NULL) {
            listRelease(copy);
            listReleaseIterator(iter);
            return NULL;
        }
    }

    listReleaseIterator(iter);

    return copy;
}

/*
 * ���б��в��Һ� key ƥ��Ľڵ㡣
 *
 * ����б����ƥ��������ôƥ��ͨ��ƥ���������С�
 * ����б�û��ƥ��������ôֱ�ӽ� key �ͽڵ��ֵ���бȶԡ�
 *
 * ƥ��ӱ�ͷ��ʼ����һ��ƥ��ɹ��Ľڵ�ᱻ����
 * ���ƥ�䲻�ɹ������� NULL ��
 *
 * T = O(N)��N Ϊ�б�ĳ���
 */
listNode *listSearchKey(list *list, void *key)
{
    listIter *iter;
    listNode *node;

    // ʹ�õ��������
    iter = listGetIterator(list, AL_START_HEAD);
    while((node = listNext(iter)) != NULL) {
        if (list->match) {
            // ʹ���б��Դ��ƥ�������бȶ�
            if (list->match(node->value, key)) {
                listReleaseIterator(iter);
                return node;
            }
        } else {
            // ֱ�����б��ֵ���ȶ�
            if (key == node->value) {
                listReleaseIterator(iter);
                return node;
            }
        }
    }

    // û�ҵ�
    listReleaseIterator(iter);
    return NULL;
}

/*
 * ��ݸ�������б��ж�Ӧ�Ľڵ�
 *
 * �������������Ҳ�����Ǹ���
 * ����� 0 ��ʼ�����ɱ�ͷ��ʼ������� -1 ��ʼ�����ɱ�β��ʼ��
 *
 * ����������б�ķ��أ����� NULL ��
 *
 * T = O(N)��N Ϊ�б�ĳ���
 */
listNode *listIndex(list *list, long index) {
    listNode *n;

    if (index < 0) {
        index = (-index)-1;
        n = list->tail;
        while(index-- && n) n = n->prev;
    } else {
        n = list->head;
        while(index-- && n) n = n->next;
    }

    return n;
}

/*
 * ȡ���б��β�ڵ㣬������뵽��ͷ����Ϊ�µı�ͷ�ڵ�
 *
 * T = O(1)
 */
void listRotate(list *list) {
    listNode *tail = list->tail;

    // �б�ֻ��һ��Ԫ��
    if (listLength(list) <= 1) return;

    // ȡ��β�ڵ�
    list->tail = tail->prev;
    list->tail->next = NULL;
    // ������뵽��ͷ
    list->head->prev = tail;
    tail->prev = NULL;
    tail->next = list->head;
    list->head = tail;
}
