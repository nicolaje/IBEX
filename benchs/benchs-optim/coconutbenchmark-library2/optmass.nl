g3 0 1 0	# problem optmass
 66 55 1 0 44	# vars, constraints, objectives, ranges, eqns
 11 1	# nonlinear constraints, objectives
 0 0	# network constraints: nonlinear, linear
 22 26 0	# nonlinear vars in constraints, objectives, both
 0 0 0 1	# linear network variables; functions; arith, flags
 0 0 0 0 0	# discrete variables: binary, integer, nonlinear (b,c,o)
 170 4	# nonzeros in Jacobian, gradients
 0 0	# max name lengths: constraints, variables
 0 0 0 0 0	# common exprs: b,c,o,c1,o1
C0	#cons3[0]
o0	# + 
o5	#^
v0	#f[1,0]
n2
o5	#^
v11	#f[2,0]
n2
C1	#cons3[1]
o0	# + 
o5	#^
v1	#f[1,1]
n2
o5	#^
v12	#f[2,1]
n2
C2	#cons3[2]
o0	# + 
o5	#^
v2	#f[1,2]
n2
o5	#^
v13	#f[2,2]
n2
C3	#cons3[3]
o0	# + 
o5	#^
v3	#f[1,3]
n2
o5	#^
v14	#f[2,3]
n2
C4	#cons3[4]
o0	# + 
o5	#^
v4	#f[1,4]
n2
o5	#^
v15	#f[2,4]
n2
C5	#cons3[5]
o0	# + 
o5	#^
v5	#f[1,5]
n2
o5	#^
v16	#f[2,5]
n2
C6	#cons3[6]
o0	# + 
o5	#^
v6	#f[1,6]
n2
o5	#^
v17	#f[2,6]
n2
C7	#cons3[7]
o0	# + 
o5	#^
v7	#f[1,7]
n2
o5	#^
v18	#f[2,7]
n2
C8	#cons3[8]
o0	# + 
o5	#^
v8	#f[1,8]
n2
o5	#^
v19	#f[2,8]
n2
C9	#cons3[9]
o0	# + 
o5	#^
v9	#f[1,9]
n2
o5	#^
v20	#f[2,9]
n2
C10	#cons3[10]
o0	# + 
o5	#^
v10	#f[1,10]
n2
o5	#^
v21	#f[2,10]
n2
C11	#cons1[1,1]
n0
C12	#cons1[1,2]
n0
C13	#cons1[2,1]
n0
C14	#cons1[2,2]
n0
C15	#cons1[3,1]
n0
C16	#cons1[3,2]
n0
C17	#cons1[4,1]
n0
C18	#cons1[4,2]
n0
C19	#cons1[5,1]
n0
C20	#cons1[5,2]
n0
C21	#cons1[6,1]
n0
C22	#cons1[6,2]
n0
C23	#cons1[7,1]
n0
C24	#cons1[7,2]
n0
C25	#cons1[8,1]
n0
C26	#cons1[8,2]
n0
C27	#cons1[9,1]
n0
C28	#cons1[9,2]
n0
C29	#cons1[10,1]
n0
C30	#cons1[10,2]
n0
C31	#cons1[11,1]
n0
C32	#cons1[11,2]
n0
C33	#cons2[1,1]
n0
C34	#cons2[1,2]
n0
C35	#cons2[2,1]
n0
C36	#cons2[2,2]
n0
C37	#cons2[3,1]
n0
C38	#cons2[3,2]
n0
C39	#cons2[4,1]
n0
C40	#cons2[4,2]
n0
C41	#cons2[5,1]
n0
C42	#cons2[5,2]
n0
C43	#cons2[6,1]
n0
C44	#cons2[6,2]
n0
C45	#cons2[7,1]
n0
C46	#cons2[7,2]
n0
C47	#cons2[8,1]
n0
C48	#cons2[8,2]
n0
C49	#cons2[9,1]
n0
C50	#cons2[9,2]
n0
C51	#cons2[10,1]
n0
C52	#cons2[10,2]
n0
C53	#cons2[11,1]
n0
C54	#cons2[11,2]
n0
O0 0	#obj
o54	#sumlist
4
o2	#*
n0.335
o5	#^
v24	#v[1,11]
n2
o2	#*
n0.335
o5	#^
v25	#v[2,11]
n2
o16	#-
o5	#^
v22	#x[1,11]
n2
o16	#-
o5	#^
v23	#x[2,11]
n2
r	#55 ranges (rhs's)
1 1
1 1
1 1
1 1
1 1
1 1
1 1
1 1
1 1
1 1
1 1
4 0.001
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0.01
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
4 0
b	#66 bounds (on variables)
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
0 -1.e8 1.e8
k65	#intermediate Jacobian column lengths
3
6
9
12
15
18
21
24
27
30
33
36
39
42
45
48
51
54
57
60
63
66
67
68
69
70
72
74
76
78
80
82
84
86
88
90
92
94
96
98
100
102
104
106
108
110
113
116
119
122
125
128
131
134
137
140
143
146
149
152
155
158
161
164
167
J0 2
0 0
11 0
J1 2
1 0
12 0
J2 2
2 0
13 0
J3 2
3 0
14 0
J4 2
4 0
15 0
J5 2
5 0
16 0
J6 2
6 0
17 0
J7 2
7 0
18 0
J8 2
8 0
19 0
J9 2
9 0
20 0
J10 2
10 0
21 0
J11 2
0 -0.005
26 1
J12 2
11 -0.005
36 1
J13 4
1 -0.005
26 -1
27 1
46 -0.1
J14 4
12 -0.005
36 -1
37 1
56 -0.1
J15 4
2 -0.005
27 -1
28 1
47 -0.1
J16 4
13 -0.005
37 -1
38 1
57 -0.1
J17 4
3 -0.005
28 -1
29 1
48 -0.1
J18 4
14 -0.005
38 -1
39 1
58 -0.1
J19 4
4 -0.005
29 -1
30 1
49 -0.1
J20 4
15 -0.005
39 -1
40 1
59 -0.1
J21 4
5 -0.005
30 -1
31 1
50 -0.1
J22 4
16 -0.005
40 -1
41 1
60 -0.1
J23 4
6 -0.005
31 -1
32 1
51 -0.1
J24 4
17 -0.005
41 -1
42 1
61 -0.1
J25 4
7 -0.005
32 -1
33 1
52 -0.1
J26 4
18 -0.005
42 -1
43 1
62 -0.1
J27 4
8 -0.005
33 -1
34 1
53 -0.1
J28 4
19 -0.005
43 -1
44 1
63 -0.1
J29 4
9 -0.005
34 -1
35 1
54 -0.1
J30 4
20 -0.005
44 -1
45 1
64 -0.1
J31 4
10 -0.005
22 1
35 -1
55 -0.1
J32 4
21 -0.005
23 1
45 -1
65 -0.1
J33 2
0 -0.1
46 1
J34 2
11 -0.1
56 1
J35 3
1 -0.1
46 -1
47 1
J36 3
12 -0.1
56 -1
57 1
J37 3
2 -0.1
47 -1
48 1
J38 3
13 -0.1
57 -1
58 1
J39 3
3 -0.1
48 -1
49 1
J40 3
14 -0.1
58 -1
59 1
J41 3
4 -0.1
49 -1
50 1
J42 3
15 -0.1
59 -1
60 1
J43 3
5 -0.1
50 -1
51 1
J44 3
16 -0.1
60 -1
61 1
J45 3
6 -0.1
51 -1
52 1
J46 3
17 -0.1
61 -1
62 1
J47 3
7 -0.1
52 -1
53 1
J48 3
18 -0.1
62 -1
63 1
J49 3
8 -0.1
53 -1
54 1
J50 3
19 -0.1
63 -1
64 1
J51 3
9 -0.1
54 -1
55 1
J52 3
20 -0.1
64 -1
65 1
J53 3
10 -0.1
24 1
55 -1
J54 3
21 -0.1
25 1
65 -1
G0 4
22 0
23 0
24 0
25 0
