
X = "aeaca";
Y = "acea";
m = len(X);
n = len(Y);

# r is number of pairs (i, j) such that X[i] = Y[i];

# R is the set of pairs (i, j) such that X[i] = Y[i];
R = set();

x_lex = sorted(enumerate(X), key = lambda e: e[1]);
y_lex = sorted(enumerate(Y), key = lambda e: e[1]);

print(x_lex);
print(y_lex);

i = 0;
j = 0;

while i < m and j < n:
	x_e = x_lex[i][1];
	y_e = y_lex[j][1];
	if x_e < y_e:
		i += 1;
	elif x_e > y_e:
		j += 1;
	else:
		new_i = i;
		new_j = j;
		while new_i < m and x_e == x_lex[new_i][1]: new_i += 1;
		while new_j < n and y_e == y_lex[new_j][1]: new_j += 1;
		for a in range(i, new_i):
			for b in range(j, new_j):
				R.add((x_lex[a][0]+1, y_lex[b][0]+1));
		i = new_i;
		j = new_j;

assert(R == set((i+1, j+1) for i in range(m) for j in range(n) if X[i] == Y[j]));

R = sorted(R);

print(R);

R_i = dict();

s = None;
last_i = None;
for i, j in R:
	if i != last_i:
		s = set();
		R_i[i] = s;
		last_i = i;
	s.add(j);

print(R_i);

class node:
	def __init__(self, i, j):
		self.i = i;
		self.j = j;
		self.prev = None;
	def __str__(self):
		return f"node({self.i}, {self.j})";
	def __lt__(self, another):
		return self.j < another.j;

T = list();

def prec(x):
	ret = None;
	for y in T:
		if y.j < x and (ret == None or y.j > ret.j):
			ret = y;
	return ret;

def succ(x):
	ret = None;
	for y in T:
		if y.j > x and (ret == None or y.j < ret.j):
			ret = y;
	return ret;

def print_T():
	x = -1;
	S = set(y.j for y in T);
	for w in range(n+1):
		if w in S:
			x += 1;
		print(x, end=' ');
	print();

T.append(node(0, 0));

print(list(map(str, sorted(T))));
print_T();

for i in range(1, m+1):
	for j in sorted(R_i[i], reverse = True):
		print(f"i = {i}, j = {j}");
		if not any(y.j == j for y in T):
			y = succ(j);
			if y is None:
				print("no successor");
				v = node(i, j);
				v.prev = prec(j);
				T.append(v);
			elif y.i == i:
				print("update");
				y.j = j;
			else:
				print("remove old, insert new");
				T.remove(y);
				v = node(i, j);
				v.prev = prec(j);
				T.append(v);
		else:
			print("already head");
		print(list(map(str, sorted(T))));
		print_T();

print(f"len(LCS) == {len(T)-1}");

LCS = str();

y = max(T, key = lambda e : e.j);

while y.i > 0:
	print(y);
	LCS = X[y.i-1] + LCS;
	y = y.prev;

print(f"LCS = {LCS}");

table = [[0 for _ in range(n + 1)] for _ in range(m + 1)];

table[0][0] = 0;

for i in range(m): table[i+1][0] = 0;
for j in range(n): table[0][j+1] = 0;

for i in range(m):
	for j in range(n):
		table[i+1][j+1] = 1 + table[i][j] if X[i] == Y[j] else max(table[i][j+1], table[i+1][j]);

print('  ', end='');
for j in range(n):
	print(Y[j], end='');
print();
for i in range(m+1):
	print(' ' if i == 0 else X[i-1], end='');
	for j in range(n+1):
		print(table[i][j], end='');
	print();


i = 0;
j = 0;

for k in LCS:
	while i < m and X[i] != k:
		print(f"-{X[i]}");
		i += 1;
	
	while j < n and Y[j] != k:
		print(f"+{Y[j]}");
		j += 1;
	
	print(f" {k}");
	
	i += 1;
	j += 1;

while i < m:
	print(f"-{X[i]}");
	i += 1;

while j < n:
	print(f"+{Y[j]}");
	j += 1;

















