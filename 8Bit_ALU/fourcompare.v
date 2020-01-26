module fourcompare(LT, GT, a, b);
output LT, GT;
input [3:0] a,b;

wire [21:1] w;

nand n1(w[1], a[3], ~b[3]);
nand n2(w[3], ~a[3], b[3]);
nand n3(w[2], w[1], w[3]);
nand n4(w[4], a[2], ~b[2]);
nand n5(w[6], ~a[2], b[2]);
nand n6(w[5], w[4], w[6]);
nand n7(w[7], a[1], ~b[1]);
nand n8(w[8], ~a[1], b[1]);
nand n9(w[9], w[7], w[8]);
nand n10(w[10], a[0], ~b[0]);
nand n11(w[11], ~a[0], b[0]);
nand n12(GT, w[20], w[1]);
nand n13(LT, w[21], w[3]);

or o1(w[12], w[10], w[9]);
or o2(w[13], w[11], w[9]);
and a1(w[14], w[12], w[7]);
and a2(w[15], w[8], w[13]);
or o3(w[16], w[14], w[5]);
or o4(w[17], w[15], w[5]);
and a3(w[18], w[16], w[4]);
and a4(w[19], w[17], w[6]);
or o5(w[20], w[18], w[2]);
or o6(w[21], w[19], w[2]);

endmodule

