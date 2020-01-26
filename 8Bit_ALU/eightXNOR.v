module eightXNOR(a,b,w);
input [7:0] a,b;
output [7:0] w;
xnor xnor0(w[0], a[0], b[0]);
xnor xnor1(w[1], a[1], b[1]);
xnor xnor2(w[2], a[2], b[2]);
xnor xnor3(w[3], a[3], b[3]);
xnor xnor4(w[4], a[4], b[4]);
xnor xnor5(w[5], a[5], b[5]);
xnor xnor6(w[6], a[6], b[6]);
xnor xnor7(w[7], a[7], b[7]);
endmodule
