#include <amneiht/asip.h>
#include <amneiht/asupport.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

int praseMess(asip_d_mess **point, char *mess, int start, int length);

void asip_m_addHeader(asip_d_mess **point, char *name, char *value) {
	asip_d_mess *pt = *point;
	if (pt == null) {
		*point = (asip_d_mess*) malloc(sizeof(asip_d_mess));
		pt = *point;
		pt->next = null;
		pt->name = asup_m_convertPointer(name);
		pt->value = asup_m_convertPointer(value);
	} else {

		while (pt->next != null) {
			pt = pt->next;
		}
		pt->next = (asip_d_mess*) malloc(sizeof(asip_d_mess));

		pt = pt->next;
		pt->next = null;
		pt->name = asup_m_convertPointer(name);
		pt->value = asup_m_convertPointer(value);
	}
}
int asip_m_toStringMess(asip_d_mess *pt, int leng, char *ps) {
	int i, k;
	k = 0; // bien dem length
	while (pt != null) {
		int d = strlen(pt->name);
		for (i = 0; i < d; i++) {
			ps[k] = (pt->name)[i];
			k++;
		}
		ps[k] = ':';
		k++;
		d = strlen(pt->value);
		for (i = 0; i < d; i++) {
			ps[k] = (pt->value)[i];
			k++;
		}
		ps[k] = '\n';
		k++;
		pt = pt->next;
	}
	ps[k] = asip_eos;
	return k + 1;

}

void asip_m_freeMess(asip_d_mess *pt) {
	asip_d_mess *ps;
	while (pt != NULL) {
		free(pt->name);
		free(pt->value);
		ps = pt->next;
		free(pt);
		pt = ps;
	}

}
char* asip_m_findHeaderValue(asip_d_mess *pt, char *test) {
	while (pt != NULL) {
		if (strcasecmp(pt->name, test) == 0) {
			return pt->value;
		}
		pt = pt->next;
	}
	return NULL;
}
int asip_m_parseMess(asip_d_mess **point, char *mess) {
	int length = strlen(mess);
	return praseMess(point, mess, 0, length);
}

int praseMess(asip_d_mess **point, char *tin, int start, int length) {
	if (start >= length)
		return 1;
	int j, i = start;
	for (; i < length; i++) {
		if (tin[i] == '\n') {
			break;
		}
	}
	int lg = i - start;
	char ls[lg + 1];
	for (j = 0; j < lg; j++) {
		ls[j] = tin[j + start];
	}
	ls[lg] = '\0';
//	printf("%s + %d\n",ls,lg);
	int m = asup_m_find(ls, ':');
	if (m == -1)
		return -1;
	*point = (asip_d_mess*) calloc(1, sizeof(asip_d_mess));
	asip_d_mess *sp = *point;

	char header[m + 1]; // luu chuoi header
	char value[lg - m]; // luu chuoi value
	for (i = 0; i < m; i++) {
		header[i] = ls[i];
	}
	header[m] = '\0';
	for (i = m + 1; i < lg; i++) {
		value[i - m - 1] = ls[i];
	}
	value[lg - m - 1] = '\0';

	sp->name = asup_m_trim(header);
	sp->value = asup_m_trim(value);
	sp->next = NULL;
	return praseMess(&(sp->next), tin, lg + start + 1, length);
}

