module eightComplement(a, b, out);
input [7:0] a,b;
output [7:0] out;

xor xor0(out[0], a[0], 1'b1);
xor xor1(out[1], a[1], 1'b1);
xor xor2(out[2], a[2], 1'b1);
xor xor3(out[3], a[3], 1'b1);
xor xor4(out[4], a[4], 1'b1);
xor xor5(out[5], a[5], 1'b1);
xor xor6(out[6], a[6], 1'b1);
xor xor7(out[7], a[7], 1'b1);

endmodule
