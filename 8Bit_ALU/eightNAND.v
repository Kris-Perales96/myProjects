module eightNAND(a,b,w);
input [7:0] a,b;
output [7:0] w;
nand nand0(w[0], a[0], b[0]);
nand nand1(w[1], a[1], b[1]);
nand nand2(w[2], a[2], b[2]);
nand nand3(w[3], a[3], b[3]);
nand nand4(w[4], a[4], b[4]);
nand nand5(w[5], a[5], b[5]);
nand nand6(w[6], a[6], b[6]);
nand nand7(w[7], a[7], b[7]);
endmodule
