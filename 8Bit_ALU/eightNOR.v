module eightNOR(a,b,w);
input [7:0] a,b;
output [7:0] w;
nor nor0(w[0], a[0], b[0]);
nor nor1(w[1], a[1], b[1]);
nor nor2(w[2], a[2], b[2]);
nor nor3(w[3], a[3], b[3]);
nor nor4(w[4], a[4], b[4]);
nor nor5(w[5], a[5], b[5]);
nor nor6(w[6], a[6], b[6]);
nor nor7(w[7], a[7], b[7]);
endmodule
