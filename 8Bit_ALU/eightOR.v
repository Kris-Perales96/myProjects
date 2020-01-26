module eightOR(a,b,w);
input [7:0] a,b;
output [7:0]w;

or or0(w[0], a[0], b[0]);
or or1(w[1], a[1], b[1]);
or or2(w[2], a[2], b[2]);
or or3(w[3], a[3], b[3]);
or or4(w[4], a[4], b[4]);
or or5(w[5], a[5], b[5]);
or or6(w[6], a[6], b[6]);
or or7(w[7], a[7], b[7]);

endmodule
