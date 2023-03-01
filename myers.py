

a = "saturday"
b = "sunday"

N = len(a)
M = len(b)
m = max(N, N)

v = {i: None for i in range(-m, m+1)};

v[1] = 0;

prev = {(0, 0): None};

d = 0;
while d <= m:
	print(f"d = {d}")
	
	k = -d;
	while k <= d:
		print("\t" f"k = {k}")
		
		if k == -d or (k != d and v[k - 1] < v[k + 1]):
			x = v[k+1];
			y = x - k;
			prev[(x, y)] = (x, y-1);
		else:
			x = v[k-1] + 1;
			y = x - k;
			prev[(x, y)] = (x-1, y);
		
		print("\t\t" f"{x, y}");
		
		while x < N and y < M and a[x] == b[y]:
			prev[(x+1, y+1)] = (x, y);
			x += 1;
			y += 1;
			
			print("\t\t" f"{x, y}");
		
		v[k] = x;
		
		if x >= N and y >= M:
			print(f"Length of SES is {d}");
			
			while x > 0 or y > 0:
				print(x, y);
				x, y = prev[(x, y)];
			print(x, y);
			
			exit(0);
		k += 2;
	d += 1;














