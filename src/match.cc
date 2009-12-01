#include <string>
#include "stealth.h"

using namespace std;


int match( const string& s1, const string& s2 )
{
	if( s1.empty() || s2.empty() )
		return 1 ;
	
	return !cmatch( s1.c_str(), s2.c_str() );
}


int cmatch(const char *mask, const char *name)
{
	const char *m = mask;
	const char *n = name;
	const char *ma = mask;
	const char *na = name;
	int wild = 0;
	int q = 0;

	while (1)
	{
		if (*m == '*')
		{
			while (*m == '*')
				m++;
			wild = 1;
			ma = m;
			na = n;
		}

		if (!*m)
		{
			if (!*n)
				return 0;
			for (m--; (m > mask) && (*m == '?'); m--)
				;
			if ((*m == '*') && (m > mask) &&
				(m[-1] != '\\'))
				return 0;
			if (!wild)
				return 1;
			m = ma;
			n = ++na;
		}
		else if (!*n)
		{
			while (*m == '*')
				m++;
			return (*m != 0);
		}
		if ((*m == '\\') && ((m[1] == '*') || (m[1] == '?')))
		{
			m++;
			q = 1;
		}
		else
			q = 0;

		if ((tolower(*m) != tolower(*n)) && ((*m != '?') || q))
		{
			if (!wild)
				return 1;
			m = ma;
			n = ++na;
		}
		else
		{
			if (*m)
				m++;
			if (*n)
				n++;
		}
	}
}

char *collapse(char *pattern)
{
	char *s = pattern, *s1, *t;

	if( (!(pattern) || (*(pattern) == '\0')) )
		return pattern;
	for (; *s; s++)
	{
		if (*s == '\\')
		{
			if (!*(s + 1))
				break;
			else
				s++;
		}
		else if (*s == '*')
		{
			if (*(t = s1 = s + 1) == '*')
				while (*t == '*')
					t++;
			else if (*t == '?')
				for (t++, s1++; *t == '*' || *t == '?'; t++)
					if (*t == '?')
						*s1++ = *t;
			while ((*s1++ = *t++));
		}
	}
	return pattern;
}

int wwcmp(const char *old_mask, const char *new_mask)
{
	register const char *m = old_mask;
	register const char *n = new_mask;
	const char *ma = m;
	const char *na = n;
	int wild = 0;
	int mq = 0, nq = 0;

	while (1)
	{
		if (*m == '*')
		{
			while (*m == '*')
				m++;
			wild = 1;
			ma = m;
			na = n;
		}

		if (!*m)
		{
			if (!*n)
				return 0;
			for (m--; (m > old_mask) && (*m == '?'); m--)
				;
			if ((*m == '*') && (m > old_mask) && (m[-1] != '\\'))
				return 0;
			if (!wild)
				return 1;
			m = ma;

			if ((*na == '\\') && ((na[1] == '*') || (na[1] == '?')))
				++na;

			n = ++na;
		}
		else if (!*n)
		{
			while (*m == '*')
				m++;
			return (*m != 0);
		}
		if ((*m == '\\') && ((m[1] == '*') || (m[1] == '?')))
		{
			m++;
			mq = 1;
		}
		else
		mq = 0;

		if ((*n == '\\') && ((n[1] == '*') || (n[1] == '?')))
		{
			n++;
			nq = 1;
		}
		else
		nq = 0;
		if ((*m == '*' && !mq) ||
			((!mq || nq) && tolower(*m) == tolower(*n)) ||
			(*m == '?' && !mq && (*n != '*' || nq)))
		{
			if (*m)
				m++;
			if (*n)
				n++;
		}
		else
		{
			if (!wild)
				return 1;
			m = ma;

			if ((*na == '\\') && ((na[1] == '*') || (na[1] == '?')))
				++na;

			n = ++na;
		}
	}
}

int comp(char *str1, char *str2, int len)
{
	int i = 0;
	while(*str1 && i < len)
	{
		if(tolower(*str1) == tolower(*str2))
		{
			str1++;
			str2++;
		}
		else if(*str1 == '?' || *str2 == '?' || *str1 == '*' || *str2 == '*')
		{
			str1++;
			str2++;
		}
		else if(*str1 && !*str2) return 1;
		else if(tolower(*str1) != tolower(*str2)) return 1;
		i++;
	}
	return 0;
}

