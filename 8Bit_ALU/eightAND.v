module eightAND(a,b,w);
input [7:0] a,b;
output [7:0] w;

and and0(w[0], a[0], b[0]);
and and1(w[1], a[1], b[1]);
and and2(w[2], a[2], b[2]);
and and3(w[3], a[3], b[3]);
and and4(w[4], a[4], b[4]);
and and5(w[5], a[5], b[5]);
and and6(w[6], a[6], b[6]);
and and7(w[7], a[7], b[7]);

endmodule
