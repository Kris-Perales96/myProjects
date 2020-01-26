module eightXOR(a,b,w);
input [7:0] a,b;
output [7:0] w;						
xor xor0(w[0], a[0], b[0]);
xor xor1(w[1], a[1], b[1]);
xor xor2(w[2], a[2], b[2]);
xor xor3(w[3], a[3], b[3]);
xor xor4(w[4], a[4], b[4]);
xor xor5(w[5], a[5], b[5]);
xor xor6(w[6], a[6], b[6]);
xor xor7(w[7], a[7], b[7]);

endmodule
